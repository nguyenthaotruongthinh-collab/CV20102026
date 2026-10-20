#ifndef APPLICATION_USER_MYLIB_INC_SVC_ENCODER_H_
#define APPLICATION_USER_MYLIB_INC_SVC_ENCODER_H_

#include "bsp_InputDigital.h"

void svc_Encoder_Init(void);
void svc_Encoder_Poll(void);
int8_t svc_Encoder_getRawCount(void);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_ENCODER_H_ */
