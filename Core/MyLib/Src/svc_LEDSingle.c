#include "svc_LEDSingle.h"
#include "svc_Timer.h"
#include <stdbool.h>

#define TIME_ON 500
#define TIME_OFF 300

typedef enum {
	STATE_LEDID_OFF = 0,
	STATE_LEDID_ON,
	STATE_LEDID_BLINK
}StateSingleLEDID_t;

typedef struct {
	SVC_LEDSingleID_t ID;
	StateSingleLEDID_t StateLED;
	bool EnaBlink;
	uint32_t TimerBlink;
} SingleLEDControl_t;

static SingleLEDControl_t s_SLED[SLEDID_COUNT];
static uint16_t ledGroup1, ledGroup2;
static uint8_t  ledGroup3;
static uint8_t GroupID[3] = {0U, 0U, 0U};

static void setLEDSingle (SVC_LEDSingleID_t ID){
	uint8_t Ofset = ID - 0;

	if (ID < 16U){
		ledGroup1 |= (1U << Ofset);
		GroupID[0] = 1U;
		return;
	}


	if (ID < 32U){
		Ofset = ID - 16U;
		ledGroup2 |= (1U << Ofset);
		GroupID[1] = 1U;
		return;
	}


	if (ID < 40U){
		Ofset = ID - 32U;
		ledGroup3 |= (1U << Ofset);
		GroupID[2] = 1U;
		return;
	}
}

static void clearLEDSingle (SVC_LEDSingleID_t ID){
	uint8_t Ofset = ID - 0;

	if (ID < 16U){
		ledGroup1 &= ~(1U << Ofset);
		GroupID[0] = 1U;
		return;
	}


	if (ID < 32U){
		Ofset = ID - 16U;
		ledGroup2 &= ~(1U << Ofset);
		GroupID[1] = 1U;
		return;
	}


	if (ID < 40U){
		Ofset = ID - 32U;
		ledGroup3 &= ~(1U << Ofset);
		GroupID[2] = 1U;
		return;
	}
}

static void BlinkLEDSingle (SVC_LEDSingleID_t ID){
	if (s_SLED[ID].EnaBlink == false){
		svc_Timer_set_TZero(&s_SLED[ID].TimerBlink);
		return;
	}

	if (svc_Timer_DeltaT_ms(s_SLED[ID].TimerBlink) < TIME_ON){
		setLEDSingle(ID);
		return;
	}

	if (svc_Timer_DeltaT_ms(s_SLED[ID].TimerBlink) < TIME_ON + TIME_OFF){
		clearLEDSingle(ID);
		return;
	}

	svc_Timer_set_TZero(&s_SLED[ID].TimerBlink);
}

static void CommitGroups_SingleLED(void){
	uint8_t buf[2];

	if (GroupID[0] != 0){
		buf[0] = (uint8_t)(ledGroup1 >> 8);
		buf[1] = (uint8_t)ledGroup1;

		bsp_LED595_SpiTransmit(buf, 2U);
		bsp_LED595_PulseLatch(BSP_BLOCK_ID_SINGLED1);

		GroupID[0] = 0;
	}

	if (GroupID[1] != 0){
		buf[0] = (uint8_t)(ledGroup2 >> 8);
		buf[1] = (uint8_t)ledGroup2;

		bsp_LED595_SpiTransmit(buf, 2U);
		bsp_LED595_PulseLatch(BSP_BLOCK_ID_SINGLED2);

		GroupID[1] = 0;
	}

	if (GroupID[2] != 0){
		bsp_LED595_SpiTransmit(&ledGroup3, 1U);
		bsp_LED595_PulseLatch(BSP_BLOCK_ID_SINGLED3);

		GroupID[2] = 0;
	}
}

static bool SkipSingleLED(SVC_LEDSingleID_t ID){
	switch ((uint8_t)ID){
		case (uint8_t)SLEDID_Unused_0:
		case (uint8_t)SLEDID_Unused_1:
		case (uint8_t)SLEDID_Unused_2:
		case (uint8_t)SLEDID_Unused_3:
		case (uint8_t)SLEDID_Unused_4:
		case (uint8_t)SLEDID_Unused_5:
		case (uint8_t)SLEDID_Unused_6:
		case (uint8_t)SLEDID_Unused_7:
		case (uint8_t)SLEDID_Unused_8:
		case (uint8_t)SLEDID_Unused_9:
			return true;
		break;
		default:
			return false;
		break;
	}
}

void svc_LEDSingle_TurnOn(SVC_LEDSingleID_t ID){
	s_SLED[ID].StateLED = STATE_LEDID_ON;
}

void svc_LEDSingle_TurnOFF(SVC_LEDSingleID_t ID){
	s_SLED[ID].StateLED = STATE_LEDID_OFF;
	s_SLED[ID].EnaBlink = false;
}

void svc_LEDSingle_Blink(SVC_LEDSingleID_t ID){
	s_SLED[ID].StateLED = STATE_LEDID_BLINK;
	s_SLED[ID].EnaBlink = true;
}

void svc_LEDSingle_Init(void){
	for (uint8_t ID = 0; ID < SLEDID_COUNT; ID++){
		s_SLED[ID].ID = ID;
		svc_LEDSingle_TurnOFF(ID);
	}
}

void svc_LEDSingle_Reffresh(void){
	for (uint8_t ID = 0; ID < SLEDID_COUNT; ID++){
		if (SkipSingleLED(ID) == true){
			continue;
		}

		if (s_SLED[ID].EnaBlink == true){
			BlinkLEDSingle(ID);
		} else {
			svc_Timer_set_TZero(&s_SLED[ID].TimerBlink);
			if (s_SLED[ID].StateLED == STATE_LEDID_OFF){
				clearLEDSingle(ID);
			} else {
				setLEDSingle(ID);
			}
		}

		CommitGroups_SingleLED();
	}
}
