/*
 * Author hasithlk
 * Created 2023-04-10
 */
#ifdef __cplusplus
 extern "C" {
#endif

#include "i2c.h"

// =============================================
 static I2C_HandleTypeDef hi2c1;

 static void MX_I2C1_Init(void)
 {

   /* USER CODE BEGIN I2C1_Init 0 */

   /* USER CODE END I2C1_Init 0 */

   /* USER CODE BEGIN I2C1_Init 1 */

   /* USER CODE END I2C1_Init 1 */
   hi2c1.Instance = I2C1;
   hi2c1.Init.ClockSpeed = 100000;
   hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
   hi2c1.Init.OwnAddress1 = 0;
   hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
   hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
   hi2c1.Init.OwnAddress2 = 0;
   hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
   hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
   if (HAL_I2C_Init(&hi2c1) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN I2C1_Init 2 */

   /* USER CODE END I2C1_Init 2 */

 }

 HAL_StatusTypeDef I2C1_Write(const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress)
 {
	 return HAL_I2C_Master_Transmit(&hi2c1, SlaveAddress, buf, 1, HAL_MAX_DELAY);
 }

// =============================================
 /*
#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

 Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte, uint8_t SlaveAddress)
 {
    __IO uint32_t Timeout = 0;

    //    I2Cx->CR2 |= I2C_IT_ERR;  interrupts for errors

    if (!nbyte)
       return Success;

    // Wait for idle I2C interface

    Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    // Enable Acknowledgement, clear POS flag

    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

    // Intiate Start Sequence (wait for EV5

    I2C_GenerateSTART(I2Cx, ENABLE);
    Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    // Send Address

    I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);

    // EV6
    Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR));

    if (nbyte == 1) {

       // Clear Ack bit

       I2C_AcknowledgeConfig(I2Cx, DISABLE);

       // EV6_1 -- must be atomic -- Clear ADDR, generate STOP

       __disable_irq();
       (void) I2Cx->SR2;
       I2C_GenerateSTOP(I2Cx, ENABLE);
       __enable_irq();

       // Receive data   EV7

       Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
       *buf++ = I2C_ReceiveData(I2Cx);

    } else if (nbyte == 2) {
       // Set POS flag

       I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Next);

       // EV6_1 -- must be atomic and in this order

       __disable_irq();
       (void) I2Cx->SR2;                           // Clear ADDR flag
       I2C_AcknowledgeConfig(I2Cx, DISABLE);       // Clear Ack bit
       __enable_irq();

       // EV7_3  -- Wait for BTF, program stop, read data twice

       Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

       __disable_irq();
       I2C_GenerateSTOP(I2Cx, ENABLE);
       *buf++ = I2Cx->DR;
       __enable_irq();

       *buf++ = I2Cx->DR;

    } else {
       (void) I2Cx->SR2;                           // Clear ADDR flag
       while (nbyte-- != 3) {
          // EV7 -- cannot guarantee 1 transfer completion time, wait for BTF
          //        instead of RXNE

          Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
          *buf++ = I2C_ReceiveData(I2Cx);
       }

       Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));

       // EV7_2 -- Figure 1 has an error, doesn't read N-2 !

       I2C_AcknowledgeConfig(I2Cx, DISABLE);           // clear ack bit

       __disable_irq();
       *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-2
       I2C_GenerateSTOP(I2Cx, ENABLE);                  // program stop
       __enable_irq();

       *buf++ = I2C_ReceiveData(I2Cx);             // receive byte N-1

       // wait for byte N

       Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
       *buf++ = I2C_ReceiveData(I2Cx);

       nbyte = 0;
    }

    // Wait for stop

    Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
    return Success;

    errReturn:

    // Any cleanup here
    return Error;
 }


 Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbyte, uint8_t SlaveAddress)
 {
    __IO uint32_t Timeout = 0;

    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    //    I2Cx->CR2 |= I2C_IT_ERR;
/*    if (nbyte) {
       Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

       // Intiate Start Sequence

       I2C_GenerateSTART(I2Cx, ENABLE);
       Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

       // Send Address  EV5

       I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
       Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

       // EV6

       // Write first byte EV8_1

       I2C_SendData(I2Cx, *buf++);

       while (--nbyte) {

          // wait on BTF

          Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
          I2C_SendData(I2Cx, *buf++);
       }

       Timed(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF));
       I2C_GenerateSTOP(I2Cx, ENABLE);
       Timed(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
    }
    return Success;
    errReturn: return Error;
 }

 void I2C_LowLevel_Init(I2C_TypeDef* I2Cx, int ClockSpeed, int OwnAddress) {

    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    // Enable GPIOB clocks

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Configure I2C clock and GPIO

    GPIO_StructInit(&GPIO_InitStructure);

    if (I2Cx == I2C1) {

       /* I2C1 clock enable */

       //RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

       /* I2C1 SDA and SCL configuration */
/*
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
       GPIO_Init(GPIOB, &GPIO_InitStructure);

       /* I2C1 Reset */

       //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
       //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    //} else {

       /* I2C2 clock enable */

       //RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

       /* I2C1 SDA and SCL configuration */

       //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
       //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
       //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
       //GPIO_Init(GPIOB, &GPIO_InitStructure);

       /* I2C2  Reset */

       //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
       //RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

    //}

    /* Configure I2Cx                */
/*
    I2C_StructInit(&I2C_InitStructure);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    //I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = OwnAddress;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;

    I2C_Init(I2Cx, &I2C_InitStructure);
    I2C_Cmd(I2Cx, ENABLE);
 }
*/

 // LCD Use
 uint8_t i2cioSlaveAddr = 0x00;

 void I2C1_Init()
 {
    //I2C_LowLevel_Init(I2C1, 400000, 0x00);
    MX_I2C1_Init();
 }

 uint8_t i2c1begin(uint8_t slaveAddr)
 {
    i2cioSlaveAddr = slaveAddr;
    I2C1_Init();
    return 1;
 }

 void i2c1write(uint8_t data)
 {
    //I2C_Write(I2C1, &data, 1, i2cioSlaveAddr << 1);
	 I2C1_Write(&data, 1, i2cioSlaveAddr << 1);
 }
 // End LCD Use


#ifdef __cplusplus
}
#endif
