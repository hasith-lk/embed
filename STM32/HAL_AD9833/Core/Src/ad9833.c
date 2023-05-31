/*
 * ad9833.c
 *
 *  Created on: May 12, 2023
 *      Author: Home Admin
 */

#include "ad9833.h"

// Forward declaration
void _AD9833_WriteRegister (AD9833_HandleTypeDef *hads, int16_t dat);
void _AD9833_WriteControlRegister (AD9833_HandleTypeDef *hads);


HAL_StatusTypeDef AD9833_Init(AD9833_HandleTypeDef *hads, uint32_t referenceFrequency) {

	HAL_GPIO_WritePin(hads->FNCpinport, hads->FNCpin, GPIO_PIN_SET);
	hads->refFrequency = referenceFrequency;

	// Setup some defaults
	hads->DacDisabled = false;
	hads->IntClkDisabled = false;
	hads->outputEnabled = false;
	hads->waveForm0 = SINE_WAVE;
	hads->waveForm1 = SINE_WAVE;
	hads->frequency0 = (float)1000;
	hads->frequency1 = (float)1000;		// 1 KHz sine wave to start
	hads->phase0 = (float)0.0;
	hads->phase1 = (float)0.0;				// 0 phase
	hads->activeFreq = REG0;
	hads->activePhase = REG0;

	AD9833_Reset(hads);

	return HAL_OK;
}

HAL_StatusTypeDef AD9833_ApplySignal(AD9833_HandleTypeDef *hads, WaveformType waveType, Registers freqReg,
		float frequencyInHz, Registers phaseReg, float phaseInDeg){

	// phaseReg = SAME_AS_REG0
	// phaseInDeg = 0.0

	AD9833_SetFrequency (hads, freqReg, frequencyInHz );
	AD9833_SetPhase (hads, phaseReg, phaseInDeg );
	AD9833_SetWaveform (hads, freqReg, waveType );
	AD9833_SetOutputSource (hads, freqReg, phaseReg );
}

/***********************************************************************
						Control Register
------------------------------------------------------------------------
D15,D14(MSB)	10 = FREQ1 write, 01 = FREQ0 write,
 				11 = PHASE write, 00 = control write
D13	If D15,D14 = 00, 0 = individual LSB and MSB FREQ write,
 					 1 = both LSB and MSB FREQ writes consecutively
	If D15,D14 = 11, 0 = PHASE0 write, 1 = PHASE1 write
D12	0 = writing LSB independently
 	1 = writing MSB independently
D11	0 = output FREQ0,
	1 = output FREQ1
D10	0 = output PHASE0
	1 = output PHASE1
D9	Reserved. Must be 0.
D8	0 = RESET disabled
	1 = RESET enabled
D7	0 = internal clock is enabled
	1 = internal clock is disabled
D6	0 = onboard DAC is active for sine and triangle wave output,
 	1 = put DAC to sleep for square wave output
D5	0 = output depends on D1
	1 = output is a square wave
D4	Reserved. Must be 0.
D3	0 = square wave of half frequency output
	1 = square wave output
D2	Reserved. Must be 0.
D1	If D5 = 1, D1 = 0.
	Otherwise 0 = sine output, 1 = triangle output
D0	Reserved. Must be 0.
***********************************************************************/

/*
 * Hold the AD9833 in RESET state.
 * Resets internal registers to 0, which corresponds to an output of
 * midscale - digital output at 0.
 *
 * The difference between Reset() and EnableOutput(false) is that
 * EnableOutput(false) keeps the AD9833 in the RESET state until you
 * specifically remove the RESET state using EnableOutput(true).
 * With a call to Reset(), ANY subsequent call to ANY function (other
 * than Reset itself and Set/IncrementPhase) will also remove the RESET
 * state.
 */
HAL_StatusTypeDef AD9833_Reset ( AD9833_HandleTypeDef *hads ) {
	_AD9833_WriteRegister(hads, RESET_CMD);
	HAL_Delay(15);
}

