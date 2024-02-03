#ifndef __PIN_H__
#define __PIN_H__

#include "./../ecp_conf.h"

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

    void (*init)(int index, uint32_t mode);
    void (*write)(int index, int value);
    int (*read)(int index);
};
typedef struct pin pin_dev;

int ecp_pin_register(pin_dev* dev);

#endif  // !__PIN_H__
