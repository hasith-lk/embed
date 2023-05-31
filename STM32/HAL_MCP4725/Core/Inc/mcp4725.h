/*
 * mcp4725.h
 *
 *  Created on: May 9, 2023
 *      Author: Home Admin
 */

#ifndef INC_MCP4725_H_
#define INC_MCP4725_H_

#define MCP4725_CMD_WRITEDAC (0x40)    ///< Writes data to the DAC
#define MCP4725_CMD_WRITEDACEEPROM (0x60) //Writes data to the DAC and the EEPROM (persisting the assigned value after reset)

#include "stdbool.h"
#include "stm32f1xx.h"

typedef enum
{
	MCP4725_NORMAL       = 0x00U,
	MCP4725_INT    = 0x01U,
	MCP4725_DMA     = 0x02U
} MCP4725_Comm_TypeDef;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t SlaveAddress;
	MCP4725_Comm_TypeDef CommType;
} Mcp4725_HandleTypeDef;

HAL_StatusTypeDef Mcp4725_Init(Mcp4725_HandleTypeDef *hmcp);
HAL_StatusTypeDef Mcp4725_Set_Value(Mcp4725_HandleTypeDef *hmcp, uint16_t value);
HAL_StatusTypeDef Mcp4725_Set_Value_ROM(Mcp4725_HandleTypeDef *hmcp, uint16_t value, bool writeToROM);

#endif /* INC_MCP4725_H_ */
