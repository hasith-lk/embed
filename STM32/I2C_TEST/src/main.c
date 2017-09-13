/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include <stdint.h>
#include "i2c.h"

#define I2C_PIN_SDA			GPIO_Pin_6
#define I2C_PIN_SCL			GPIO_Pin_7

#define SLAVE_ADDRESS		0x08

uint8_t receivedByte;

void main ()
{
#ifdef DEBUG
	initialise_monitor_handles();
#endif

	printf("Start \n\r");
	I2C_InitTypeDef I2C_InitStruct;
	I2C_StructInit(&I2C_InitStruct);

	I2C_LowLevel_Init(I2C1, 100000, 0x00);
	printf("Init Done \n\r");

	printf("%d \n\r",10);

	uint32_t nbuf = 1;

	// RTC address 0x57, 0x68;
	// LCD Display 0x27

	uint8_t SlaveAddress1 = 0x27;
	uint8_t SlaveAddress2 = 0x27;

	//for (uint8_t slave_Addr = 10; slave_Addr < 127 ; slave_Addr++)
	{
		if (I2C_Read(I2C1, &receivedByte, nbuf, SlaveAddress1 << 1) == Success)
		{
			printf("Read Done \n\r");
			printf("%x \n\r", receivedByte);
		}
		else
			printf("Read Failed \n\r");
	}

	{
		if (I2C_Read(I2C1, &receivedByte, nbuf, SlaveAddress2 << 1) == Success)
		{
			printf("Read Done \n\r");
			printf("%x \n\r", receivedByte);
		}
		else
			printf("Read Failed \n\r");
		}
}

/*
void i2c_init()
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

    I2C_StructInit(&I2C_InitStruct);
    I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C1, &I2C_InitStruct);
    I2C_Cmd(I2C1, ENABLE);

}

void i2c_start()
{
    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    // Generate start condition
    I2C_GenerateSTART(I2C1, ENABLE);

    // Wait for I2C EV5.
    // It means that the start condition has been correctly released
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

void i2c_stop()
{
    // Generate I2C stop condition
    I2C_GenerateSTOP(I2C2, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));
}

void i2c_address_direction(uint8_t address, uint8_t direction)
{
    // Send slave address
    I2C_Send7bitAddress(I2C2, address, direction);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2C2,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if (direction == I2C_Direction_Receiver)
    {
        while (!I2C_CheckEvent(I2C2,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

void i2c_transmit(uint8_t byte)
{
    // Send data byte
    I2C_SendData(I2C2, byte);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and
    // output on the bus)
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

uint8_t i2c_receive_ack()
{
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C2);
}

uint8_t i2c_receive_nack()
{
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C2);
}

void i2c_write(uint8_t address, uint8_t data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Transmitter);
    i2c_transmit(data);
    i2c_stop();
}

void i2c_read(uint8_t address, uint8_t* data)
{
    i2c_start();
    i2c_address_direction(address << 1, I2C_Direction_Receiver);
    *data = i2c_receive_nack();
    i2c_stop();
}

void loopDelay(uint32_t nTime)
{
	uint32_t c;
	for (c = nTime; c > 0; c--)
	{
		asm volatile("nop");
	}
}


int main(void)
{
#ifdef DEBUG
	initialise_monitor_handles();
#endif

	loopDelay(100);
    //DelayInit();
    //lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);

    // Initialize I2C
    i2c_init();
    printf("Start \n\r");
    uint8_t slave_address = 0x68; //RTC address
    printf("Test connection");
    i2c_read(slave_address, &receivedByte);
    printf("Test connection Done");

    while (slave_address < 123)
    {
    	slave_address++;

    	printf("Test Loop \n\r");
    	printf("0x%08X \n\r", slave_address);
    	i2c_read(slave_address, &receivedByte);
    	if (receivedByte == 1)
		{
    		printf("Done \n\r");
		}
    	//DelayMs(5);
    	loopDelay(500);

        // Write 0x01 to slave (turn on LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x01);
        //DelayMs(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status
        //lcd16x2_clrscr();
        if (receivedByte == 0)
        {
            //lcd16x2_puts("LED Blinking Off");
        }
        else if (receivedByte == 1)
        {
            //lcd16x2_puts("LED Blinking On");
        }
        //DelayMs(2500);

        // Write 0x00 to slave (turn off LED blinking)
        i2c_write(SLAVE_ADDRESS, 0x00);
        //DelayMs(5);
        // Read LED blinking status (off/on)
        i2c_read(SLAVE_ADDRESS, &receivedByte);
        // Display LED blinking status
        //lcd16x2_clrscr();
        if (receivedByte == 0)
        {
            //lcd16x2_puts("LED Blinking Off");
        }
        else if (receivedByte == 1)
        {
            //lcd16x2_puts("LED Blinking On");
        }
        //DelayMs(2500);
    }
}
*/
