#include "bsp_Timer.h"

#define TickTIM 10 //us

static uint32_t CounterTIM_us = 0;

void bsp_Timer_InitTimer (void){
	HAL_TIM_Base_Start_IT(&htim8);
}

void bsp_Timer_UpdateCounterTIM_us (void){
	CounterTIM_us += TickTIM;
}

uint32_t bsp_Timer_get_TickNow (void){
	return CounterTIM_us;
}
