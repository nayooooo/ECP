#ifndef __DRV_SOFT_I2C_H__
#define __DRV_SOFT_I2C_H__

#include "./../ecp_port_stm32f407_hal.h"

/*==================================================================
    soft_i2c
==================================================================*/
void ecp_stm32_soft_i2c_init(void);
/**
 * @brief 暂时只支持设备地址为8位的设备
 *
 * @param sda_pin_index sda引脚序号
 * @param scl_pin_index scl引脚序号
 * @param slave_addr 从机地址
 * @param slave_addr_length 从机地址长度
 * @param reg_addr 寄存器地址
 * @param mem_addr_length 寄存器地址长度
 * @param buf 数据缓冲区指针
 * @param buf_len 缓冲区长度
 * @param wwn 想要写入数据量
 * @param awn 实际写入数据量
 */
void ecp_stm32_soft_i2c_write(
    int sda_pin_index, int scl_pin_index,
    uint16_t slave_addr, uint8_t slave_addr_length, uint16_t reg_addr, uint8_t mem_addr_length,
    uint8_t* buf, uint32_t buf_len, uint32_t wwn, uint32_t* awn
);
/**
 * @brief 暂时只支持设备地址为8位的设备
 *
 * @param sda_pin_index sda引脚序号
 * @param scl_pin_index scl引脚序号
 * @param slave_addr 从机地址
 * @param slave_addr_length 从机地址长度
 * @param reg_addr 寄存器地址
 * @param mem_addr_length 寄存器地址长度
 * @param buf 数据缓冲区指针
 * @param buf_len 缓冲区长度
 * @param wrn 想要读出数据量
 * @param arn 实际读出数据量
 */
void ecp_stm32_soft_i2c_read(
    int sda_pin_index, int scl_pin_index,
    uint16_t slave_addr, uint8_t slave_addr_length, uint16_t reg_addr, uint8_t mem_addr_length,
    uint8_t* buf, uint32_t buf_len, uint32_t wrn, uint32_t* arn
);

#endif  // !__DRV_SOFT_I2C_H__
