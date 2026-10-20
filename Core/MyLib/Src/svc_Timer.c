#include "svc_Timer.h"

const uint32_t _us = 1;
const uint32_t _ms = 1000;
const uint32_t _s = 1000000;

uint32_t svc_Timer_DeltaT_us (uint32_t TZero){
	uint32_t Input = TZero;
	uint32_t Output_us = bsp_Timer_get_TickNow() - Input;

	return (Output_us/(_us));
}

uint32_t svc_Timer_DeltaT_ms (uint32_t TZero){
	uint32_t Input = TZero;
	uint32_t Output_us = bsp_Timer_get_TickNow() - Input;

	return (Output_us/(_ms));
}

uint32_t svc_Timer_DeltaT_s (uint32_t TZero){
	uint32_t Input = TZero;
	uint32_t Output_us = bsp_Timer_get_TickNow() - Input;

	return (Output_us/(_s));
}

void svc_Timer_set_TZero (uint32_t *TIM){
	*TIM = bsp_Timer_get_TickNow();
}
