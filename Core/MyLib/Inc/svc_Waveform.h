#ifndef APPLICATION_USER_MYLIB_INC_SVC_WAVEFORM_H_
#define APPLICATION_USER_MYLIB_INC_SVC_WAVEFORM_H_

#include "bsp_OutputAnalog.h"
#include "mid_Waveform.h"
#include "mid_ManagerMenu.h"
#include <stdlib.h>

void svc_Waveform_Init(void);

void svc_Waveform_setPeak(uint32_t Peak, uint32_t MinAC);
void svc_Waveform_setPeak2(uint32_t Peak);

void svc_Waveform_setPulse (uint32_t f, uint32_t d, uint32_t Base);

void svc_Waveform_setAC(uint32_t f, uint32_t Balance, uint32_t RaitoNP);

void svc_Waveform_setIsAC(void);

void svc_Waveform_setIsPulse(void);

void svc_Waveform_clearIsAC(void);

void svc_Waveform_clearIsPulse(void);

void svc_Waveform_setShapeAC(MIDMenuACShapeId_t Shape, MIDMenuCOMA_t COMA);

void svc_Waveform_setShapePulse(MIDMenuPulseShape_t Shape);

void svc_Waveform_UpdateRMSArr(void);
uint32_t svc_Waveform_getEN(uint32_t A_RMS, MIDMenuACShapeId_t Shape);
uint32_t svc_Waveform_getRMS(uint32_t A_EN, MIDMenuACShapeId_t Shape);

void svc_Waveform_UpdateRingBuffer(void);

void svc_Waveform_UpdateDAC(void);



#endif /* APPLICATION_USER_MYLIB_INC_SVC_WAVEFORM_H_ */
