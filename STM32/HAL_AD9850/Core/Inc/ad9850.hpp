/******************************************************************************************************************
 * Arduino library for AD9850
 * Created 23/08/2014
 * Christophe Caiveau f4goj@free.fr
 *
 * Use this library freely
 *
 * Hardware connections :
 * W_CLK   -> any pin
 * FQ_UD   -> any pin
 * DATA/D7 -> any pin
 * RESET   -> any pin
 *
 * Functions :
 * dds.begin(W_CLK pin, FQ_UD pin, DATA pin, RESET pin); Initialize the output pins and master reset the AD9850
 * dds.calibrate(frequency); Compensation of crystal oscillator frequency
 * dds.setfreq(frequency,phase); frequency in Hz, phase coded on 5 bits
 * dds.down(); power down mode reducing the dissipated power from 380mW to 30mW @5V
 * dds.up(); wake-up the AD9850
 *
 * AD9850 datasheet at http://www.analog.com/static/imported-files/data_sheets/AD9850.pdf
 *
 *****************************************************************************************************************/

#include "stm32f1xx_hal.h"

#ifndef AD9850_H
#define AD9850_H

class AD9850{
	public:
  	AD9850();

	void begin(GPIO_TypeDef  *GPIOx, int w_clk, int fq_ud, int data, int reset);
	void setfreq(double f, uint8_t p);
	void down();
	void up();
	void calibrate(double TrimFreq);
	void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

	private:
	int W_CLK;
	int FQ_UD;
	int DATA;
	int RESET;
	uint32_t deltaphase;
	uint8_t phase;
	void update();
	void begin_priv();
	void pulse(int pin);
	double calibFreq;
	GPIO_TypeDef  *GPIOx;
	void digitalWrite(int pin, GPIO_PinState PinState);
	const uint8_t LSBFIRST = 0;
	const uint8_t MSBFIRST = 1;
};

extern AD9850 DDS;

#endif
