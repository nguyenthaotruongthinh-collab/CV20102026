#include "svc_Button.h"
#include "svc_Timer.h"

typedef struct {
	BSP_BTN_ID_t 	ID;
	SVC_BTN_STATE_t state;
    uint32_t        timestamp;
} BtnContext_t;

static const uint32_t DebBtnMs = 50;
static const uint32_t HoldBtnMs = 3000;

static BtnContext_t s_BtnContext[BSP_BTN_ID_COUNT];
static bool s_StateBtnPinFilter[BSP_BTN_ID_COUNT];

void svc_Button_BTN_Init(void){
	for (uint8_t i = 0; i < BSP_BTN_ID_COUNT; i++){
		s_StateBtnPinFilter[i] = true;
		s_BtnContext[i].ID = i;
		s_BtnContext[i].state = SVC_BTN_STATE_RELEASED;
		svc_Timer_set_TZero(&s_BtnContext[i].timestamp);
	}
}

void svc_Button_UpdateStateButton(void){
	static uint32_t debounceMs[BSP_BTN_ID_COUNT] = {0};
	for (uint8_t i = 0; i < BSP_BTN_ID_COUNT; i++){
		bool raw = bsp_InputDigital_BTN_ReadPin(i);

		if (raw == s_StateBtnPinFilter[i]){
			svc_Timer_set_TZero(&debounceMs[i]);
		} else if (svc_Timer_DeltaT_ms(debounceMs[i]) > DebBtnMs){
			s_StateBtnPinFilter[i] = raw;
		}

		if (s_StateBtnPinFilter[i] == true){
			s_BtnContext[i].state = SVC_BTN_STATE_RELEASED;
			svc_Timer_set_TZero(&s_BtnContext[i].timestamp);
		} else {
			if (svc_Timer_DeltaT_ms(s_BtnContext[i].timestamp) > HoldBtnMs){
				s_BtnContext[i].state = SVC_BTN_STATE_HOLD;
			} else {
				s_BtnContext[i].state = SVC_BTN_STATE_PRESSED;
			}
		}
	}
}

SVC_BTN_STATE_t svc_Button_get_StateBtn(BSP_BTN_ID_t ID){
	if (ID >= BSP_BTN_ID_COUNT){
		return SVC_BTN_STATE_RELEASED;
	}

	return s_BtnContext[ID].state;
}
