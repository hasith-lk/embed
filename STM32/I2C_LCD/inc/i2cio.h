/*
 * i2cio.h
 *
 *  Created on: Jul 29, 2016
 *      Author: Admin
 */

#ifndef I2CIO_H_
#define I2CIO_H_

#include "i2c.h"

void I2c_Init();
uint8_t i2cbegin(uint8_t slaveAddr);
void i2cwrite(uint8_t data);

#endif /* I2CIO_H_ */
