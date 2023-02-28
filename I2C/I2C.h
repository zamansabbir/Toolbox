#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    I2C0,
    I2C1,
    I2C2,
    I2C3
} I2C_Peripheral;

typedef struct {
    I2C_Peripheral peripheral;
    uint32_t frequency;
    uint8_t slave_address;
} I2C_Config;

void i2c_init(I2C_Config *config);

void i2c_write(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t data);
void i2c_WriteArray(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length);

uint8_t i2c_read(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address);
void i2c_read_array(I2C_Peripheral peripheral, uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint32_t length);

#endif /* I2C_H_ */
