#ifndef APPLICATION_USER_MYLIB_INC_MID_WAVEFORM_H_
#define APPLICATION_USER_MYLIB_INC_MID_WAVEFORM_H_

#include <stdint.h>
#include "mid_ManagerMenu.h"

typedef struct{
	uint32_t MaxEN;
	uint32_t MaxEP;
	uint32_t Min;

	uint32_t TimeEN;
	uint32_t TimeEP;

	uint32_t Cycle;
} ParameterAC_t;

typedef struct{
	uint32_t MaxPeak;
	uint32_t MaxBase;
	uint32_t TimePeak;
	uint32_t TimeBase;
	uint32_t Cycle;
} ParameterPulse_t;

void mid_Waveform_setRampSoftSquareAC(uint32_t JUp, uint32_t JDown);
uint32_t mid_Waveform_SoftSquareAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us);

void mid_Waveform_setRampSquareAC(uint32_t JUp1, uint32_t JUp2, uint32_t JDown1);
uint32_t mid_Waveform_SquareAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us);

uint32_t mid_Waveform_SinAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us);

uint32_t mid_Waveform_TriAC(uint32_t Min, uint32_t Max, uint32_t TMax, uint32_t t_us);

uint32_t mid_Waveform_SquareDC(uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us);

uint32_t mid_Waveform_SinDC(uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us);

uint32_t mid_Waveform_TriDC(uint32_t Min, uint32_t Max, uint32_t TMax, bool IsPeak, uint32_t t_us);

#endif /* APPLICATION_USER_MYLIB_INC_MID_WAVEFORM_H_ */
