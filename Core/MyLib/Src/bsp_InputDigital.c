#include "bsp_InputDigital.h"

// Button Mapping
static const GPIO_TypeDef* s_BTN_ports[BSP_BTN_ID_COUNT] = {
	[BSP_BTN_ID_Amper]       = _i_B_Amper_GPIO_Port,
	[BSP_BTN_ID_Memory]      = _i_B_Memory_GPIO_Port,
	[BSP_BTN_ID_Process]     = _i_B_Process_GPIO_Port,
	[BSP_BTN_ID_Polarity]    = _i_B_Polarilty_GPIO_Port,
	[BSP_BTN_ID_Mode]        = _i_B_Mode_GPIO_Port,
	[BSP_BTN_ID_Pulser]      = _i_B_Pulser_GPIO_Port,
	[BSP_BTN_ID_Sequence]    = _i_B_Sequence_GPIO_Port,
	[BSP_BTN_ID_Gas]        = _i_B_Gas_GPIO_Port,
	[BSP_BTN_ID_ACShape]     = _i_B_AC_Shape_GPIO_Port,
    [BSP_BTN_ID_ACParameter] = _i_B_AC_Parameter_GPIO_Port,
    [BSP_BTN_ID_External]    = _i_B_External_GPIO_Port
};

static const uint16_t s_BTN_pins[BSP_BTN_ID_COUNT] = {
	[BSP_BTN_ID_Amper]       = _i_B_Amper_Pin,
	[BSP_BTN_ID_Memory]      = _i_B_Memory_Pin,
	[BSP_BTN_ID_Process]     = _i_B_Process_Pin,
	[BSP_BTN_ID_Polarity]    = _i_B_Polarilty_Pin,
	[BSP_BTN_ID_Mode]        = _i_B_Mode_Pin,
	[BSP_BTN_ID_Pulser]      = _i_B_Pulser_Pin,
	[BSP_BTN_ID_Sequence]    = _i_B_Sequence_Pin,
	[BSP_BTN_ID_Gas]        = _i_B_Gas_Pin,
	[BSP_BTN_ID_ACShape]     = _i_B_AC_Shape_Pin,
    [BSP_BTN_ID_ACParameter] = _i_B_AC_Parameter_Pin,
    [BSP_BTN_ID_External]    = _i_B_External_Pin,
};

// Faults Mapping
static const GPIO_TypeDef* s_Faults_ports[BSP_FAULT_ID_COUNT] = {
	[BSP_FAULT_ID_Y0]       = _i_HC147_Y0_GPIO_Port,
	[BSP_FAULT_ID_Y1]       = _i_HC147_Y1_GPIO_Port,
	[BSP_FAULT_ID_Y2]       = _i_HC147_Y2_GPIO_Port,
	[BSP_FAULT_ID_Y3]       = _i_HC147_Y3_GPIO_Port
};

static const uint16_t s_Faults_pins[BSP_FAULT_ID_COUNT] = {
	[BSP_FAULT_ID_Y0]       = _i_HC147_Y0_Pin,
	[BSP_FAULT_ID_Y1]       = _i_HC147_Y1_Pin,
	[BSP_FAULT_ID_Y2]       = _i_HC147_Y2_Pin,
	[BSP_FAULT_ID_Y3]       = _i_HC147_Y3_Pin,
};

// Trigger Mapping
static const GPIO_TypeDef* s_Trigger_ports = _i_Trigger_GPIO_Port;
static const uint16_t s_Trigger_pins = _i_Trigger_Pin;

//static const uint16_t s_Trigger_pins = _i_B_Amper_Pin;
//static const GPIO_TypeDef* s_Trigger_ports = _i_B_Amper_GPIO_Port;

// Encoder Mapping
static const GPIO_TypeDef* s_EncoderA_ports = _i_ENCB_GPIO_Port;
static const uint16_t s_EncoderA_pins = _i_ENCB_Pin;

static const GPIO_TypeDef* s_EncoderB_ports = _i_ENCA_GPIO_Port;
static const uint16_t s_EncoderB_pins = _i_ENCA_Pin;

// Init
static void bsp_BTN_Init(void) { /* GPIO already initialized by CubeMX */ }
static void bsp_Fault_Init(void) { /* GPIO already initialized by CubeMX */ }
static void bsp_Trigger_Init(void) { /* GPIO already initialized by CubeMX */ }
static void bsp_Encoder_Init(void) { /* GPIO already initialized by CubeMX */ }

void bsp_InputDigital_Init(void){
	bsp_BTN_Init();
	bsp_Fault_Init();
	bsp_Trigger_Init();
	bsp_Encoder_Init();
}

// ReadPin
bool bsp_InputDigital_BTN_ReadPin(BSP_BTN_ID_t id){
    bool result = true;
    if (id < BSP_BTN_ID_COUNT) {
        GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef*)s_BTN_ports[id], s_BTN_pins[id]);
        result = (state == GPIO_PIN_SET);  // active low
    }
    return result;
}

uint8_t bsp_InputDigital_Fault_ReadPin(void){
	uint8_t FaultCode = 0x00;
	GPIO_PinState state = GPIO_PIN_RESET;

	for (uint8_t i = 0; i < BSP_FAULT_ID_COUNT; i++){
		state = HAL_GPIO_ReadPin((GPIO_TypeDef*)s_Faults_ports[i], s_Faults_pins[i]);

		if (state == GPIO_PIN_SET){
			FaultCode |= 1<<i;
		}
	}

	return FaultCode;
}

bool bsp_InputDigital_Trigger_ReadPin(void){
	bool result = true;

	GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef*)s_Trigger_ports, s_Trigger_pins);
	result = (state == GPIO_PIN_SET);  // active low

	return result;
}

bool bsp_InputDigital_EncoderA_ReadPin(void){
	bool result = true;

	GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef*)s_EncoderA_ports, s_EncoderA_pins);
	result = (state == GPIO_PIN_SET);  // active low

	return result;
}

bool bsp_InputDigital_EncoderB_ReadPin(void){
	bool result = true;

	GPIO_PinState state = HAL_GPIO_ReadPin((GPIO_TypeDef*)s_EncoderB_ports, s_EncoderB_pins);
	result = (state == GPIO_PIN_SET);  // active low

	return result;
}
