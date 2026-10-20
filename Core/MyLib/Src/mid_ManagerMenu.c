#include "mid_ManagerMenu.h"

static ListMenuItem_t s_MenuItem[MENULV2_ID_COUNT];
static MIDMenuLV2Id_t s_PresentMenuLV2 = MENULV2_ID_AMPER;
static ParamItem_t s_ParamItem;
static MIDMenuLV1Id_t s_PresentMenuLV1 = MENULV1_ID_STARTUP;

static bool HiddenHeaderMode(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_MODE){return false;}

	if (s_ParamItem.SPECIAL_MODE != SPECIALMODE_OFF){
		return true;
	}

	return false;
}

static bool HiddenHeaderPulse(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_PULSE){return false;}

	if (s_ParamItem.PULSE_ENABLE != ID_ON || s_ParamItem.PROCESS == PROCESS_MMA){
		return true;
	}

	return false;
}

static bool HiddenHeaderSequence(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_SEQUENCE){return false;}

	if (s_ParamItem.PROCESS != PROCESS_TIG){
		return true;
	}

	return false;
}

static bool HiddenHeaderForce(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_FORCE){return false;}

	if (s_ParamItem.PROCESS != PROCESS_MMA){
		return true;
	}

	return false;
}

static bool HiddenHeaderGas(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_GAS){return false;}

	if (s_ParamItem.PROCESS != PROCESS_TIG){
		return true;
	}

	return false;
}

static bool HiddenHeaderACShape(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_AC_SHAPE){return false;}

	if (s_ParamItem.POLARITY == POLARITY_DC){return true;}

	if (s_ParamItem.PROCESS == PROCESS_MMA){
		s_ParamItem.AC_SHAPE = AC_SINE;
		return true;
	}

	return false;
}

static bool HiddenHeaderACPrameter(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_AC_PARAMETER){return false;}

	if (s_ParamItem.POLARITY == POLARITY_DC || s_ParamItem.PROCESS == PROCESS_MMA){return true;}

	return false;
}

static bool HiddenHeaderExternal(MIDMenuHeaderId_t PresentHeaderID){
	if (PresentHeaderID != HEADER_ID_EXTERNAL){return false;}

	if (IsOption == true){
		return true;
	}

	return false;
}

static void HiddenItemSequence(void){
	if (s_ParamItem.SPECIAL_MODE != SPECIALMODE_OFF ) {
		s_MenuItem[MENULV2_ID_SEQUENCE_START].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_SEQUENCE_FINAL].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.MODE != MODE_HOLD) {
		s_MenuItem[MENULV2_ID_SEQUENCE_START].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_SEQUENCE_FINAL].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_SEQUENCE_START].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_SEQUENCE_FINAL].data.IsHidden = false;
}

static void HiddenItemARCStartAuto(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_ARCSTART_AUTO].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_ARCSTART_AUTO].data.IsHidden = false;
}

static void HiddenItemSTRP_DC(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_STRP_DC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.POLARITY == POLARITY_AC) {
		s_MenuItem[MENULV2_ID_STRP_DC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_STRP_DC].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_STRP_DC].data.IsHidden = false;
}

static void HiddenItemSTRP_AC(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_STRP_AC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.POLARITY == POLARITY_DC) {
		s_MenuItem[MENULV2_ID_STRP_AC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_STRP_AC].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_STRP_AC].data.IsHidden = false;
}

static void HiddenItemTUNG(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_TUNGSTEN].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_TUNGSTEN].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_TUNGSTEN].data.IsHidden = false;
}

static void HiddenItemHOT(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_HOTT].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_HOTC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_HOTT].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_HOTC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.TUNGSTEN != TUNG_OFF) {
		s_MenuItem[MENULV2_ID_HOTT].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_HOTC].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_HOTT].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_HOTC].data.IsHidden = false;
}

static void HiddenItemSTPC(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_STPC].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_STPC].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_STPC].data.IsHidden = false;
}

static void HiddenItemTPLT(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_TPLT].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.ARCSTART_AUTO == AUTO) {
		s_MenuItem[MENULV2_ID_TPLT].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_TPLT].data.IsHidden = false;
}

static void HiddenItemSpecialMode(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_SPECIAL_MODE].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.LIFT != ID_OFF) {
		s_MenuItem[MENULV2_ID_SPECIAL_MODE].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_SPECIAL_MODE].data.IsHidden = false;
}

static void HiddenItemAutoSpot(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_AUTOSPOT_WELDING].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_AUTOSPOT_RELASE].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.SPECIAL_MODE != SPECIALMODE_T2) {
		s_MenuItem[MENULV2_ID_AUTOSPOT_WELDING].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_AUTOSPOT_RELASE].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_AUTOSPOT_WELDING].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_AUTOSPOT_RELASE].data.IsHidden = false;
}

