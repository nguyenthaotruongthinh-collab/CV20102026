#ifndef APPLICATION_USER_MYLIB_INC_BSP_TIMER_H_
#define APPLICATION_USER_MYLIB_INC_BSP_TIMER_H_

#include "main.h"
#include "tim.h"

void bsp_Timer_InitTimer(void);
void bsp_Timer_UpdateCounterTIM_us(void);
uint32_t bsp_Timer_get_TickNow(void);

#endif /* APPLICATION_USER_MYLIB_INC_BSP_TIMER_H_ */
