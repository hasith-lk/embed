/*
 * ad9833.h
 *
 *  Created on: May 12, 2023
 *      Author: Home Admin
 */

#ifndef AD9833_H_
#define AD9833_H_

#include "stdbool.h"
#include "stm32f1xx.h"

/*
#ifdef FNC_PIN
	// Use digitalWriteFast for a speedup
	#include "digitalWriteFast.h"
	#define WRITE_FNCPIN(Val) digitalWriteFast2(FNC_PIN,(Val))
#else  // otherwise, just use digitalWrite
	#define WRITE_FNCPIN(Val) digitalWrite(FNCpin,(Val))
#endif
*/

#define pow2_28				268435456L	// 2^28 used in frequency word calculation
#define BITS_PER_DEG		11.3777777777778	// 4096 / 360

#define RESET_CMD			0x0100		// Reset enabled (also CMD RESET)
/*		Sleep mode
 * D7	1 = internal clock is disabled
 * D6	1 = put DAC to sleep
 */
#define SLEEP_MODE			0x00C0		// Both DAC and Internal Clock
#define DISABLE_DAC			0x0040
#define	DISABLE_INT_CLK		0x0080

#define PHASE_WRITE_CMD		0xC000		// Setup for Phase write
#define PHASE1_WRITE_REG	0x2000		// Which phase register
#define FREQ0_WRITE_REG		0x4000		//
#define FREQ1_WRITE_REG		0x8000
#define PHASE1_OUTPUT_REG	0x0400		// Output is based off REG0/REG1
#define FREQ1_OUTPUT_REG	0x0800		// ditto

typedef enum { SINE_WAVE = 0x2000, TRIANGLE_WAVE = 0x2002,
			   SQUARE_WAVE = 0x2028, HALF_SQUARE_WAVE = 0x2020 } WaveformType;

typedef enum { REG0, REG1, SAME_AS_REG0 } Registers;


typedef struct {
	SPI_HandleTypeDef *hspi;
	WaveformType waveForm0, waveForm1;
	GPIO_TypeDef  *FNCpinport;
	uint16_t		 FNCpin;
	uint8_t		 outputEnabled, DacDisabled, IntClkDisabled;
	uint32_t	 refFrequency;
	float		 frequency0, frequency1, phase0, phase1;
	Registers	 activeFreq, activePhase;
} AD9833_HandleTypeDef;

HAL_StatusTypeDef AD9833_Init(AD9833_HandleTypeDef *hads, uint32_t referenceFrequency);
HAL_StatusTypeDef AD9833_ApplySignal(AD9833_HandleTypeDef *hads, WaveformType waveType, Registers freqReg, float frequencyInHz, Registers phaseReg, float phaseInDeg);
HAL_StatusTypeDef AD9833_Reset(AD9833_HandleTypeDef *hads);
HAL_StatusTypeDef AD9833_SetFrequency(AD9833_HandleTypeDef *hads, Registers freqReg, float frequency );
HAL_StatusTypeDef AD9833_IncrementFrequency(AD9833_HandleTypeDef *hads, Registers freqReg, float freqIncHz );
HAL_StatusTypeDef AD9833_SetPhase(AD9833_HandleTypeDef *hads, Registers phaseReg, float phaseInDeg);
HAL_StatusTypeDef AD9833_IncrementPhase(AD9833_HandleTypeDef *hads, Registers phaseReg, float phaseIncDeg );
HAL_StatusTypeDef AD9833_SetWaveform(AD9833_HandleTypeDef *hads, Registers waveFormReg, WaveformType waveType );
HAL_StatusTypeDef AD9833_SetOutputSource(AD9833_HandleTypeDef *hads, Registers freqReg, Registers phaseReg);
HAL_StatusTypeDef AD9833_EnableOutput(AD9833_HandleTypeDef *hads, bool enable );
HAL_StatusTypeDef AD9833_SleepMode(AD9833_HandleTypeDef *hads, bool enable );
HAL_StatusTypeDef AD9833_DisableDAC(AD9833_HandleTypeDef *hads, bool enable );
HAL_StatusTypeDef AD9833_DisableInternalClockC(AD9833_HandleTypeDef *hads, bool enable );

void AD9833_Test(AD9833_HandleTypeDef *hads);
#endif /* AD9833_H_ */
