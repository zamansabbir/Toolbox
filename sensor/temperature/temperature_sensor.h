#ifndef TEMPERATURE_SENSOR_H_
#define TEMPERATURE_SENSOR_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TMP102,
    NEW_TEMPERATURE_SENSOR,
    NUMBER_OF_TEMPERATURE_SENSOR
} Temperature_Sensor_Type;

typedef struct {
    Temperature_Sensor_Type type;
    uint8_t address;
    uint16_t mfg_id;
    uint16_t device_id;
} Temperature_Sensor_Config;

typedef struct {
    float temperature;
} Temperature_Sensor_Data;

typedef struct {
    void (*init)(Temperature_Sensor_Config *config);
    void (*read_temperature)(Temperature_Sensor_Data *data);
    bool (*read_configuration_register)(uint8_t *data);
    uint16_t (*get_manufacturer_id)(void);
    uint16_t (*get_device_id)(void);

} Temperature_Sensor_Driver;

Temperature_Sensor_Driver *temperature_sensor_get_driver(Temperature_Sensor_Type type);

#endif /* TEMPERATURE_SENSOR_H_ */
