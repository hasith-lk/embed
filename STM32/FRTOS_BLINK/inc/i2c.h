#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stm32f10x_i2c.h>

typedef enum {Error = 0, Success = !Error } Status;

Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress);

// LCD Use
void I2c_Init();
uint8_t i2cbegin(uint8_t slaveAddr);
void i2cwrite(uint8_t slaveAddr, uint8_t data);
// End LCD Use

#ifdef __cplusplus
}
#endif

#endif
