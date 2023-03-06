#ifndef TMP102_H_
#define TMP102_H_

#include "temperature_sensor.h"

#define TMP102_ALERT_PIN_POLARITY_LOW 0
#define TMP102_ALERT_PIN_POLARITY_HIGH 1
#define TMP102_ALERT_PIN_MODE_COMPARATOR 0
#define TMP102_ALERT_PIN_MODE_INTERRUPT 1
#define TMP102_CONVERSION_RATE_0_25_HZ 0
#define TMP102_CONVERSION_RATE_1_HZ 1
#define TMP102_CONVERSION_RATE_4_HZ 2
#define TMP102_CONVERSION_RATE_8_HZ 3
#define TMP102_DEFAULT_ALERT_POLARITY TMP102_ALERT_PIN_POLARITY_LOW
#define TMP102_DEFAULT_ALERT_MODE TMP102_ALERT_PIN_MODE_COMPARATOR
#define TMP102_DEFAULT_CONVERSION_RATE TMP102_CONVERSION_RATE_4_HZ
#define TMP102_DEFAULT_CELSIUS true

typedef struct {
    uint8_t high_thresh;
    uint8_t low_thresh;
    uint8_t alert_polarity;
    uint8_t alert_mode;
    uint8_t conversion_rate;
    bool celsius;
} TMP102_Config;

void tmp102_init(Temperature_Sensor_Config *config);

void tmp102_read_temperature(Temperature_Sensor_Data *data);

bool tmp102_read_configuration_register(uint8_t *data);

void tmp102_get_high_threshold(Temperature_Sensor_Data *data);

void tmp102_set_high_threshold(Temperature_Sensor_Data data);

void tmp102_get_low_threshold(Temperature_Sensor_Data *data);

void tmp102_set_low_threshold(uint8_t value);

uint16_t tmp102_get_manufacturer_id(void);

uint16_t tmp102_get_device_id(void);

#endif /* TMP102_H_ */
