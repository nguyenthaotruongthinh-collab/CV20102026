#ifndef APPLICATION_USER_MYLIB_INC_SVC_LED14SEG_H_
#define APPLICATION_USER_MYLIB_INC_SVC_LED14SEG_H_

#include "bsp_LED595.h"
#include <stdint.h>

typedef enum{
	Alignment_Left = 0,
	Alignment_Right = 4
}SVC_LED14Seg_Alignment_t;

typedef enum{
	PointID_E0 = 0,
	PointID_E1 = 1,
	PointID_E2 = 3,
	PointID_E3 = 5
}SVC_LED14Seg_PointID_t;

void svc_LED14Seg_ShowString (char Input[], SVC_LED14Seg_Alignment_t Align);
void svc_LED14Seg_ShowNumber (uint16_t Number, SVC_LED14Seg_Alignment_t Align, SVC_LED14Seg_PointID_t PointID);
void svc_LED14Seg_ShowUint(char Input[], SVC_LED14Seg_Alignment_t Align);

void svc_LED14Seg_Init(void);
void svc_LED14Seg_Reffresh(void);


#endif /* APPLICATION_USER_MYLIB_INC_SVC_LED14SEG_H_ */
