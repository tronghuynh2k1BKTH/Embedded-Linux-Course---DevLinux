#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include "mgpio.h"

static void __iomem *gpio_base;
static int __init gpio_driver_init(void)
{
    uint32_t reg;

    // Map GPIO registers into kernel virtual address space
    gpio_base = ioremap(BCM2835_GPIO_BASE_ADDR, 0x28);
    if (!gpio_base) {
        pr_err("Failed to ioremap GPIO!\n");
        return -ENOMEM;
    }

    // Configure GPIO27 as output
    reg = ioread32(gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10) * 4);
    reg &= ~(7 << ((GPIO_NUMBER_27 % 10) * 3)); // Clear current function
    reg |= (1 << ((GPIO_NUMBER_27 % 10) * 3));  // Set as output (001)
    iowrite32(reg, gpio_base + GPIO_FSEL_OFFSET + (GPIO_NUMBER_27 / 10) * 4);

    // Set GPIO27 to HIGH
    iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_SET_0_OFFSET);
    pr_info("GPIO27 set to HIGH\n");

    return 0;
}

static void __exit gpio_driver_exit(void)
{
    // Set GPIO27 to LOW before exiting
    iowrite32(1 << GPIO_NUMBER_27, gpio_base + GPIO_CLR_0_OFFSET);
    pr_info("GPIO27 set to LOW\n");

    // Unmap GPIO memory region
    iounmap(gpio_base);
    pr_info("GPIO memory unmapped\n");
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Simple GPIO driver for Raspberry Pi Zero W");
