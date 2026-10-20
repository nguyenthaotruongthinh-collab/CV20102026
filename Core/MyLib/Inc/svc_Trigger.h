#ifndef APPLICATION_USER_MYLIB_INC_SVC_TRIGGER_H_
#define APPLICATION_USER_MYLIB_INC_SVC_TRIGGER_H_

#include "bsp_InputDigital.h"

typedef enum {
	SVC_STATETRIGGER_RELEASED,
	SVC_STATETRIGGER_PRESS,
	SVC_STATETRIGGER_HOLD,
	SVC_STATETRIGGER_PRESS_N_RELEASED,
	SVC_STATETRIGGER_PRESS_AGAIN,
} SVC_StateTrigger_t;

void svc_Trigger_Update_StateTrigger(void);
void svc_Trigger_set_ModeTriggerHOLD (void);
void svc_Trigger_clear_ModeTriggerHOLD (void);
SVC_StateTrigger_t svc_Trigger_get_StateTrigger(void);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_TRIGGER_H_ */
