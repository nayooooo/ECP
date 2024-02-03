#include "pin.h"

#if ECP_USE_STM32F407

	static void ecp_pin_init(struct pin* dev, uint32_t mode)
	{
		if (dev == nullptr) return;
		
		if (mode == PIN_MODE_OUTPUT) mode = GPIO_MODE_OUTPUT_PP;
		else if (mode == PIN_MODE_INPUT) mode = GPIO_MODE_INPUT;
		else return;
		
		ecp_stm32_pin_init(dev->index, mode);
	}
	
    static void ecp_pin_write(struct pin* dev, int value)
	{
		if (dev == nullptr) return;
		
		ecp_stm32_pin_write(dev->index, (int)((value == PIN_HIGH)?(GPIO_PIN_SET):(GPIO_PIN_RESET)));
	}
	
    static int ecp_pin_read(struct pin* dev)
	{
		if (dev == nullptr) return -1;
		
		return ecp_stm32_pin_read(dev->index);
	}

#endif  // ECP_USE_STM32F407

int ecp_pin_register(pin_dev* dev)
{
    if (dev == nullptr) return -1;

    dev->init = ecp_pin_init;
    dev->write = ecp_pin_write;
    dev->read = ecp_pin_read;

    return 0;
}
