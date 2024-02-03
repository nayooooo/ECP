#ifndef __ECP_PORT_STM32F407_HAL_H__
#define __ECP_PORT_STM32F407_HAL_H__

#include "main.h"

/*==================================================================
    pin
==================================================================*/
#define __STM32_PIN(index, gpio, gpio_index) \
    { \
        index, GPIO##gpio, GPIO_PIN_##gpio_index \
    }
#define __STM32_PIN_RESERVE \
    { \
        -1, 0, 0 \
    }

#define STM32_GET_PIN(GPIOx, pin) ((int)((#GPIOx)[0] - 'A') * 16 + pin)

struct pin_index
{
    int index;
    GPIO_TypeDef* gpio;
    uint32_t pin;
};

void ecp_pin_init(int index, uint32_t mode);
void ecp_pin_write(int index, int value);
int ecp_pin_read(int index);

#endif  // __ECP_PORT_STM32F407_HAL_H__
