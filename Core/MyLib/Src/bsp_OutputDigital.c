#include "bsp_OutputDigital.h"
#include "tim.h"

extern TIM_HandleTypeDef htim8;

// Normal Output Mapping
static const GPIO_TypeDef* s_NormalOutput_ports[OUTPUT_ID_NORMAL_COUNT] = {
	[OUTPUT_ID_PWM] 				= o_PWM_On_GPIO_Port,
	[OUTPUT_ID_GAS] 				= o_Gas_GPIO_Port,
	[OUTPUT_ID_HF] 					= o_HF_GPIO_Port,
	[OUTPUT_ID_EP] 					= o_EP_GPIO_Port,
	[OUTPUT_ID_EN] 					= o_EN_GPIO_Port,
	[OUTPUT_ID_ASSISTANT_ENABLE] 	= o_Relay400V_GPIO_Port,
	[OUTPUT_ID_ASSISTANT] 			= o_Assistant_ARC_GPIO_Port,
	[OUTPUT_ID_TANK] 				= o_PowerTank_GPIO_Port,
};

static const uint16_t s_NormalOutput_pins[OUTPUT_ID_NORMAL_COUNT] = {
	[OUTPUT_ID_PWM] 				= o_PWM_On_Pin,
	[OUTPUT_ID_GAS] 				= o_Gas_Pin,
	[OUTPUT_ID_HF] 					= o_HF_Pin,
	[OUTPUT_ID_EP] 					= o_EP_Pin,
	[OUTPUT_ID_EN] 					= o_EN_Pin,
	[OUTPUT_ID_ASSISTANT_ENABLE] 	= o_Relay400V_Pin,
	[OUTPUT_ID_ASSISTANT] 			= o_Assistant_ARC_Pin,
	[OUTPUT_ID_TANK] 				= o_PowerTank_Pin,
};

static BSP_StateOutput_t s_NormalOutput[OUTPUT_ID_NORMAL_COUNT];

/* **************** PRIVATE FUNCTION **************** */
static void NormalOutput_Init(void) {
	/* GPIO already initialized by CubeMX */
	for (uint16_t i = 0; i < OUTPUT_ID_NORMAL_COUNT; i++){
		s_NormalOutput[i].Port = (GPIO_TypeDef*)s_NormalOutput_ports[i];
		s_NormalOutput[i].Pin = s_NormalOutput_pins[i];
		s_NormalOutput[i].id = i;
		s_NormalOutput[i].IsSet = false;
		s_NormalOutput[i].State = true;
	}
}

static void UpdateOutput(BSP_NormalOutput_ID_t id, BSP_StateOutput_t *ConfigOutput){
	GPIO_PinState pinState;

    if ((ConfigOutput + id)->State == (ConfigOutput + id)->IsSet) {
    	return;
    }

    pinState = (ConfigOutput + id)->IsSet ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin((GPIO_TypeDef*)((ConfigOutput + id)->Port), (ConfigOutput + id)->Pin, pinState);
    (ConfigOutput + id)->State = (ConfigOutput + id)->IsSet;
}

static void NormalOutput (void){
	for (uint16_t i = 0; i < OUTPUT_ID_NORMAL_COUNT; i++){
		UpdateOutput(i, s_NormalOutput);
	}
}
/* **************** PUBLIC FUNCTION **************** */
void bsp_OutputDigital_Init(void){
	NormalOutput_Init();

	NormalOutput();

	HAL_TIM_Base_Start_IT(&htim8);
}

void bsp_OutputDigital_setNormalOutput(BSP_NormalOutput_ID_t id){
	s_NormalOutput[id].IsSet = true;
}

void bsp_OutputDigital_clearNormalOutput(BSP_NormalOutput_ID_t id){
	s_NormalOutput[id].IsSet = false;
}

void bsp_OutputDigital(void){
	NormalOutput();
}