HAL_StatusTypeDef AD9833_SetFrequency (AD9833_HandleTypeDef *hads, Registers freqReg, float frequency ) {
	// TODO: calculate max frequency based on refFrequency.
	// Use the calculations for sanity checks on numbers.
	// Sanity check on frequency: Square - refFrequency / 2
	//							  Sine/Triangle - refFrequency / 4
	if ( frequency > 12.5e6 )	// TODO: Fix this based on refFreq
		frequency = 12.5e6;
	if ( frequency < 0.0 ) frequency = 0.0;

	// Save frequency for use by IncrementFrequency function
	if ( freqReg == REG0 ) hads->frequency0 = frequency;
	else hads->frequency1 = frequency;

	int32_t freqWord = (frequency * pow2_28) / (float)hads->refFrequency;
	int16_t upper14 = (int16_t)((freqWord & 0xFFFC000) >> 14),
			lower14 = (int16_t)(freqWord & 0x3FFF);

	// Which frequency register are we updating?
	uint16_t reg = freqReg == REG0 ? FREQ0_WRITE_REG : FREQ1_WRITE_REG;
	lower14 |= reg;
	upper14 |= reg;

	// I do not reset the registers during write. It seems to remove
	// 'glitching' on the outputs.
	_AD9833_WriteControlRegister(hads);
	// Control register has already been setup to accept two frequency
	// writes, one for each 14 bit part of the 28 bit frequency word
	_AD9833_WriteRegister(hads, lower14);			// Write lower 14 bits to AD9833
	_AD9833_WriteRegister(hads, upper14);			// Write upper 14 bits to AD9833
}

HAL_StatusTypeDef AD9833_IncrementFrequency (AD9833_HandleTypeDef *hads, Registers freqReg, float freqIncHz ) {
	// Add/subtract a value from the current frequency programmed in
	// freqReg by the amount given
	float frequency = (freqReg == REG0) ? hads->frequency0 : hads->frequency1;
	AD9833_SetFrequency(hads,freqReg,frequency+freqIncHz);
}

/*
 * Set the specified phase register with the phase (in degrees)
 * The output signal will be phase shifted by 2π/4096 x PHASEREG
 */
HAL_StatusTypeDef AD9833_SetPhase (AD9833_HandleTypeDef *hads, Registers phaseReg, float phaseInDeg ) {
	// Sanity checks on input
	phaseInDeg = fmod(phaseInDeg,360);
	if ( phaseInDeg < 0 ) phaseInDeg += 360;

	// Phase is in float degrees ( 0.0 - 360.0 )
	// Convert to a number 0 to 4096 where 4096 = 0 by masking
	uint16_t phaseVal = (uint16_t)(BITS_PER_DEG * phaseInDeg) & 0x0FFF;
	phaseVal |= PHASE_WRITE_CMD;

	// Save phase for use by IncrementPhase function
	if ( phaseReg == REG0 )	{
		hads->phase0 = phaseInDeg;
	}
	else {
		hads->phase1 = phaseInDeg;
		phaseVal |= PHASE1_WRITE_REG;
	}
	_AD9833_WriteRegister(hads, phaseVal);
}

/*
 * Increment the specified phase register by the phase (in degrees)
 */
HAL_StatusTypeDef AD9833_IncrementPhase (AD9833_HandleTypeDef *hads, Registers phaseReg, float phaseIncDeg ) {
	// Add/subtract a value from the current phase programmed in
	// phaseReg by the amount given
	float phase = (phaseReg == REG0) ? hads->phase0 : hads->phase1;
	AD9833_SetPhase(hads, phaseReg,phase + phaseIncDeg);
}

/*
 * Set the type of waveform that is output for a frequency register
 * SINE_WAVE, TRIANGLE_WAVE, SQUARE_WAVE, HALF_SQUARE_WAVE
 */
HAL_StatusTypeDef AD9833_SetWaveform (AD9833_HandleTypeDef *hads, Registers waveFormReg, WaveformType waveType ) {
	// TODO: Add more error checking?
	if ( waveFormReg == REG0 )
		hads->waveForm0 = waveType;
	else
		hads->waveForm1 = waveType;
	_AD9833_WriteControlRegister(hads);
}

/*
 * EnableOutput(false) keeps the AD9833 is RESET state until a call to
 * EnableOutput(true). See the Reset function description.
 */
HAL_StatusTypeDef AD9833_EnableOutput (AD9833_HandleTypeDef *hads, bool enable ) {
	hads->outputEnabled = enable;
	_AD9833_WriteControlRegister(hads);
}

/*
 * Set which frequency and phase register is being used to output the
 * waveform. If phaseReg is not supplied, it defaults to the same
 * register as freqReg.
 */
HAL_StatusTypeDef AD9833_SetOutputSource (AD9833_HandleTypeDef *hads, Registers freqReg, Registers phaseReg ) {
	// TODO: Add more error checking?
	hads->activeFreq = freqReg;
	if ( phaseReg == SAME_AS_REG0 )	hads->activePhase = hads->activeFreq;
	else hads->activePhase = phaseReg;
	_AD9833_WriteControlRegister(hads);
}

