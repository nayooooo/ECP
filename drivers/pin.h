#ifndef __PIN_H__
#define __PIN_H__

struct pin;

#include "./../ecp.h"

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01

#if ECP_USE_STM32F407
#define GET_PIN(GPIOx, pin) STM32_GET_PIN(GPIOx, pin)
#endif  // ECP_USE_STM32F407

struct pin
{
    int index;

    void (*init)(struct pin* dev, int index, uint32_t mode);
    void (*write)(struct pin* dev, int value);
    int (*read)(struct pin* dev);
};
typedef struct pin pin_dev;

int ecp_pin_register(pin_dev* dev);

#endif  // !__PIN_H__
