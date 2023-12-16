#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stm32f1xx_hal.h>

 // =======================
 static void MX_I2C1_Init(void);
 HAL_StatusTypeDef I2C1_Write(const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
 // =======================

 /*
 typedef enum {Error = 0, Success = !Error } Status;

 Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
 Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
 void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress);
*/

 // LCD Use
 void I2C1_Init();
 uint8_t i2c1begin(uint8_t slaveAddr);
 void i2c1write(uint8_t data);
 // End LCD Use


#ifdef __cplusplus
}
#endif

#endif