/*
 * Disable/enable both the internal clock and the DAC. Note that square
 * wave outputs are avaiable if using an external Reference.
 * TODO: ?? IS THIS TRUE ??
 */
HAL_StatusTypeDef AD9833_SleepMode (AD9833_HandleTypeDef *hads, bool enable ) {
	hads->DacDisabled = enable;
	hads->IntClkDisabled = enable;
	_AD9833_WriteControlRegister(hads);
}

/*
 * Enables / disables the DAC. It will override any previous DAC
 * setting by Waveform type, or via the SleepMode function
 */
HAL_StatusTypeDef AD9833_DisableDAC (AD9833_HandleTypeDef *hads, bool enable ) {
	hads->DacDisabled = enable;
	_AD9833_WriteControlRegister(hads);
}

/*
 * Enables / disables the internal clock. It will override any
 * previous clock setting by the SleepMode function
 */
HAL_StatusTypeDef AD9833_DisableInternalClock (AD9833_HandleTypeDef *hads, bool enable ) {
	hads->IntClkDisabled = enable;
	_AD9833_WriteControlRegister(hads);
}

void AD9833_Test(AD9833_HandleTypeDef *hads){
	uint16_t code1 = 0X2100;
	uint16_t code2 = 0x440D;//0x50C7;
	uint16_t code3 = 0x4000;
	uint16_t code4 = 0xC000;
	uint16_t code5 = 0x2000;

	_AD9833_WriteRegister (hads, code1);
	_AD9833_WriteRegister (hads, code2);
	_AD9833_WriteRegister (hads, code3);
	_AD9833_WriteRegister (hads, code4);
	_AD9833_WriteRegister (hads, code5);
}
// --------------------- PRIVATE FUNCTIONS --------------------------

/*
 * Write control register. Setup register based on defined states
 */
void _AD9833_WriteControlRegister ( AD9833_HandleTypeDef *hads ) {
	uint16_t waveForm;
	// TODO: can speed things up by keeping a writeReg0 and writeReg1
	// that presets all bits during the various setup function calls
	// rather than setting flags. Then we could just call WriteRegister
	// directly.
	if ( hads->activeFreq == REG0 ) {
		waveForm = hads->waveForm0;
		waveForm &= ~FREQ1_OUTPUT_REG;
	}
	else {
		waveForm = hads->waveForm1;
		waveForm |= FREQ1_OUTPUT_REG;
	}
	if ( hads->activePhase == REG0 )
		waveForm &= ~PHASE1_OUTPUT_REG;
	else
		waveForm |= PHASE1_OUTPUT_REG;
	if ( hads->outputEnabled )
		waveForm &= ~RESET_CMD;
	else
		waveForm |= RESET_CMD;
	if ( hads->DacDisabled )
		waveForm |= DISABLE_DAC;
	else
		waveForm &= ~DISABLE_DAC;
	if ( hads->IntClkDisabled )
		waveForm |= DISABLE_INT_CLK;
	else
		waveForm &= ~DISABLE_INT_CLK;

	_AD9833_WriteRegister (hads, waveForm );
}

void _AD9833_WriteRegister (AD9833_HandleTypeDef *hads, int16_t dat) {
	/*
	 * We set the mode here, because other hardware may be doing SPI also
	 */
	//SPI.setDataMode(SPI_MODE2);

	/* Improve overall switching speed
	 * Note, the times are for this function call, not the write.
	 * digitalWrite(FNCpin)			~ 17.6 usec
	 * digitalWriteFast2(FNC_PIN)	~  8.8 usec
	 */
	HAL_GPIO_WritePin(hads->FNCpinport, hads->FNCpin, GPIO_PIN_RESET);		// FNCpin low to write to AD9833

	//delayMicroseconds(2);	// Some delay may be needed

	// TODO: Are we running at the highest clock rate?
	//SPI.transfer(highByte(dat));	// Transmit 16 bits 8 bits at a time
	//SPI.transfer(lowByte(dat));
	uint8_t pData[2];
	pData[0] = dat >> 8 & 0xFF; // MSB
	pData[1] = dat >> 0 & 0xFF; // LSB

	HAL_SPI_Transmit(hads->hspi, pData, 2, HAL_MAX_DELAY);
	//HAL_SPI_Transmit_IT(hads->hspi, pData, 2);

	HAL_GPIO_WritePin(hads->FNCpinport, hads->FNCpin, GPIO_PIN_SET);		// Write done
}
