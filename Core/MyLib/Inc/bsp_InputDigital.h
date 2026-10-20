#ifndef APPLICATION_USER_MYLIB_INC_BSP_INPUTDIGITAL_H_
#define APPLICATION_USER_MYLIB_INC_BSP_INPUTDIGITAL_H_

#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef enum {
	BSP_BTN_ID_Amper = 0,
	BSP_BTN_ID_Memory,
	BSP_BTN_ID_Process,
	BSP_BTN_ID_Polarity,
	BSP_BTN_ID_Mode,
	BSP_BTN_ID_Pulser,
	BSP_BTN_ID_Sequence,
	BSP_BTN_ID_Gas,
	BSP_BTN_ID_ACShape,
	BSP_BTN_ID_ACParameter,
	BSP_BTN_ID_External,
	BSP_BTN_ID_COUNT
} BSP_BTN_ID_t;

typedef enum {
	BSP_FAULT_ID_Y0 = 0,
	BSP_FAULT_ID_Y1,
	BSP_FAULT_ID_Y2,
	BSP_FAULT_ID_Y3,
	BSP_FAULT_ID_COUNT
} BSP_Fault_ID_t;

void bsp_InputDigital_Init(void);

bool bsp_InputDigital_BTN_ReadPin(BSP_BTN_ID_t id);

uint8_t bsp_InputDigital_Fault_ReadPin(void);

bool bsp_InputDigital_Trigger_ReadPin(void);

bool bsp_InputDigital_EncoderA_ReadPin(void);
bool bsp_InputDigital_EncoderB_ReadPin(void);

#endif /* APPLICATION_USER_MYLIB_INC_BSP_INPUTDIGITAL_H_ */
