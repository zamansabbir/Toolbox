#include "temperature_sensor.h"
#include "TMP102.h"
#include "new_temperature_sensor.h" // add new temperature driver

Temperature_Sensor_Driver *temperature_sensor_get_driver(Temperature_Sensor_Type type) {
    static Temperature_Sensor_Driver driver = {0};

    switch (type) {
        case TMP102:
            driver.init = tmp102_init;
            driver.read_temperature = tmp102_read_temperature;
            driver.get_manufacturer_id = tmp102_get_manufacturer_id;
            driver.read_configuration_register = tmp102_read_configuration_register;
            driver.get_device_id = tmp102_get_device_id;
            break;
        case NEW_TEMPERATURE_SENSOR:
            // Add implementation for new temperature sensor here
            break;
        default:
            // Invalid type, return NULL
            return NULL;
    }

    return &driver;
}
