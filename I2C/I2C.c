#include "I2C.h"
#include "em_i2c.h"
#include "em_cmu.h"

static const I2C_TypeDef *I2C_BASE_ADDRESSES[] = {
    I2C0,
    I2C1,
    I2C2,
    I2C3
};

void i2c_init(I2C_Config *config) {
    I2C_TypeDef *i2c_base = (I2C_TypeDef *) I2C_BASE_ADDRESSES[config->peripheral];
    CMU_ClockEnable(cmuClock_HFPER, true);
    CMU_ClockEnable(cmuClock_I2C0 + config->peripheral, true);
    i2c_base->CTRL = I2C_CTRL_EN;
    i2c_base->ROUTELOC0 |= I2C_ROUTELOC0_SCLLOC_LOC14 | I2C_ROUTELOC0_SDALOC_LOC16;
    i2c_base->ROUTEPEN |= I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN;
    i2c_base->FREQ = config->frequency;
    i2c_base->ADDR = (config->slave_address << 1) | 0x00; // Set slave address and enable master mode
}

void i2c_write(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t data) {
    I2C_TypeDef *i2c_base = (I2C_TypeDef *) I2C_BASE_ADDRESSES[peripheral];
    i2c_base->TXDATA = (slave_address << 1) | 0x00; // Set slave address and enable master mode
    i2c_base->CMD = I2C_CMD_START;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->TXDATA = reg_address;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->TXDATA = data;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->CMD = I2C_CMD_STOP;
}

void i2c_WriteArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length) {
    I2C_TypeDef *i2c_base = (I2C_TypeDef *) I2C_BASE_ADDRESSES[peripheral];
    i2c_base->TXDATA = (slave_address << 1) | 0x00; // Set slave address and enable master mode
    i2c_base->CMD = I2C_CMD_START;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->TXDATA = reg_address;
    while (!(i2c_base->IF & I2C_IF_ACK));
    for (uint32_t i = 0; i < length; i++) {
        i2c_base->TXDATA = data[i];
        while (!(i2c_base->IF & I2C_IF_ACK));
    }
    i2c_base->CMD = I2C_CMD_STOP;
}

uint8_t i2c_read(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address) {
    I2C_TypeDef *i2c_base = (I2C_TypeDef *) I2C_BASE_ADDRESSES[peripheral];
    i2c_base->TXDATA = (slave_address << 1) | 0x00; // Set slave address and enable master mode
    i2c_base->CMD = I2C_CMD_START;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->TXDATA = reg_address;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->CMD = I2C_CMD_START;
    i2c_base->TXDATA = (slave_address << 1) | 0x01; // Set slave address and enable read mode
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->CMD = I2C_CMD_ACK;
    while (!(i2c_base->IF & I2C_IF_RXDATAV));
    uint8_t data = i2c_base->RXDATA;
    i2c_base->CMD = I2C_CMD_NACK;
    i2c_base->CMD = I2C_CMD_STOP;
    return data;
}

void i2c_read_array(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length) {
    I2C_TypeDef *i2c_base = (I2C_TypeDef *) I2C_BASE_ADDRESSES[peripheral];
    i2c_base->TXDATA = (slave_address << 1) | 0x00; // Set slave address and enable master mode
    i2c_base->CMD = I2C_CMD_START;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->TXDATA = reg_address;
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->CMD = I2C_CMD_START;
    i2c_base->TXDATA = (slave_address << 1) | 0x01; // Set slave address and enable read mode
    while (!(i2c_base->IF & I2C_IF_ACK));
    i2c_base->CMD = I2C_CMD_ACK;
    for (uint32_t i = 0; i < length; i++) {
        while (!(i2c_base->IF & I2C_IF_RXDATAV));
        data[i] = i2c_base->RXDATA;
        if (i == length - 1) {
            i2c_base->CMD = I2C_CMD_NACK;
        } else {
            i2c_base->CMD = I2C_CMD_ACK;
        }
    }
    i2c_base->CMD = I2C_CMD_STOP;
}
