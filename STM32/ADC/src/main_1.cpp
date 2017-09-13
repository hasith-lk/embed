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
//#include "usart.h"
#include <stdio.h>
#include <stdint.h>

#include "delay.h"
#include "LiquidCrystal_I2C.h"

#ifdef DEBUG
extern "C" void initialise_monitor_handles();
#endif

uint16_t       AD_value;
const uint16_t V25 = 1750;// when V25=1.41V at ref 3.3V
const uint16_t Avg_Slope = 5; //when avg_slope=4.3mV/C at ref 3.3V
uint16_t       TemperatureC;

#define INPUTS 2
#define ARRAYSIZE INPUTS*4
#define ADC1_DR    ((uint32_t)0x4001244C)
volatile uint16_t ADC_values[ARRAYSIZE];
volatile uint32_t status = 0;

uint16_t ReadInternalTemp() {
   //enable ADC1 clock
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   ADC_InitTypeDef ADC_InitStructure;
   //ADC1 configuration
   //select independent conversion mode (single)
   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
   //We will convert single channel only
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
   //we will convert one time
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   //select no external triggering
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
   //right 12-bit data alignment in ADC data register
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   //single channel conversion
   ADC_InitStructure.ADC_NbrOfChannel = 1;
   //load structure values to control and status registers
   ADC_Init(ADC1, &ADC_InitStructure);
   //wake up temperature sensor
   ADC_TempSensorVrefintCmd(ENABLE);
   //ADC1 channel16 configuration
   //we select 41.5 cycles conversion for channel16
   //and rank=1 which doesn't matter in single mode
   ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_41Cycles5);
   //Enable ADC1
   ADC_Cmd(ADC1, ENABLE);
   //Enable ADC1 reset calibration register
   ADC_ResetCalibration(ADC1);
   //Check the end of ADC1 reset calibration register
   while (ADC_GetResetCalibrationStatus(ADC1))
      ;
   //Start ADC1 calibration
   ADC_StartCalibration(ADC1);
   //Check the end of ADC1 calibration
   while (ADC_GetCalibrationStatus(ADC1))
      ;
   //Start ADC1 Software Conversion
   ADC_SoftwareStartConvCmd(ADC1, ENABLE);
   //wait for conversion complete
   while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
   }
   //read ADC value
   AD_value = ADC_GetConversionValue(ADC1);
   //clear EOC flag
   ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
   return AD_value;
}

void InternalTempReadMain() {
   //enable ADC1 clock
   uint16_t AD_value = ReadInternalTemp();
   printf("\r\n ADC value: %d \r\n", AD_value);
   TemperatureC = (uint16_t) (((V25 - AD_value) / Avg_Slope + 25));
   printf("Temperature: %d%cC\r\n", TemperatureC, 176);
   I2c_Init();
   DelayInit();
   LiquidCrystal_I2C lcd1602(0x27, 3, POSITIVE);
   lcd1602.begin(16, 2);
   lcd1602.clear();
   uint32_t counter = 0;
   while (1) {
      counter++;
      //Start ADC1 Software Conversion
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);
      //wait for conversion complete
      while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
      }
      //read ADC value
      AD_value = ADC_GetConversionValue(ADC1);
      //clear EOC flag
      ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
      TemperatureC = (uint16_t) ((V25 - AD_value) / Avg_Slope + 25);

      char printBuf[20];
      sprintf(printBuf, "%d : %d", counter, TemperatureC);
      lcd1602.clear();
      lcd1602.print(printBuf);
      delay_ms(1500);
   }
}

void ADC_Init(void);
void DMA_Init(void);
void Adctest()
{

}

int main(void)
{
#ifdef DEBUG
   initialise_monitor_handles();
#else
   //Stdinout_Init();
#endif

   //InternalTempReadMain();
   uint8_t index;
   //initialize USART1
   //Usart1Init();
   ADC_Init();
   DMA_Init();
   DelayInit();

  while (1)
  {
      //Enable DMA1 Channel transfer
      DMA_Cmd(DMA1_Channel1, ENABLE);
      //Start ADC1 Software Conversion
      ADC_SoftwareStartConvCmd(ADC1, ENABLE);
      //wait for DMA complete
      while (!status){};
      ADC_SoftwareStartConvCmd(ADC1, DISABLE);
      //print averages
      /*for(index = 0; index<8; index++)
          {
          printf("ch%d = %d ",index, ADC_values[index]);
          }*/

      for(index = 0; index<INPUTS; index++)
      {
          printf("\r\n ADC value on ch%d = %d",
                  index, (uint16_t)((ADC_values[index]+ADC_values[index+(INPUTS)]
                          +ADC_values[index+(INPUTS*2)]+ADC_values[index+(INPUTS*3)])/4));
      }
      delay_ms(1500);
       //interrupts does the job
     }
}

void ADC_Init(void){
    //--Enable ADC1 and GPIOA--
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
    //==Configure ADC pins (PA0 -> Channel 0 to PA7 -> Channel 7) as analog inputs==
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1| GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    //ADC1 configuration

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//!
    //select no external triggering
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //8 channels conversion
    ADC_InitStructure.ADC_NbrOfChannel = INPUTS;
    //load structure values to control and status registers
    ADC_Init(ADC1, &ADC_InitStructure);
    //wake up temperature sensor
    //ADC_TempSensorVrefintCmd(ENABLE);
    //configure each channel
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_41Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_41Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_41Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_41Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_41Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_41Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 7, ADC_SampleTime_41Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_41Cycles5);
    //Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    //enable DMA for ADC
    ADC_DMACmd(ADC1, ENABLE);
    //Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1);
    //Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));
    //Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    //Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
}

void DMA_Init(void){
    //enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //create DMA structure
    DMA_InitTypeDef  DMA_InitStructure;
    //reset DMA1 channe1 to default values;
    DMA_DeInit(DMA1_Channel1);
    //channel will be used for memory to memory transfer
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //setting normal mode (non circular)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //medium priority
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //source and destination data size word=32bit
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //automatic memory destination increment enable.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //source address increment disable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //chunk of data to be transfered
    DMA_InitStructure.DMA_BufferSize = ARRAYSIZE;
    //source and destination start addresses
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_values;
    //send values to DMA registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Enable DMA1 Channel Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1
    NVIC_InitTypeDef NVIC_InitStructure;
    //Enable DMA1 channel IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


}
