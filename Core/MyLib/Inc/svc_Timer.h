#ifndef APPLICATION_USER_MYLIB_INC_SVC_TIMER_H_
#define APPLICATION_USER_MYLIB_INC_SVC_TIMER_H_

#include "bsp_Timer.h"

uint32_t svc_Timer_DeltaT_us(uint32_t TZero);
uint32_t svc_Timer_DeltaT_ms(uint32_t TZero);
uint32_t svc_Timer_DeltaT_s(uint32_t TZero);
void svc_Timer_set_TZero(uint32_t *TIM);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_TIMER_H_ */
