/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define _o_SysPeak_Pin GPIO_PIN_2
#define _o_SysPeak_GPIO_Port GPIOE
#define _i_SysPeak_Pin GPIO_PIN_3
#define _i_SysPeak_GPIO_Port GPIOE
#define _i_SysEP_Pin GPIO_PIN_4
#define _i_SysEP_GPIO_Port GPIOE
#define _o_SysEP_Pin GPIO_PIN_5
#define _o_SysEP_GPIO_Port GPIOE
#define _i_SysARCOn_Pin GPIO_PIN_6
#define _i_SysARCOn_GPIO_Port GPIOE
#define _o_CS_InsideSD_Pin GPIO_PIN_13
#define _o_CS_InsideSD_GPIO_Port GPIOC
#define i_ARCOn_Pin GPIO_PIN_15
#define i_ARCOn_GPIO_Port GPIOC
#define adc_Robot_CMDEP_Pin GPIO_PIN_0
#define adc_Robot_CMDEP_GPIO_Port GPIOC
#define adc_Robot_CMDEN_Pin GPIO_PIN_1
#define adc_Robot_CMDEN_GPIO_Port GPIOC
#define adc_VoltFeedback_Pin GPIO_PIN_0
#define adc_VoltFeedback_GPIO_Port GPIOA
#define adc_AmperFeedback_Pin GPIO_PIN_1
#define adc_AmperFeedback_GPIO_Port GPIOA
#define adc_Robot_CMD_Pin GPIO_PIN_2
#define adc_Robot_CMD_GPIO_Port GPIOA
#define _o_Inductor_EP_Pin GPIO_PIN_3
#define _o_Inductor_EP_GPIO_Port GPIOA
#define dac_CMDCurrent_Pin GPIO_PIN_4
#define dac_CMDCurrent_GPIO_Port GPIOA
#define dac_CMDInductor_Pin GPIO_PIN_5
#define dac_CMDInductor_GPIO_Port GPIOA
#define adc_InductorFeedback_Pin GPIO_PIN_6
#define adc_InductorFeedback_GPIO_Port GPIOA
#define _o_DisplayWarning_Pin GPIO_PIN_4
#define _o_DisplayWarning_GPIO_Port GPIOC
#define _o_Display1_Pin GPIO_PIN_5
#define _o_Display1_GPIO_Port GPIOC
#define _o_Display2_Pin GPIO_PIN_0
#define _o_Display2_GPIO_Port GPIOB
#define _o_Display_Digit_Pin GPIO_PIN_1
#define _o_Display_Digit_GPIO_Port GPIOB
#define _o_Display_Segments_Pin GPIO_PIN_7
#define _o_Display_Segments_GPIO_Port GPIOE
#define _i_Trigger_Pin GPIO_PIN_8
#define _i_Trigger_GPIO_Port GPIOE
#define o_EP_Pin GPIO_PIN_9
#define o_EP_GPIO_Port GPIOE
#define o_EN_Pin GPIO_PIN_10
#define o_EN_GPIO_Port GPIOE
#define _i_HC147_Y0_Pin GPIO_PIN_11
#define _i_HC147_Y0_GPIO_Port GPIOE
#define _i_HC147_Y1_Pin GPIO_PIN_12
#define _i_HC147_Y1_GPIO_Port GPIOE
#define _i_HC147_Y2_Pin GPIO_PIN_13
#define _i_HC147_Y2_GPIO_Port GPIOE
#define _i_HC147_Y3_Pin GPIO_PIN_14
#define _i_HC147_Y3_GPIO_Port GPIOE
#define _o_Inductor_Trigger_Pin GPIO_PIN_15
#define _o_Inductor_Trigger_GPIO_Port GPIOE
#define _i_B_AC_Parameter_Pin GPIO_PIN_11
#define _i_B_AC_Parameter_GPIO_Port GPIOB
#define _i_B_AC_Shape_Pin GPIO_PIN_12
#define _i_B_AC_Shape_GPIO_Port GPIOB
#define _i_B_Gas_Pin GPIO_PIN_13
#define _i_B_Gas_GPIO_Port GPIOB
#define _i_B_External_Pin GPIO_PIN_14
#define _i_B_External_GPIO_Port GPIOB
#define _i_B_Pulser_Pin GPIO_PIN_15
#define _i_B_Pulser_GPIO_Port GPIOB
#define _i_B_Mode_Pin GPIO_PIN_8
#define _i_B_Mode_GPIO_Port GPIOD
#define _i_B_Polarilty_Pin GPIO_PIN_9
#define _i_B_Polarilty_GPIO_Port GPIOD
#define _i_B_Process_Pin GPIO_PIN_10
#define _i_B_Process_GPIO_Port GPIOD
#define _i_B_Amper_Pin GPIO_PIN_11
#define _i_B_Amper_GPIO_Port GPIOD
#define _i_B_Memory_Pin GPIO_PIN_12
#define _i_B_Memory_GPIO_Port GPIOD
#define o_Gas_Pin GPIO_PIN_13
#define o_Gas_GPIO_Port GPIOD
#define o_HF_Pin GPIO_PIN_14
#define o_HF_GPIO_Port GPIOD
#define _i_ENCB_Pin GPIO_PIN_15
#define _i_ENCB_GPIO_Port GPIOD
#define _i_ENCA_Pin GPIO_PIN_6
#define _i_ENCA_GPIO_Port GPIOC
#define _i_B_ENC_Pin GPIO_PIN_7
#define _i_B_ENC_GPIO_Port GPIOC
#define _i_B_Sequence_Pin GPIO_PIN_8
#define _i_B_Sequence_GPIO_Port GPIOC
#define o_Relay400V_Pin GPIO_PIN_9
#define o_Relay400V_GPIO_Port GPIOC
#define o_DE_RS485_Pin GPIO_PIN_8
#define o_DE_RS485_GPIO_Port GPIOA
#define o_PowerTank_Pin GPIO_PIN_15
#define o_PowerTank_GPIO_Port GPIOA
#define o_Emergency_Pin GPIO_PIN_10
#define o_Emergency_GPIO_Port GPIOC
#define _i_CodeMem2_Pin GPIO_PIN_11
#define _i_CodeMem2_GPIO_Port GPIOC
#define _i_CodeMem1_Pin GPIO_PIN_12
#define _i_CodeMem1_GPIO_Port GPIOC
#define _i_CodeMem3_Pin GPIO_PIN_0
#define _i_CodeMem3_GPIO_Port GPIOD
#define i_ArcForce_Pin GPIO_PIN_1
#define i_ArcForce_GPIO_Port GPIOD
#define o_Touch_Sense_Pin GPIO_PIN_2
#define o_Touch_Sense_GPIO_Port GPIOD
#define i_Touch_Sense_Enable_Pin GPIO_PIN_3
#define i_Touch_Sense_Enable_GPIO_Port GPIOD
#define o_Signal_Spare_Pin GPIO_PIN_4
#define o_Signal_Spare_GPIO_Port GPIOD
#define o_Signal_FinalSlope_Pin GPIO_PIN_5
#define o_Signal_FinalSlope_GPIO_Port GPIOD
#define o_Signal_ARCLock_Pin GPIO_PIN_6
#define o_Signal_ARCLock_GPIO_Port GPIOD
#define o_Signal_ARCon_Pin GPIO_PIN_7
#define o_Signal_ARCon_GPIO_Port GPIOD
#define i_RobotMode2_Pin GPIO_PIN_4
#define i_RobotMode2_GPIO_Port GPIOB
#define i_RobotMode1_Pin GPIO_PIN_5
#define i_RobotMode1_GPIO_Port GPIOB
#define o_Assistant_ARC_Pin GPIO_PIN_8
#define o_Assistant_ARC_GPIO_Port GPIOB
#define o_PWM_On_Pin GPIO_PIN_1
#define o_PWM_On_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
