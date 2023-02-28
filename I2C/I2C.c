#include "I2C.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"

static const uint32_t I2C_BASE_ADDRESSES[] = {
    I2C0_BASE,
    I2C1_BASE,
    I2C2_BASE,
    I2C3_BASE
};

void I2C_Init(I2C_Config *config) {
    uint32_t i2c_base = I2C_BASE_ADDRESSES[config->peripheral];

    I2CMasterInitExpClk(i2c_base, SysCtlClockGet(), config->mode == I2C_MASTER);
    I2CMasterEnable(i2c_base);
    I2CMasterSlaveAddrSet(i2c_base, config->slave_address, config->mode == I2C_MASTER);
    I2CMasterTimeoutSet(i2c_base, SysCtlClockGet() / 100); // Set timeout to 10ms
}

void I2C_Write(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t data) {
    uint32_t i2c_base = I2C_BASE_ADDRESSES[peripheral];;

    I2CMasterSlaveAddrSet(i2c_base, slave_address, false);
    I2CMasterDataPut(i2c_base, reg_address);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(i2c_base));
    I2CMasterDataPut(i2c_base, data);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(i2c_base));
}

void I2C_WriteArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length) {
    uint32_t i2c_base = I2C_BASE_ADDRESSES[peripheral];

    I2CMasterSlaveAddrSet(i2c_base, slave_address, false);
    I2CMasterDataPut(i2c_base, reg_address);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(i2c_base));
    for (uint32_t i = 0; i < length; i++) {
        I2CMasterDataPut(i2c_base, data[i]);
        I2CMasterControl(i2c_base, i == length - 1 ? I2C_MASTER_CMD_BURST_SEND_FINISH : I2C_MASTER_CMD_BURST_SEND_CONT);
        while (I2CMasterBusy(i2c_base));
    }
}

uint8_t I2C_Read(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address) {
    uint32_t i2c_base = I2C_BASE_ADDRESSES[peripheral];

    I2CMasterSlaveAddrSet(i2c_base, slave_address, false);
    I2CMasterDataPut(i2c_base, reg_address);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_SEND);
    while (I2CMasterBusy(i2c_base));
    I2CMasterSlaveAddrSet(i2c_base, slave_address, true);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while (I2CMasterBusy(i2c_base));
    return I2CMasterDataGet(i2c_base);
}

void I2C_ReadArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length) {
    uint32_t i2c_base = I2C_BASE_ADDRESSES[peripheral];

    I2CMasterSlaveAddrSet(i2c_base, slave_address, false);
    I2CMasterDataPut(i2c_base, reg_address);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_SEND_START);
    while (I2CMasterBusy(i2c_base));
    I2CMasterSlaveAddrSet(i2c_base, slave_address, true);
    I2CMasterControl(i2c_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while (I2CMasterBusy(i2c_base));
    for (uint32_t i = 0; i < length; i++) {
        data[i] = I2CMasterDataGet(i2c_base);
        I2CMasterControl(i2c_base, i == length - 1 ? I2C_MASTER_CMD_BURST_RECEIVE_FINISH : I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while (I2CMasterBusy(i2c_base));
    }
}

