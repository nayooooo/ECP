#include "soft_i2c.h"

#if ECP_USE_STM32F407

    void ecp_soft_i2c_init(struct soft_i2c* dev, int sda_pin_index, int scl_pin_index, struct soft_i2c_slave_info slave_info)
    {
        if (dev == nullptr) return;

        if (slave_info.dev_addr_length > SOFT_I2C_SLAVE_DEV_ADDR_LENGTH_MAX) return;
        if (slave_info.dev_mem_length > SOFT_I2C_SLAVE_MEM_ADDR_LENGTH_MAX) return;

        dev->slave.dev_addr = slave_info.dev_addr;
        dev->slave.dev_addr_length = slave_info.dev_addr_length;
        dev->slave.mem_addr_length = slave_info.mem_addr_length;

        dev->sda.init(&dev->sda, sda_pin_index, PIN_MODE_OUTPUT);
        dev->scl.init(&dev->scl, scl_pin_index, PIN_MODE_OUTPUT);
        dev->scl.write(&dev->scl, PIN_HIGH);
        dev->sda.write(&dev->sda, PIN_HIGH);

        ecp_stm32_soft_i2c_init();
    }

    void ecp_soft_i2c_write(struct soft_i2c* dev, uint16_t reg_addr, uint8_t* buf, uint32_t buf_len, uint32_t wwn, uint32_t* awn)
    {
        if (dev == nullptr) return;
        if (buf == nullptr) return;
        if (awn == nullptr) return;

        ecp_stm32_soft_i2c_write(dev->sda.index, dev->scl.index,
                                 dev->slave.dev_addr, dev->slave.dev_addr_length, reg_addr, dev->slave.mem_addr_length,
                                 buf, buf_len, wwn, awn);
    }

    void ecp_soft_i2c_read(struct soft_i2c* dev, uint16_t reg_addr, uint8_t* buf, uint32_t buf_len, uint32_t wrn, uint32_t* arn)
    {
        if (dev == nullptr) return;
        if (buf == nullptr) return;
        if (arn == nullptr) return;

        ecp_stm32_soft_i2c_read(dev->sda.index, dev->scl.index,
                                dev->slave.dev_addr, dev->slave.dev_addr_length, reg_addr, dev->slave.mem_addr_length,
                                buf, buf_len, wrn, arn);
    }

#endif  // ECP_USE_STM32F407

int ecp_soft_i2c_register(soft_i2c_dev* dev)
{
    if (dev == nullptr) return -1;

    ecp_pin_register(dev->sda);
    ecp_pin_register(dev->scl);

    dev->init = ecp_soft_i2c_init;
    dev->write = ecp_soft_i2c_write;
    dev->read = ecp_soft_i2c_read;

    return 0;
}
