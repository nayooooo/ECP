#ifndef __DRV_PIN_H__
#define __DRV_PIN_H__

#include "./../ecp_port_stm32f407_hal.h"

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

void ecp_stm32_pin_init(int index, uint32_t mode);
void ecp_stm32_pin_write(int index, int value);
int ecp_stm32_pin_read(int index);

#endif  // !__DRV_PIN_H__
