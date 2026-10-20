#ifndef APPLICATION_USER_MYLIB_INC_BSP_OUTPUTDIGITAL_H_
#define APPLICATION_USER_MYLIB_INC_BSP_OUTPUTDIGITAL_H_

#include "main.h"
#include "gpio.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "svc_Timer.h"

typedef enum {
	OUTPUT_ID_PWM = 0,
	OUTPUT_ID_GAS,
	OUTPUT_ID_HF,
	OUTPUT_ID_EP,
	OUTPUT_ID_EN,
	OUTPUT_ID_ASSISTANT_ENABLE,
	OUTPUT_ID_ASSISTANT,
	OUTPUT_ID_TANK,
	OUTPUT_ID_NORMAL_COUNT
} BSP_NormalOutput_ID_t;

typedef struct{
	GPIO_TypeDef* Port;
	uint16_t Pin;
	uint16_t id;
	bool IsSet;
	bool State;
}BSP_StateOutput_t;

void bsp_OutputDigital_Init(void);
void bsp_OutputDigital_setNormalOutput(BSP_NormalOutput_ID_t id);
void bsp_OutputDigital_clearNormalOutput(BSP_NormalOutput_ID_t id);
void bsp_OutputDigital(void);

#endif /* APPLICATION_USER_MYLIB_INC_BSP_OUTPUTDIGITAL_H_ */
