#ifndef I2C_H
#define I2C_H

#include "stm8s_conf.h"

// uint8_t I2C_setup(void);
// void I2C_write(uint8_t slaveAddr, uint8_t data);

#define I2C_PORT                                     GPIOB
#define SDA_pin                                      GPIO_PIN_5
#define SCL_pin                                      GPIO_PIN_4
#define PCF8574_address                               0x27
void I2C_GPIO_setup(void);
void I2C_setup(void);
void PCF8574_init(void);
unsigned char PCF8574_read(void);
void PCF8574_write(unsigned char data_byte);

#endif
