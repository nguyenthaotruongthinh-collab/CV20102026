#include "bsp_OutputAnalog.h"

extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim6;

void bsp_OutputAnalog_Reffersh_DAC1(uint32_t data){
	uint32_t TempDAC = data;

	if (TempDAC >= MAX_DAC){
		TempDAC = MAX_DAC;
	}

	(void)HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, TempDAC);
}

void bsp_OutputAnalog_Reffersh_DAC2(uint32_t data){
	uint32_t TempDAC = data;

	if (TempDAC >= MAX_DAC){
		TempDAC = MAX_DAC;
	}

	(void)HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, TempDAC);
}

void bsp_OutputAnalog_InitalBuffer(void){
	(void)HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	(void)HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	(void)HAL_TIM_Base_Start_IT(&htim6);
}
