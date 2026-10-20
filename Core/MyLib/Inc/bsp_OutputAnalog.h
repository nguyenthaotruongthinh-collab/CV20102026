#ifndef APPLICATION_USER_MYLIB_INC_BSP_OUTPUTANALOG_H_
#define APPLICATION_USER_MYLIB_INC_BSP_OUTPUTANALOG_H_

#include "main.h"
#include "dac.h"
#include "stdbool.h"
#include "stdint.h"
#include <string.h>
#include "tim.h"

#define MAX_DAC 4095U
#define VREF 			3300U //mV
#define JUMPTIME_DAC	  50U //us

void bsp_OutputAnalog_InitalBuffer(void);
void bsp_OutputAnalog_Reffersh_DAC1(uint32_t data);
void bsp_OutputAnalog_Reffersh_DAC2(uint32_t data);

#endif /* APPLICATION_USER_MYLIB_INC_BSP_OUTPUTANALOG_H_ */
