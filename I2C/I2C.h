#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>

typedef enum {
    I2C0,
    I2C1,
    I2C2,
    I2C3
} I2C_Peripheral;

typedef enum {
    I2C_MASTER,
    I2C_SLAVE
} I2C_Mode;

typedef struct {
    I2C_Peripheral peripheral;
    uint32_t clock_frequency;
    uint8_t slave_address;
    I2C_Mode mode;
} I2C_Config;

void I2C_Init(I2C_Config *config);
void I2C_Write(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t data);
void I2C_WriteArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length);
uint8_t I2C_Read(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address);
void I2C_ReadArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length);

#endif /* _I2C_H_ */