static void HiddenItemManualSpot(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_MANUALSPOT_TIME].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.SPECIAL_MODE != SPECIALMODE_T3) {
		s_MenuItem[MENULV2_ID_MANUALSPOT_TIME].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_MANUALSPOT_TIME].data.IsHidden = false;
}

static void HiddenItemAC(void){
	if (s_ParamItem.PROCESS == PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_SAME_ENEP].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_COMA].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.POLARITY != POLARITY_AC) {
		s_MenuItem[MENULV2_ID_SAME_ENEP].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_COMA].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_SAME_ENEP].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_COMA].data.IsHidden = false;
}

static void HiddenItemMMA(void){
	if (s_ParamItem.PROCESS != PROCESS_MMA) {
		s_MenuItem[MENULV2_ID_STIK].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_VRD].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_STIK].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_VRD].data.IsHidden = false;
}

static void HiddenItemTIG(void){
	if (s_ParamItem.PROCESS != PROCESS_TIG) {
		s_MenuItem[MENULV2_ID_LIFT].data.IsHidden = true;
		return;
	}

	if (s_ParamItem.SPECIAL_MODE != SPECIALMODE_OFF) {
		s_MenuItem[MENULV2_ID_LIFT].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_LIFT].data.IsHidden = false;
}

static void HiddenItemTIGControl(void){
	if (s_ParamItem.PROCESS != PROCESS_TIG) {
		s_MenuItem[MENULV2_ID_STA].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_HU0].data.IsHidden = true;
		s_MenuItem[MENULV2_ID_HFC].data.IsHidden = true;
		return;
	}

	s_MenuItem[MENULV2_ID_STA].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_HU0].data.IsHidden = false;
	s_MenuItem[MENULV2_ID_HFC].data.IsHidden = false;
}

static void UpdateParamRaito(void){
	if (s_ParamItem.SAME_ENEP == RAITO_ENEP_SAME){
		s_ParamItem.AC_PARAM_K_ENEP = 100;
	} else {
		s_ParamItem.AC_PARAM_K_ENEP = (s_ParamItem.AC_PARAM_EN*100)/s_ParamItem.AC_PARAM_EP;
	}
}

void mid_ManagerMenu_HiddenItem(void){
	UpdateParamRaito();
	if (s_PresentMenuLV1 == MENULV1_ID_FRONT){
		HiddenItemSequence();
	}

	if (s_PresentMenuLV1 == MENULV1_ID_USER){
		HiddenItemARCStartAuto();
		HiddenItemSTRP_DC();
		HiddenItemSTRP_AC();
		HiddenItemTUNG();
		HiddenItemHOT();
		HiddenItemSTPC();
		HiddenItemTPLT();
		HiddenItemSpecialMode();
		HiddenItemAutoSpot();
		HiddenItemManualSpot();
		HiddenItemAC();
		HiddenItemMMA();
		HiddenItemTIG();
		HiddenItemTIGControl();
	}

}

bool mid_ManagerMenu_HiddenHeader(MIDMenuHeaderId_t PresentHeaderID){
	if (HiddenHeaderMode(PresentHeaderID) == true){return true;}
	if (HiddenHeaderPulse(PresentHeaderID) == true){return true;}
	if (HiddenHeaderSequence(PresentHeaderID) == true){return true;}
	if (HiddenHeaderForce(PresentHeaderID) == true){return true;}
	if (HiddenHeaderGas(PresentHeaderID) == true){return true;}
	if (HiddenHeaderACShape(PresentHeaderID) == true){return true;}
	if (HiddenHeaderACPrameter(PresentHeaderID) == true){return true;}
	if (HiddenHeaderExternal(PresentHeaderID) == true){return true;}

	return false;
}

void mid_ManagerMenu_LimitValue(int32_t *Value, uint16_t Min, uint16_t Max){
	if(Min > Max){
		uint16_t Temp = Min;
		Min = Max;
		Max = Temp;
	}

	if (*Value > Max){
		*Value = Max;
		return;
	}

	if (*Value < Min){
		*Value = Min;
		return;
	}
}

void mid_ManagerMenu_RoundRobin(int32_t *Value, uint16_t Min, uint16_t Max){
	if(Min > Max){
		uint16_t Temp = Min;
		Min = Max;
		Max = Temp;
	}

	if (*Value > Max){
		*Value = Min;
		return;
	}

	if (*Value < Min){
		*Value = Max;
		return;
	}
}

