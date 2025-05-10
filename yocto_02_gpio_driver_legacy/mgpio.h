#ifndef RASPBERRY_PI_ZERO_W_GPIO_H
#define RASPBERRY_PI_ZERO_W_GPIO_H

#define BCM2835_GPIO_BASE_ADDR 0x20200000 // Base address for Raspberry Pi Zero W (BCM2835)
#define GPIO_FSEL_OFFSET 0x00 // Function select register offset
#define GPIO_SET_0_OFFSET 0x1C // Set register offset
#define GPIO_CLR_0_OFFSET 0x28 // Clear register offset
#define GPIO_NUMBER_27 27 // GPIO pin 27

#endif // RASPBERRY_PI_ZERO_W_GPIO_H
