#include "pin.h"

int ecp_pin_register(pin_dev* dev)
{
    if (dev == nullptr) return -1;

    dev->init = ecp_pin_init;
    dev->write = ecp_pin_write;
    dev->read = ecp_pin_read;

    return 0;
}
