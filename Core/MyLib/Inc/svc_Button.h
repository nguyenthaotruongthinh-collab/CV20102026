#ifndef APPLICATION_USER_MYLIB_INC_SVC_BUTTON_H_
#define APPLICATION_USER_MYLIB_INC_SVC_BUTTON_H_

#include "bsp_InputDigital.h"

typedef enum {
	SVC_BTN_STATE_RELEASED,
	SVC_BTN_STATE_PRESSED,
	SVC_BTN_STATE_HOLD
} SVC_BTN_STATE_t;

void svc_Button_BTN_Init(void);
void svc_Button_UpdateStateButton(void);
SVC_BTN_STATE_t svc_Button_get_StateBtn(BSP_BTN_ID_t ID);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_BUTTON_H_ */
