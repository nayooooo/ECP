#ifndef __ECP_H__
#define __ECP_H__

#include <stdint.h>

#ifndef nullptr
#define nullptr ((void*)0)
#endif  // nullptr

#define ECP_USE_STM32F407           (1)

#if ECP_USE_STM32F407
    #include "./port/STM32F407/ecp_port_stm32f407_hal.h"
#endif  // ECP_USE_STM32F407

#include "./drivers/pin.h"
#include "./drivers/soft_i2c.h"

#endif // !__ECP_H__
