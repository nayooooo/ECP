/**
 * -*- coding: utf-8 -*-
 * @file soft_i2c.h
 * @author yewan (YEWANhup@outlook.com)
 * @brief 软件模拟I2C
 * @version 0.1
 * @date 2024-02-03
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __SOFT_I2C_H__
#define __SOFT_I2C_H__

#include "./../ecp.h"

#define SOFT_I2C_SLAVE_DEV_ADDR_LENGTH_MAX          (2)
#define SOFT_I2C_SLAVE_MEM_ADDR_LENGTH_MAX          (2)

struct soft_i2c_slave_info
{
    uint16_t dev_addr;                  // 设备地址
    uint8_t dev_addr_length;            // 设备地址的长度（单位：字节）
    uint8_t mem_addr_length;            // 内存地址的长度
};

struct soft_i2c
{
    pin_dev sda;
    pin_dev scl;

    struct soft_i2c_slave_info slave;

    void (*init)(struct soft_i2c* dev, int sda_pin_index, int scl_pin_index, struct soft_i2c_slave_info slave_info);
    /**
     * @brief 软件I2C写数据
     *
     * @param dev soft_i2c设备
     * @param buf 待写入的数据缓冲区指针
     * @param buf_len 缓冲区大小
     * @param wwn want write number 想要写入的数据量
     * @param awn actual write number 实际写入的数据量
     */
    void (*write)(struct soft_i2c* dev, uint16_t reg_addr, uint8_t* buf, uint32_t buf_len, uint32_t wwn, uint32_t* awn);
    void (*read)(struct soft_i2c* dev, uint16_t reg_addr, uint8_t* buf, uint32_t buf_len, uint32_t wrn, uint32_t* arn);
};
typedef struct soft_i2c soft_i2c_dev;

int ecp_soft_i2c_register(soft_i2c_dev* dev);

#endif  // !__SOFT_I2C_H__
