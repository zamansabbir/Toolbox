#include "temperature_sensor.h"
#include "TMP102.h"
#include "I2C.h"

#define TMP102_SLAVE_ADDRESS 0x48

#define TMP102_REG_TEMPERATURE 0x00
#define TMP102_REG_CONFIGURATION 0x01
#define TMP102_REG_T_LOW 0x02
#define TMP102_REG_T_HIGH 0x03
#define TMP102_REG_T_CRIT 0x04
#define TMP102_REG_MANUFACTURER_ID 0xFE
#define TMP102_REG_DEVICE_ID 0xFF

#define TMP102_MSB_MASK 0x7F

#define TMP102_ALERT_PIN_POLARITY_MASK 0x2
#define TMP102_ALERT_PIN_MODE_MASK 0x1C
#define TMP102_CONVERSION_RATE_MASK 0x60
#define TMP102_CELSIUS_FLAG_MASK 0x1

#define SIGN_BIT_LOCATION       0x0800

static uint16_t temperature_to_register_format(float temperature);

static TMP102_Config _g_tmp102_config = {.high_thresh = 0,
                                 .low_thresh = 0, 
                                 .alert_polarity = 0,
                                 .alert_mode  = 0,
                                 .conversion_rate = 0,
                                 .celsius = 0}; // configure it based on your need, setting it to default

void tmp102_init(Temperature_Sensor_Config *config) {

    uint8_t configuration = (_g_tmp102_config.alert_polarity << 3) |
                            (_g_tmp102_config.alert_mode << 2) |
                            (_g_tmp102_config.conversion_rate << 6) |
                            (_g_tmp102_config.celsius ? 0 : 1);
    uint8_t config_data[2] = {TMP102_REG_CONFIGURATION, configuration};
    i2c_WriteArray(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_CONFIGURATION, config_data, 2);

    config->mfg_id = tmp102_get_manufacturer_id();
    config->device_id = tmp102_get_device_id();
    config->type = TMP102;
}

void tmp102_read_temperature(Temperature_Sensor_Data *data) {
    uint8_t temperature_data[2];
    i2c_read_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_TEMPERATURE, temperature_data, 2);
    int16_t signed_temperature = ((int16_t) temperature_data[0] << 8) | temperature_data[1];
    if (signed_temperature & SIGN_BIT_LOCATION) {
        signed_temperature |= 0xF800;
    }

    float celsius = signed_temperature * 0.0625;

    if (!_g_tmp102_config.celsius) {
        float fahrenheit = (celsius * 1.8) + 32;
        data->temperature = fahrenheit;
    } else {
        data->temperature = celsius;
    }
}

bool tmp102_read_configuration_register(uint8_t *data) {
    bool status = false;
    *data = i2c_read(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_CONFIGURATION);

    if (*data !=0xFF) {
        status = true;
    }
    return status;
}

void tmp102_get_high_threshold(Temperature_Sensor_Data *data) {
    uint8_t temperature_data[2];
    i2c_read_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_T_HIGH, data, 2);
    int16_t signed_temperature = ((int16_t) temperature_data[0] << 8) | temperature_data[1];
    if (signed_temperature & SIGN_BIT_LOCATION) {
        signed_temperature |= 0xF800;
    }

    float celsius = signed_temperature * 0.0625;

    if (!_g_tmp102_config.celsius) {
        float fahrenheit = (celsius * 1.8) + 32;
        data->temperature = fahrenheit;
    } else {
        data->temperature = celsius;
    }
}

void tmp102_set_high_threshold(Temperature_Sensor_Data data) {
    // Convert the temperature value from Celsius to register format
    uint16_t threshold_reg_value = temperature_to_register_format(data.temperature);

    // Extract the high and low bytes of the register value
    uint8_t high_byte = (uint8_t)((threshold_reg_value >> 8) & 0xFF);
    uint8_t low_byte = (uint8_t)(threshold_reg_value & 0xFF);

    // Write the threshold value to the high threshold register
    uint8_t reg_value[2] = {high_byte, low_byte};
    i2c_write_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_T_HIGH, reg_value, 2);

    // Store the threshold value in the configuration
    _g_tmp102_config.high_thresh = threshold_reg_value;
}


void tmp102_get_low_threshold(Temperature_Sensor_Data *data) {
    uint8_t temperature_data[2];
    i2c_read_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_T_LOW, data, 2);

    int16_t signed_temperature = ((int16_t) temperature_data[0] << 8) | temperature_data[1];
    if (signed_temperature & SIGN_BIT_LOCATION) {
        signed_temperature |= 0xF800;
    }

    float celsius = signed_temperature * 0.0625;

    if (!_g_tmp102_config.celsius) {
        float fahrenheit = (celsius * 1.8) + 32;
        data->temperature = fahrenheit;
    } else {
        data->temperature = celsius;
    }
}

void tmp102_set_low_threshold(uint8_t value) {
    uint16_t threshold_reg_value = temperature_to_register_format(data.temperature);
    uint8_t high_byte = (uint8_t)((threshold_reg_value >> 8) & 0xFF);
    uint8_t low_byte = (uint8_t)(threshold_reg_value & 0xFF);

    // Write the threshold value to the high threshold register
    uint8_t reg_value[2] = {high_byte, low_byte};
    i2c_WriteArray(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_T_LOW, reg_value, 2);
    _g_tmp102_config.low_thresh = value;
}

static uint16_t temperature_to_register_format(float temperature) {
    float resolution = 0.0625;
    uint16_t result;

    if (!_g_tmp102_config.celsius) {
        if (temperature < 0) {
            result = ~((uint16_t)(-temperature / resolution) - 1) & 0xFFF;
        } else {
            result = (uint16_t)(temperature / resolution) & 0xFFF;
        }
    } else {  // Fahrenheit
        temperature = (temperature - 32) * 5 / 9;  // Convert to Celsius
        if (temperature < 0) {
            result = ~((uint16_t)(-temperature / resolution) - 1) & 0xFFF;
        } else {
            result = (uint16_t)(temperature / resolution) & 0xFFF;
        }
    }

    return result;
}

uint16_t tmp102_get_manufacturer_id(void) {
    uint8_t data[2] = {0};
    i2c_read_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_MANUFACTURER_ID, data, 2);
    uint16_t mfg_id =  ((uint16_t) data[0] << 8) | data[1];
    return mfg_id;
}

uint16_t tmp102_get_device_id(void) {
    uint8_t data[2] = {0};
    i2c_read_array(I2C0, TMP102_SLAVE_ADDRESS, TMP102_REG_DEVICE_ID, data, 2);
    uint16_t mfg_id =  ((uint16_t) data[0] << 8) | data[1];
    return data;
}