void mid_ManagerMenu_AdjustParamterByEncoder (ListMenuItem_t **Itemp){
	static MIDMenuLV2Id_t PreviousID = MENULV2_ID_AMPER;
	uint16_t Step = (*Itemp)->data.Step;
	int32_t Temp = *(*Itemp)->data.pValue;
	bool IsBooth  = (*Itemp)->data.AllowAccel;
	int16_t Raw = svc_Encoder_getRawCount();
	static uint32_t Timer_ms = 0;
	static int32_t Counter = 0;



	if (PreviousID != (*Itemp)->data.ID){
		PreviousID = (*Itemp)->data.ID;
		svc_Timer_set_TZero(&Timer_ms);
		Counter = 0;
	}

	if (IsBooth == false){
		svc_Timer_set_TZero(&Timer_ms);
		Counter = 0;
	}

	Counter += Raw;

	if (svc_Timer_DeltaT_ms(Timer_ms) < 100){
		if (Counter > 5 || Counter < -5){
			Raw = Raw*10;
		}

		if (Counter > 10 || Counter < -10){
			Raw = Raw*100;
		}

		if (Counter > 15 || Counter < -15){
			Raw = Raw*200;
		}
	} else {
		Counter = 0;
		svc_Timer_set_TZero(&Timer_ms);
	}

	Temp += Raw*Step;

	if (Temp <= 0){
		Temp = 0;
	}
	mid_ManagerMenu_LimitValue(&Temp, (*Itemp)->data.Min, (*Itemp)->data.Max);

	*(*Itemp)->data.pValue = Temp;
}

void mid_ManagerMenu_NavigationChild (ListMenuItem_t **OutChild){
	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(BSP_BTN_ID_Gas);
	static SVC_BTN_STATE_t PreviousBTN = SVC_BTN_STATE_RELEASED;

	bool SkipMenu = false;

	if (BTN != SVC_BTN_STATE_RELEASED){
		PreviousBTN = BTN;
	} else if (PreviousBTN != SVC_BTN_STATE_RELEASED){
		PreviousBTN = BTN;
		do {
			mid_ManagerMenu_getAdressItem((*OutChild)->IDNext, OutChild);
			SkipMenu = (*OutChild)->data.IsHidden;
		} while (SkipMenu == true);
	}
}

void mid_ManagerMenu_DisplayValueSpecial(MIDMenuLV2Id_t PresentID, MIDMenuLV2Id_t ID, const char Mapping[][8]){
	if (PresentID != ID){return;}

	char Display[8];
	memcpy(Display, Mapping[s_ParamItem.arr[PresentID]], sizeof(Display));

	svc_LED14Seg_ShowString(Display, Alignment_Right);
}

void mid_ManagerMenu_setMenuLV1 (MIDMenuLV1Id_t ID){
	s_PresentMenuLV1 = ID;
}

MIDMenuLV1Id_t mid_ManagerMenu_getMenuLV1 (void){
	return s_PresentMenuLV1;
}

void mid_ManagerMenu_setItem (MIDMenuLV2Id_t ID, ListMenuItem_t Input){
	s_MenuItem[ID] = Input;
	s_MenuItem[ID].data.pValue = &s_ParamItem.arr[ID];
	s_ParamItem.arr[ID] = s_MenuItem[ID].data.Def;
}

ListMenuItem_t mid_ManagerMenu_getItem (MIDMenuLV2Id_t ID){
	return s_MenuItem[ID];
}

void mid_ManagerMenu_getAdressItem (MIDMenuLV2Id_t ID, ListMenuItem_t **Output){
	*Output = &s_MenuItem[ID];
}

uint16_t mid_ManagerMenu_getParamItem (MIDMenuLV2Id_t ID){
	return s_ParamItem.arr[ID];
}

bool mid_ManagerMenu_getHiddenItem (MIDMenuLV2Id_t ID){
	return s_MenuItem[ID].data.IsHidden;
}

void mid_ManagerMenu_setHiddenItem (MIDMenuLV2Id_t ID){
	s_MenuItem[ID].data.IsHidden = true;
}

void mid_ManagerMenu_clearHiddenItem (MIDMenuLV2Id_t ID){
	s_MenuItem[ID].data.IsHidden = false;
}

void mid_ManagerMenu_setParamItem (MIDMenuLV2Id_t ID, uint16_t Input){
	s_ParamItem.arr[ID] = Input;
}

void mid_ManagerMenu_setMenuLV2 (MIDMenuLV2Id_t ID){
	s_PresentMenuLV2 = ID;
}

MIDMenuLV2Id_t mid_ManagerMenu_getMenuLV2 (void){
	return s_PresentMenuLV2;
}

uint16_t *mid_ManagerMenu_getAddressParam(MIDMenuLV2Id_t ID){
	return &s_ParamItem.arr[ID];
}
