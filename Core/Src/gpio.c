/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, _o_SysPeak_Pin|_o_SysEP_Pin|_o_Display_Segments_Pin|_o_Inductor_Trigger_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, _o_CS_InsideSD_Pin|_o_DisplayWarning_Pin|_o_Display1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(_o_Inductor_EP_GPIO_Port, _o_Inductor_EP_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, _o_Display2_Pin|_o_Display_Digit_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, o_EP_Pin|o_EN_Pin|o_PWM_On_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, o_Gas_Pin|o_HF_Pin|o_Touch_Sense_Pin|o_Signal_Spare_Pin
                          |o_Signal_FinalSlope_Pin|o_Signal_ARCLock_Pin|o_Signal_ARCon_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, o_Relay400V_Pin|o_Emergency_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, o_DE_RS485_Pin|o_PowerTank_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(o_Assistant_ARC_GPIO_Port, o_Assistant_ARC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : _o_SysPeak_Pin _o_SysEP_Pin */
  GPIO_InitStruct.Pin = _o_SysPeak_Pin|_o_SysEP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : _i_SysPeak_Pin _i_SysEP_Pin _i_SysARCOn_Pin _i_Trigger_Pin
                           _i_HC147_Y0_Pin _i_HC147_Y1_Pin _i_HC147_Y2_Pin _i_HC147_Y3_Pin */
  GPIO_InitStruct.Pin = _i_SysPeak_Pin|_i_SysEP_Pin|_i_SysARCOn_Pin|_i_Trigger_Pin
                          |_i_HC147_Y0_Pin|_i_HC147_Y1_Pin|_i_HC147_Y2_Pin|_i_HC147_Y3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : _o_CS_InsideSD_Pin */
  GPIO_InitStruct.Pin = _o_CS_InsideSD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(_o_CS_InsideSD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : i_ARCOn_Pin */
  GPIO_InitStruct.Pin = i_ARCOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(i_ARCOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : _o_Inductor_EP_Pin */
  GPIO_InitStruct.Pin = _o_Inductor_EP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(_o_Inductor_EP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : _o_DisplayWarning_Pin _o_Display1_Pin */
  GPIO_InitStruct.Pin = _o_DisplayWarning_Pin|_o_Display1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : _o_Display2_Pin _o_Display_Digit_Pin */
  GPIO_InitStruct.Pin = _o_Display2_Pin|_o_Display_Digit_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : _o_Display_Segments_Pin */
  GPIO_InitStruct.Pin = _o_Display_Segments_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(_o_Display_Segments_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : o_EP_Pin o_EN_Pin */
  GPIO_InitStruct.Pin = o_EP_Pin|o_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : _o_Inductor_Trigger_Pin */
  GPIO_InitStruct.Pin = _o_Inductor_Trigger_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(_o_Inductor_Trigger_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : _i_B_AC_Parameter_Pin _i_B_AC_Shape_Pin _i_B_Gas_Pin _i_B_External_Pin
                           _i_B_Pulser_Pin */
  GPIO_InitStruct.Pin = _i_B_AC_Parameter_Pin|_i_B_AC_Shape_Pin|_i_B_Gas_Pin|_i_B_External_Pin
                          |_i_B_Pulser_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : _i_B_Mode_Pin _i_B_Polarilty_Pin _i_B_Process_Pin _i_B_Amper_Pin
                           _i_B_Memory_Pin _i_ENCB_Pin _i_CodeMem3_Pin */
  GPIO_InitStruct.Pin = _i_B_Mode_Pin|_i_B_Polarilty_Pin|_i_B_Process_Pin|_i_B_Amper_Pin
                          |_i_B_Memory_Pin|_i_ENCB_Pin|_i_CodeMem3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : o_Gas_Pin o_HF_Pin */
  GPIO_InitStruct.Pin = o_Gas_Pin|o_HF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : _i_ENCA_Pin _i_B_ENC_Pin _i_B_Sequence_Pin _i_CodeMem2_Pin
                           _i_CodeMem1_Pin */
  GPIO_InitStruct.Pin = _i_ENCA_Pin|_i_B_ENC_Pin|_i_B_Sequence_Pin|_i_CodeMem2_Pin
                          |_i_CodeMem1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : o_Relay400V_Pin o_Emergency_Pin */
  GPIO_InitStruct.Pin = o_Relay400V_Pin|o_Emergency_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : o_DE_RS485_Pin */
  GPIO_InitStruct.Pin = o_DE_RS485_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(o_DE_RS485_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : o_PowerTank_Pin */
  GPIO_InitStruct.Pin = o_PowerTank_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(o_PowerTank_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : i_ArcForce_Pin i_Touch_Sense_Enable_Pin */
  GPIO_InitStruct.Pin = i_ArcForce_Pin|i_Touch_Sense_Enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : o_Touch_Sense_Pin o_Signal_Spare_Pin o_Signal_FinalSlope_Pin o_Signal_ARCLock_Pin
                           o_Signal_ARCon_Pin */
  GPIO_InitStruct.Pin = o_Touch_Sense_Pin|o_Signal_Spare_Pin|o_Signal_FinalSlope_Pin|o_Signal_ARCLock_Pin
                          |o_Signal_ARCon_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : i_RobotMode2_Pin i_RobotMode1_Pin */
  GPIO_InitStruct.Pin = i_RobotMode2_Pin|i_RobotMode1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : o_Assistant_ARC_Pin */
  GPIO_InitStruct.Pin = o_Assistant_ARC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(o_Assistant_ARC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : o_PWM_On_Pin */
  GPIO_InitStruct.Pin = o_PWM_On_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(o_PWM_On_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
