#ifndef __ECP_CONF_H__
#define __ECP_CONF_H__

#include <stdint.h>

#ifndef nullptr
#define nullptr ((void*)0)
#endif  // nullptr

#define ECP_USE_STM32F407           (1)

#if ECP_USE_STM32F407
#include "./Port/STM32F407/ecp_port_stm32f407_hal.h"
#endif  // ECP_USE_STM32F407

#endif // !__ECP_CONF_H__
