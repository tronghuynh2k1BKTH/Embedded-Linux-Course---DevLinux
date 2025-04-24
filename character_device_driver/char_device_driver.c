#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DRIVER_AUTHOR "huynpham huynh@gmail.com"
#define DRIVER_DESC   "character device driver example"
#define DRIVER_VERS   "1.0"
#define BUF_PAGES     1

struct simple_dev {
    int32_t buf_size;
    char *kernel_buf;
    dev_t dev_number;
    struct class *dev_class;
    struct cdev char_dev;
} mydev;

static int      __init simple_init(void);
static void     __exit simple_exit(void);
static int      dev_open(struct inode *inode, struct file *file);
static int      dev_close(struct inode *inode, struct file *file);
static ssize_t  dev_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset);
static ssize_t  dev_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset);

static struct file_operations fops = {
    .owner      = THIS_MODULE,
    .read       = dev_read,
    .write      = dev_write,
    .open       = dev_open,
    .release    = dev_close,
};

static int dev_open(struct inode *inode, struct file *file)
{
    pr_info("Device open() called\n");
    return 0;
}

static int dev_close(struct inode *inode, struct file *file)
{
    pr_info("Device close() called\n");
    return 0;
}

static ssize_t dev_read(struct file *filp, char __user *user_buf, size_t size, loff_t *offset)
{
    size_t bytes_to_read;

    pr_info("Device read() called\n");

    bytes_to_read = (size > mydev.buf_size - *offset) ? (mydev.buf_size - *offset) : size;

    if (copy_to_user(user_buf, mydev.kernel_buf + *offset, bytes_to_read))
        return -EFAULT;

    *offset += bytes_to_read;
    return bytes_to_read;
}

static ssize_t dev_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *offset)
{
    size_t bytes_to_write;

    pr_info("Device write() called\n");

    bytes_to_write = (size + *offset > BUF_PAGES * PAGE_SIZE) ? (BUF_PAGES * PAGE_SIZE - *offset) : size;

    memset(mydev.kernel_buf, 0, BUF_PAGES * PAGE_SIZE);
    if (copy_from_user(mydev.kernel_buf + *offset, user_buf, bytes_to_write))
        return -EFAULT;

    pr_info("Data from user: %s\n", mydev.kernel_buf);

    *offset += bytes_to_write;
    mydev.buf_size = *offset;

    return bytes_to_write;
}

static int __init simple_init(void)
{
    if (alloc_chrdev_region(&mydev.dev_number, 0, 1, "simple_cdev") < 0) {
        pr_err("Failed to allocate device number\n");
        return -1;
    }

    pr_info("Major = %d, Minor = %d\n", MAJOR(mydev.dev_number), MINOR(mydev.dev_number));

    if ((mydev.dev_class = class_create(THIS_MODULE, "simple_class")) == NULL) {
        pr_err("Failed to create device class\n");
        goto unregister_region;
    }

    if ((device_create(mydev.dev_class, NULL, mydev.dev_number, NULL, "simple_dev")) == NULL) {
        pr_err("Failed to create device file\n");
        goto destroy_class;
    }

    cdev_init(&mydev.char_dev, &fops);
    if (cdev_add(&mydev.char_dev, mydev.dev_number, 1) < 0) {
        pr_err("Failed to add device to system\n");
        goto destroy_device;
    }

    mydev.kernel_buf = kmalloc(BUF_PAGES * PAGE_SIZE, GFP_KERNEL);
    if (!mydev.kernel_buf) {
        pr_err("Failed to allocate kernel buffer\n");
        goto destroy_device;
    }

    mydev.buf_size = 0;

    pr_info("Simple char driver loaded\n");
    return 0;

destroy_device:
    device_destroy(mydev.dev_class, mydev.dev_number);
destroy_class:
    class_destroy(mydev.dev_class);
unregister_region:
    unregister_chrdev_region(mydev.dev_number, 1);
    return -1;
}

static void __exit simple_exit(void)
{
    kfree(mydev.kernel_buf);
    cdev_del(&mydev.char_dev);
    device_destroy(mydev.dev_class, mydev.dev_number);
    class_destroy(mydev.dev_class);
    unregister_chrdev_region(mydev.dev_number, 1);
    pr_info("Simple char driver unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERS);