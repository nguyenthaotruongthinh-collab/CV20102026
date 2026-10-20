#include "svc_Trigger.h"
#include "svc_Timer.h"
#include <stdbool.h>

typedef struct{
	SVC_StateTrigger_t	State;
	uint32_t			timestamp;
} TriggerContext_t;

typedef enum{
	MODE_TRIGGER_ID_STD = 0,
	MODE_TRIGGER_ID_HOLD
}ModeTriggerID_t;

static const uint32_t DebTriggerMs = 50;

static TriggerContext_t s_TriggerContext = {.State = SVC_STATETRIGGER_RELEASED, .timestamp = 0};
static ModeTriggerID_t s_ModeTrigger = MODE_TRIGGER_ID_STD;
static bool s_StateTriggerPinFilter = true;

static void Update_StateTriggerPinFilter(void){
	static uint32_t debounceMs = 0;

	bool raw = bsp_InputDigital_Trigger_ReadPin();

	if (raw == s_StateTriggerPinFilter){
		svc_Timer_set_TZero(&debounceMs);
	} else if (svc_Timer_DeltaT_ms(debounceMs) > DebTriggerMs){
		s_StateTriggerPinFilter = raw;
	}
}

static SVC_StateTrigger_t StateTriggerSTD(void){
	if (s_StateTriggerPinFilter == true){
		return SVC_STATETRIGGER_RELEASED;
	}

	return SVC_STATETRIGGER_PRESS;
}

static SVC_StateTrigger_t StateTriggerHOLD(SVC_StateTrigger_t PreviousStateTrigger){
	if (s_StateTriggerPinFilter == true){
		switch ((uint8_t)PreviousStateTrigger){
			case (uint8_t)SVC_STATETRIGGER_PRESS:
				return SVC_STATETRIGGER_PRESS_N_RELEASED;
			break;
			case (uint8_t)SVC_STATETRIGGER_PRESS_AGAIN:
				return SVC_STATETRIGGER_RELEASED;
			break;
			default:
				return PreviousStateTrigger;
			break;
		}
	}

	switch ((uint8_t)PreviousStateTrigger){
		case (uint8_t)SVC_STATETRIGGER_PRESS_N_RELEASED:
			return SVC_STATETRIGGER_PRESS_AGAIN;
		break;
		case (uint8_t)SVC_STATETRIGGER_RELEASED:
			return SVC_STATETRIGGER_PRESS;
		break;
		default:
			return PreviousStateTrigger;
		break;
	}
}

void svc_Trigger_Update_StateTrigger(void){
	Update_StateTriggerPinFilter();

	if (s_ModeTrigger == MODE_TRIGGER_ID_STD){
		s_TriggerContext.State = StateTriggerSTD();
		return;
	}

	SVC_StateTrigger_t PreviousStateTrigger = s_TriggerContext.State;
	s_TriggerContext.State = StateTriggerHOLD(PreviousStateTrigger);
}

void svc_Trigger_set_ModeTriggerHOLD (void){
	s_ModeTrigger = MODE_TRIGGER_ID_HOLD;
}

void svc_Trigger_clear_ModeTriggerHOLD (void){
	s_ModeTrigger = MODE_TRIGGER_ID_STD;
}

SVC_StateTrigger_t svc_get_StateTrigger(void){
	return s_TriggerContext.State;
}
