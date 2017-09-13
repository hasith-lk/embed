#include <inttypes.h>
#include "i2cio.h"

#ifdef __cplusplus
 extern "C" {
#endif

 uint8_t i2cioSlaveAddr = 0x00;

 void I2c_Init()
 {
    I2C_LowLevel_Init(I2C1, 100000, 0x00);
 }

 uint8_t i2cbegin(uint8_t slaveAddr)
 {
    i2cioSlaveAddr = slaveAddr;
    I2c_Init();
    return 1;
 }

 void i2cwrite(uint8_t data)
 {
    I2C_Write(I2C1, &data, 1, i2cioSlaveAddr << 1);
 }

#ifdef __cplusplus
}
#endif
