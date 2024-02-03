#include "drv_soft_i2c.h"

/*==================================================================
    soft_i2c base timing components
==================================================================*/

static void ecp_stm32_soft_i2c_base_set_sda_output(int sda_pin_index)
{
    ecp_stm32_pin_init(sda_pin_index, GPIO_MODE_OUTPUT_PP);
}

static void ecp_stm32_soft_i2c_base_set_sda_input(int sda_pin_index)
{
    ecp_stm32_pin_init(sda_pin_index, GPIO_MODE_INPUT);
}

static void ecp_stm32_soft_i2c_base_start(int sda_pin_index, int scl_pin_index)
{
    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_SET);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
    delay_us(10);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_RESET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    delay_us(10);
}

static void ecp_stm32_soft_i2c_base_stop(int sda_pin_index, int scl_pin_index)
{
    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_RESET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
    delay_us(10);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_SET);
    delay_us(10);
}

static void ecp_stm32_soft_i2c_base_ack(int sda_pin_index, int scl_pin_index)
{
    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_RESET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    delay_us(10);
}

static void ecp_stm32_soft_i2c_base_nack(int sda_pin_index, int scl_pin_index)
{
    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_SET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
    delay_us(10);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    delay_us(10);
}

static uint8_t ecp_stm32_soft_i2c_base_wait_ack(int sda_pin_index, int scl_pin_index)
{
    uint8_t ucErrTime = 0;

    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(sda_pin_index, GPIO_PIN_SET); delay_us(1);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET); delay_us(1);
    ecp_stm32_soft_i2c_base_set_sda_input(sda_pin_index);
    while (ecp_stm32_pin_read(sda_pin_index) == (int)GPIO_PIN_SET)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            ecp_stm32_soft_i2c_base_stop(sda_pin_index, scl_pin_index);
            return GPIO_PIN_SET;
        }
    }
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    return GPIO_PIN_RESET;
}

static void ecp_stm32_soft_i2c_base_write_byte(int sda_pin_index, int scl_pin_index, uint8_t txd)
{
    uint8_t i;

    ecp_stm32_soft_i2c_base_set_sda_output(sda_pin_index);
    ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
    for (i = 0; i < 8; i++)
    {
        ecp_stm32_pin_write(sda_pin_index, ((txd&0x80)>>7)?(GPIO_PIN_SET):(GPIO_PIN_RESET));
        txd <<= 1;
        ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
        delay_us(10);
        ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
        delay_us(10);
    }
}

static uint8_t ecp_stm32_soft_i2c_base_read_byte(int sda_pin_index, int scl_pin_index, uint8_t ifack)
{
    uint8_t i, rece = 0;

    ecp_stm32_soft_i2c_base_set_sda_input(sda_pin_index);
    for (i = 0; i < 8; i++)
    {
        ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_RESET);
        delay_us(30);
        ecp_stm32_pin_write(scl_pin_index, GPIO_PIN_SET);
        rece <<= 1;
        if (ecp_stm32_pin_read(sda_pin_index) == (int)GPIO_PIN_SET)
            rece++;
    }
    if (ifack) ecp_stm32_soft_i2c_base_ack(sda_pin_index, scl_pin_index);
    else ecp_stm32_soft_i2c_base_nack(sda_pin_index, scl_pin_index);

    return rece;
}

/*==================================================================
    soft_i2c
==================================================================*/
void ecp_stm32_soft_i2c_init(void)
{}

void ecp_stm32_soft_i2c_write(
    int sda_pin_index, int scl_pin_index,
    uint16_t slave_addr, uint8_t slave_addr_length, uint16_t reg_addr, uint8_t mem_addr_length,
    uint8_t* buf, uint32_t buf_len, uint32_t wwn, uint32_t* awn
)
{
    uint32_t i;

    if (slave_addr_length > 1) return;
    if (wwn > buf_len) return;

    ecp_stm32_soft_i2c_base_start(sda_pin_index, scl_pin_index);
    ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, slave_addr|0x00);
    ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    if (mem_addr_length > 1)
    {
        ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, reg_addr>>8);
        ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    }
    ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, reg_addr&0xFF);
    ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    *awn = 0;
    for (i = 0; i < wwn; i++)
    {
        ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, buf[i]);
        if (GPIO_PIN_SET == ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index)) break;
        *awn = *awn + 1;
    }
    ecp_stm32_soft_i2c_base_stop(sda_pin_index, scl_pin_index);
}

void ecp_stm32_soft_i2c_read(
    int sda_pin_index, int scl_pin_index,
    uint16_t slave_addr, uint8_t slave_addr_length, uint16_t reg_addr, uint8_t mem_addr_length,
    uint8_t* buf, uint32_t buf_len, uint32_t wrn, uint32_t* arn
)
{
    uint32_t i;

    if (slave_addr_length > 1) return;

    ecp_stm32_soft_i2c_base_start(sda_pin_index, scl_pin_index);
    ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, slave_addr|0x00);
    ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    if (mem_addr_length > 1)
    {
        ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, reg_addr>>8);
        ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    }
    ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, reg_addr&0xFF);
    ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    ecp_stm32_soft_i2c_base_start(sda_pin_index, scl_pin_index);
    ecp_stm32_soft_i2c_base_write_byte(sda_pin_index, scl_pin_index, slave_addr|0x01);
    ecp_stm32_soft_i2c_base_wait_ack(sda_pin_index, scl_pin_index);
    *arn = 0;
    for (i = 0; i < wrn; i++)
    {
        buf[i] = ecp_stm32_soft_i2c_base_read_byte(sda_pin_index, scl_pin_index, (i==(wrn-1))?0:1);
        *arn = *arn + 1;
    }
    ecp_stm32_soft_i2c_base_stop(sda_pin_index, scl_pin_index);
}
