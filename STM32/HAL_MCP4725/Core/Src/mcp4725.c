/*
 * mcp4725.c
 *
 *  Created on: May 9, 2023
 *      Author: Home Admin
 */

#include "mcp4725.h"

HAL_StatusTypeDef Mcp4725_Init(Mcp4725_HandleTypeDef *hmcp) {
	return HAL_I2C_IsDeviceReady((hmcp->hi2c), (uint16_t) (hmcp->SlaveAddress << 1), 3,
			HAL_MAX_DELAY);
}

HAL_StatusTypeDef Mcp4725_Set_Value(Mcp4725_HandleTypeDef *hmcp, uint16_t value) {
	return Mcp4725_Set_Value_ROM(hmcp, value, false);
}

HAL_StatusTypeDef Mcp4725_Set_Value_ROM(Mcp4725_HandleTypeDef *hmcp, uint16_t value,
		bool writeToROM) {

	uint8_t packet[3];

	if (writeToROM) {
		packet[0] = MCP4725_CMD_WRITEDACEEPROM;
	} else {
		packet[0] = MCP4725_CMD_WRITEDAC;
	}
	packet[1] = value / 16; // Upper data bits (D11.D10.D9.D8.D7.D6.D5.D4)
	packet[2] = (value % 16) << 4; // Lower data bits (D3.D2.D1.D0.x.x.x.x)

	int returnVal;
	if (hmcp->CommType == MCP4725_NORMAL)
		returnVal = HAL_I2C_Master_Transmit(hmcp->hi2c, (uint16_t) (hmcp->SlaveAddress << 1), packet, 3, HAL_MAX_DELAY);
	else if (hmcp->CommType == MCP4725_INT)
		returnVal = HAL_I2C_Master_Transmit_IT(hmcp->hi2c, (uint16_t) (hmcp->SlaveAddress << 1), packet, 3);
	else if (hmcp->CommType == MCP4725_DMA)
			returnVal = HAL_I2C_Master_Transmit_DMA(hmcp->hi2c, (uint16_t) (hmcp->SlaveAddress << 1), packet, 3);
	return returnVal;
}
