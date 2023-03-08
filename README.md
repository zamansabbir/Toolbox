# Toolbox generated for EFR32FG13, but concept can be realized with other SoC
This repo contains one of my approaches to design library in firmware. Concept can be extended to other SoC. 

I2C: the I2C driver library is dervived on em_i2c library functions to adopt to the need of a firmware application which continuously needs to use different I2C peripherals to drive different networks of sensors. This modular design can adapt to new addition of i2c perihpherals in SoC

UART: Same concecpt applied to UART driver as I2C to drive UART peripherals

Temperature Sensor driver: Created an example i2c based temperature sensor to demonstrate scalability and flexibility of adding new sensor without affecting application development. The framework provides driver api which can be configured easily for new type sensor and keep engineering hour to dedicate over application developement.

