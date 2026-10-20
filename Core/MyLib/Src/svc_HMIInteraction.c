#include "svc_HMIInteraction.h"

static SVC_BTN_STATE_t ComboGasAmper (void){
	SVC_BTN_STATE_t BTNGas = svc_Button_get_StateBtn(BSP_BTN_ID_Gas);
	SVC_BTN_STATE_t BTNAmper = svc_Button_get_StateBtn(BSP_BTN_ID_Amper);

	if(BTNGas  == SVC_BTN_STATE_RELEASED || BTNAmper  == SVC_BTN_STATE_RELEASED){
		return SVC_BTN_STATE_RELEASED;
	}

	if(BTNGas  == SVC_BTN_STATE_HOLD && BTNAmper  == SVC_BTN_STATE_HOLD){
		return SVC_BTN_STATE_HOLD;
	}

	return SVC_BTN_STATE_PRESSED;
}

static SVC_BTN_STATE_t ComboProcessGasACParameter (void){
	SVC_BTN_STATE_t BTNProcess = svc_Button_get_StateBtn(BSP_BTN_ID_Process);
	SVC_BTN_STATE_t BTNGas = svc_Button_get_StateBtn(BSP_BTN_ID_Gas);
	SVC_BTN_STATE_t BTNACPrameter = svc_Button_get_StateBtn(BSP_BTN_ID_ACParameter);

	if(BTNProcess  == SVC_BTN_STATE_HOLD && BTNGas  == SVC_BTN_STATE_HOLD && BTNACPrameter  == SVC_BTN_STATE_HOLD){
		return SVC_BTN_STATE_HOLD;
	}

	return SVC_BTN_STATE_RELEASED;
}

static SVC_BTN_STATE_t ComboProcessAmperSequence(void){
	SVC_BTN_STATE_t BTNAmper = svc_Button_get_StateBtn(BSP_BTN_ID_Amper);
	SVC_BTN_STATE_t BTNSequence = svc_Button_get_StateBtn(BSP_BTN_ID_Sequence);

	if(BTNAmper  == SVC_BTN_STATE_HOLD && BTNSequence  == SVC_BTN_STATE_HOLD){
		return SVC_BTN_STATE_HOLD;
	}

	return SVC_BTN_STATE_RELEASED;
}

static void NavigationMenuLV1 (void){
	MIDMenuLV1Id_t PresentIDLV1 = mid_ManagerMenu_getMenuLV1();

	static uint32_t TimerStartUp = 0;

	static MIDMenuLV1Id_t PreviousIDLV1 = MENULV1_ID_STARTUP;

	static SVC_BTN_STATE_t Previous_Combo1 = SVC_BTN_STATE_RELEASED;
	static SVC_BTN_STATE_t Previous_Combo2 = SVC_BTN_STATE_RELEASED;
	static SVC_BTN_STATE_t Previous_Combo3 = SVC_BTN_STATE_RELEASED;

	if (Previous_Combo1 == SVC_BTN_STATE_RELEASED &&
		Previous_Combo2 == SVC_BTN_STATE_RELEASED &&
		Previous_Combo3 == SVC_BTN_STATE_RELEASED){
		PreviousIDLV1 = PresentIDLV1;
	}

	if (ComboGasAmper() != SVC_BTN_STATE_RELEASED){
		Previous_Combo1 = ComboGasAmper();
	} else if (Previous_Combo1 != SVC_BTN_STATE_RELEASED){
		Previous_Combo1 = SVC_BTN_STATE_RELEASED;
		PreviousIDLV1 = PresentIDLV1;
	}

	if (ComboProcessGasACParameter() != SVC_BTN_STATE_RELEASED){
		Previous_Combo2 = ComboProcessGasACParameter();
	} else if (Previous_Combo2 != SVC_BTN_STATE_RELEASED){
		Previous_Combo2 = SVC_BTN_STATE_RELEASED;
		PreviousIDLV1 = PresentIDLV1;
	}

	if (ComboProcessAmperSequence() != SVC_BTN_STATE_RELEASED){
		Previous_Combo3 = ComboProcessAmperSequence();
	} else if (Previous_Combo3 != SVC_BTN_STATE_RELEASED){
		Previous_Combo3 = SVC_BTN_STATE_RELEASED;
		PreviousIDLV1 = PresentIDLV1;
	}

	switch ((uint16_t)PreviousIDLV1){
		case (uint16_t)MENULV1_ID_STARTUP:
			if (svc_Timer_DeltaT_s(TimerStartUp) >= 5){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_FRONT);
			}
		break;
		case (uint16_t)MENULV1_ID_FRONT:
			if (Previous_Combo1 == SVC_BTN_STATE_PRESSED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_USER);
			}

			if (Previous_Combo1 == SVC_BTN_STATE_HOLD){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_TECH);
			}

			if (Previous_Combo2 != SVC_BTN_STATE_RELEASED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_HIDDEN);
			}

			if (Previous_Combo3 != SVC_BTN_STATE_RELEASED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_ARC);
			}
		break;
		case (uint16_t)MENULV1_ID_USER:
			if (Previous_Combo1 == SVC_BTN_STATE_PRESSED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_FRONT);
			}

			if (Previous_Combo1 == SVC_BTN_STATE_HOLD){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_TECH);
			}
		break;
		case (uint16_t)MENULV1_ID_TECH:
			if (Previous_Combo1 != SVC_BTN_STATE_RELEASED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_FRONT);
			}
		break;
		case (uint16_t)MENULV1_ID_HIDDEN:
			if (Previous_Combo1 != SVC_BTN_STATE_RELEASED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_FRONT);
			}
		break;
		case (uint16_t)MENULV1_ID_ARC:
			if (Previous_Combo1 != SVC_BTN_STATE_RELEASED){
				mid_ManagerMenu_setMenuLV1(MENULV1_ID_FRONT);
			}
		break;
		default:

		break;
	}
}

void svc_HMIInteraction(void){
	NavigationMenuLV1();
}

void svc_HMIInital (void){
}
