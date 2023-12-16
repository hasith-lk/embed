
/**
  ********************************************************************************
  * @file    stm8s_tim5_GenerateEvent.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file contains all the functions for the UART1 peripheral.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s_tim5.h"
#include "private/stm8s_tim5_private.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/** @}
  * @addtogroup TIM5_Public_Functions
  * @{
  */

/**
  * @brief  Configures the TIM5 event to be generated by software.
  * @param   TIM5_EventSource specifies the event source.
  * This parameter can be one of the following values:
  *                       - TIM5_EVENTSOURCE_UPDATE: TIM5 update Event source
  *                       - TIM5_EVENTSOURCE_CC1: TIM5 Capture Compare 1 Event source
  *                       - TIM5_EVENTSOURCE_CC2: TIM5 Capture Compare 2 Event source
  *                       - TIM5_EVENTSOURCE_CC3: TIM5 Capture Compare 3 Event source
  * @retval None
  */
void TIM5_GenerateEvent(TIM5_EventSource_TypeDef TIM5_EventSource)
{
  /* Check the parameters */
  assert_param(IS_TIM5_EVENT_SOURCE_OK(TIM5_EventSource));
  
  /* Set the event sources */
  TIM5->EGR = (uint8_t)TIM5_EventSource;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
