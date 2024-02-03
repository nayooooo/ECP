#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "./../ecp.h"

struct soft_i2c
{
    pin_dev sda;
    pin_dev scl;

    void (*init)(int sda_index, int scl_index);
    void (*write)();
};
typedef struct soft_i2c soft_i2c_dev;

int ecp_soft_i2c_register(soft_i2c_dev* dev);

#endif  // !__SOFT_I2C_H__
