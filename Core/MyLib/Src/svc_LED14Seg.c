#include "svc_LED14Seg.h"
#include "mid_Font14Seg.h"
#include <string.h>

#define TOTAL_DIGITS  8U
#define LEFT_DIGITS  4U
#define RIGHT_DIGITS  4U
#define MAX_DIGITS 4U

static uint16_t s_segmentCodes[TOTAL_DIGITS];
static char s_LBuffer[MAX_DIGITS*2U];
static char s_RBuffer[MAX_DIGITS*2U];

static void ShowString_LED_Align (char Input[], SVC_LED14Seg_Alignment_t Align, uint16_t *segmentCode){
	for (uint8_t i = 0; i < LEFT_DIGITS*2; i++){
		if (i%2 == 0){
			*(segmentCode + (uint8_t)Align + i/2) = mid_Font14Seg_getCode(Input[i]);
		} else if (Input[i] == '.'){
			*(segmentCode + (uint8_t)Align + i/2) |= mid_Font14Seg_getCode(Input[i]);
		}
	}
}

void svc_LED14Seg_ShowString (char Input[], SVC_LED14Seg_Alignment_t Align){
	ShowString_LED_Align(Input, Align, s_segmentCodes);
}

static void ShowUnit (char Input, SVC_LED14Seg_Alignment_t Align, uint16_t *segmentCode){
	*(segmentCode + (uint8_t)Align + (MAX_DIGITS - 1)) = mid_Font14Seg_getCode(Input);
}

static void Number2String (uint16_t Number, SVC_LED14Seg_Alignment_t Align, SVC_LED14Seg_PointID_t PointID){
	char Buffer[MAX_DIGITS*2U] = {' '};
	uint8_t FinalDigit = MAX_DIGITS - 2;
	uint16_t Temp = Number;

	for (uint8_t i = 0; i < MAX_DIGITS*2U; i++){
		Buffer[i] = ' ';
	}

	if (Number > 999){
		FinalDigit = MAX_DIGITS - 1;
	}

	for (int8_t i = FinalDigit; i >= 0; i--){
		Buffer[i*2U]  = Temp%10 + 48;
		Temp = Temp/10;
	}

	if (PointID != PointID_E0){
		Buffer[(FinalDigit*2U) - PointID] = '.';
	}

	if (Align == Alignment_Left){
		(void)memcpy(s_LBuffer, Buffer, sizeof(s_LBuffer));
		return;
	}

	(void)memcpy(s_RBuffer, Buffer, sizeof(s_RBuffer));
}

void svc_LED14Seg_ShowNumber (uint16_t Number, SVC_LED14Seg_Alignment_t Align, SVC_LED14Seg_PointID_t PointID){
	Number2String(Number, Align, PointID);

	if (Align == Alignment_Left){
		ShowString_LED_Align(s_LBuffer, Align, s_segmentCodes);
		return;
	}

	ShowString_LED_Align(s_RBuffer, Align, s_segmentCodes);
}

void svc_LED14Seg_ShowUint(char Input[], SVC_LED14Seg_Alignment_t Align){
	char c = Input[0];
	ShowUnit(c, Align, s_segmentCodes);
}

void svc_LED14Seg_Init (void){
	svc_LED14Seg_ShowString("L O A D ", Alignment_Left);
	svc_LED14Seg_ShowString("L O A D ", Alignment_Right);
}

void svc_LED14Seg_Reffresh(void){
	static uint8_t CurrentDigit = 0;
	uint8_t AllOFF = 0x00U;
	bsp_LED595_SpiTransmit(&AllOFF, 1U);
	bsp_LED595_PulseLatch(BSP_BLOCK_ID_DIGIT);

//	for (volatile uint32_t i = 0U; i < 100U; i++){
//		__NOP();
//	}

	uint16_t code = s_segmentCodes[CurrentDigit];
	uint8_t segData[2] = { (uint8_t)(code >> 8), (uint8_t)(code & 0xFFU) };

	bsp_LED595_SpiTransmit(segData, 2U);
	bsp_LED595_PulseLatch(BSP_BLOCK_ID_SEG);

	uint8_t DigitMask = (1U << CurrentDigit);
	bsp_LED595_SpiTransmit(&DigitMask, 1U);
	bsp_LED595_PulseLatch(BSP_BLOCK_ID_DIGIT);

	CurrentDigit++;
	if (CurrentDigit >= TOTAL_DIGITS){
		CurrentDigit = 0U;
	}
}
