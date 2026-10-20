#include "svc_MenuUserInteraction.h"

static ListMenuItem_t *s_PresentChildUser;

// Mapping label value
static const char s_MappingValueAuto[AUTO_ID_COUNT][8] = {
		[AUTO] 		= "A U T O",
		[MANUAL] 	= "M A N U",
};

static const char s_MappingValueSTRP[STRP_ID_COUNT][8] = {
		[STRP_EN] 	= "    E N",
		[STRP_EP] 	= "    E P",
};

static const char s_MappingValueTUNG[TUNG_ID_COUNT][8] = {
		[TUNG_OFF] 	= "  O F F",
		[TUNG_05] 	= "    0.5",
		[TUNG_10] 	= "    1.0",
		[TUNG_16] 	= "    1.6",
		[TUNG_24] 	= "    2.4",
		[TUNG_32] 	= "    3.2",
		[TUNG_40] 	= "    4.0",
		[TUNG_48] 	= "    4.8",
		[TUNG_60] 	= "    0.6",
		[TUNG_ALL] 	= "  A L L"
};

static const char s_MappingValueSpecialMode[SPECIALMODE_COUNT][8] = {
		[SPECIALMODE_OFF]	= "  O F F",
		[SPECIALMODE_T2]	= "  T - 2",
		[SPECIALMODE_T3]	= "  T - 3",
		[SPECIALMODE_T4]	= "  T - 4"
};

static const char s_MappingValueRaitoENEP[RAITO_ENEP_COUNT][8] = {
		[RAITO_ENEP_SAME] 			= "S A M E",
		[RAITO_ENEP_INDEPENDENT] 	= "I N D P",
};

static const char s_MappingValueCOMA[COMA_ID_COUNT][8] = {
		[COMA_OFF]		= "  O F F",
		[COMA_1]		= "O N - 1",
		[COMA_2]		= "O N - 2"
};

static const char s_MappingValueOnOff[ONOFF_ID_COUNT][8] = {
		[ID_OFF]	= "  O F F",
		[ID_ON] 	= "    O N"
};

static const char s_MappingValueYesNo[YESNO_ID_COUNT][8] = {
		[ID_NO]		= "    N O",
		[ID_YES] 	= "  Y E S"
};

static const ListMenuItem_t MenuUserInit[MENULV2_ID_FINAL_USER - MENULV2_ID_START_USER] = {
	[MENULV2_ID_OPEN_USER - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_ARCSTART_AUTO,
			.data = {.Min = 0, .Max = 0, .Def = 0, .Step = 1,
					.Name = "U S E R", .IsSpecialLeft = true, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_OPEN_USER, .Unit = " "}
	},
	[MENULV2_ID_ARCSTART_AUTO - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_STRP_DC,
			.data = {.Min = AUTO_ID + 1, .Max = AUTO_ID_COUNT - 1, .Def = AUTO, .Step = 1,
					.Name = "S E T A", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_ARCSTART_AUTO, .Unit = " "}
	},
	[MENULV2_ID_STRP_DC - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_STRP_AC,
			.data = {.Min = STRP_ID + 1, .Max = STRP_ID_COUNT - 1, .Def = STRP_EN, .Step = 1,
					.Name = "S T R P", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_STRP_DC, .Unit = " "}
	},
	[MENULV2_ID_STRP_AC - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_TUNGSTEN,
			.data = {.Min = STRP_ID + 1, .Max = STRP_ID_COUNT - 1, .Def = STRP_EP, .Step = 1,
					.Name = "S T R P", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_STRP_AC, .Unit = " "}
	},
	[MENULV2_ID_TUNGSTEN - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_HOTC,
			.data = {.Min = TUNG_ID + 1, .Max = TUNG_ID_COUNT - 1, .Def = TUNG_ALL, .Step = 1,
					.Name = "T U N G", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_TUNGSTEN, .Unit = " "}
	},
	[MENULV2_ID_HOTC - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_HOTT,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 100, .Step = 1,
					.Name = "H O T C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_HOTC, .Unit = "A"}
	},
	[MENULV2_ID_HOTT - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_STPC,
			.data = {.Min = 10, .Max = 100, .Def = 50, .Step = 1,
					.Name = "H O T T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_HOTT, .Unit = "m"}
	},
	[MENULV2_ID_STPC - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_TPLT,
			.data = {.Min = 20, .Max = 200, .Def = 50, .Step = 1,
					.Name = "S T P C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_STPC, .Unit = "A"}
	},
	[MENULV2_ID_TPLT - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_SPECIAL_MODE,
			.data = {.Min = 10, .Max = 100, .Def = 30, .Step = 1,
					.Name = "T P L T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_TPLT, .Unit = "m"}
	},
	[MENULV2_ID_SPECIAL_MODE - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_AUTOSPOT_WELDING,
			.data = {.Min = SPECIALMODE_ID + 1, .Max = SPECIALMODE_COUNT - 1, .Def = SPECIALMODE_OFF, .Step = 1,
					.Name = "O U T ", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_SPECIAL_MODE, .Unit = " "}
	},
	[MENULV2_ID_AUTOSPOT_WELDING - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_AUTOSPOT_RELASE,
			.data = {.Min = 1, .Max = 900, .Def = 10, .Step = 1,
					.Name = "T W E L", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_AUTOSPOT_WELDING, .Unit = "m"}
	},
	[MENULV2_ID_AUTOSPOT_RELASE - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_MANUALSPOT_TIME,
			.data = {.Min = 1, .Max = 900, .Def = 10, .Step = 1,
					.Name = "T O F F", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_AUTOSPOT_RELASE, .Unit = "m"}
	},
	[MENULV2_ID_MANUALSPOT_TIME - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_SAME_ENEP,
			.data = {.Min = 1, .Max = 900, .Def = 10, .Step = 1,
					.Name = "S P O T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_MANUALSPOT_TIME, .Unit = "m"}
	},
	[MENULV2_ID_SAME_ENEP - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_COMA,
			.data = {.Min = RAITO_ENEP_ID + 1, .Max = RAITO_ENEP_COUNT - 1, .Def = RAITO_ENEP_SAME, .Step = 1,
					.Name = "E N E P", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_SAME_ENEP, .Unit = " "}
	},
	[MENULV2_ID_COMA - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_STIK,
			.data = {.Min = COMA_ID + 1, .Max = COMA_ID_COUNT - 1, .Def = COMA_OFF, .Step = 1,
					.Name = "C O M.A", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_COMA, .Unit = " "}
	},
	[MENULV2_ID_STIK - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_LIFT,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "S T I K", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_STIK, .Unit = " "}
	},
	[MENULV2_ID_LIFT - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_VRD,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "L I F T", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_LIFT, .Unit = " "}
	},
	[MENULV2_ID_VRD - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_STA,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "V R D", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_VRD, .Unit = " "}
	},
	[MENULV2_ID_STA - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_HU0,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_ON, .Step = 1,
					.Name = "S T A", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_STA, .Unit = " "}
	},
	[MENULV2_ID_HU0 - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_HFC,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_ON, .Step = 1,
					.Name = "H U 0", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_HU0, .Unit = " "}
	},
	[MENULV2_ID_HFC - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_DEF,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_ON, .Step = 1,
					.Name = "H F C", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_HFC, .Unit = " "}
	},
	[MENULV2_ID_DEF - MENULV2_ID_START_USER] = {.IDHome = MENULV2_ID_OPEN_USER, .IDNext = MENULV2_ID_ARCSTART_AUTO,
			.data = {.Min = YESNO_ID + 1, .Max = YESNO_ID_COUNT - 1, .Def = ID_NO, .Step = 1,
					.Name = "D E F C", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_DEF, .Unit = " "}
	}
};

static void DisplayHeaderMenu(void){
	if (s_PresentChildUser->data.ID != MENULV2_ID_OPEN_USER){return;}

	svc_LED14Seg_ShowString("U S E R", Alignment_Left);
	svc_LED14Seg_ShowString("M E N U", Alignment_Right);
}

static void DisplayNameChild(void){
	if (s_PresentChildUser->data.IsSpecialLeft == false){
		svc_LED14Seg_ShowString(s_PresentChildUser->data.Name, Alignment_Left);
		return;
	}

	DisplayHeaderMenu();
}

static void DisplayValueChild(void){
	if (s_PresentChildUser->data.IsSpecialRight == false){
		svc_LED14Seg_ShowNumber(*s_PresentChildUser->data.pValue, Alignment_Right, s_PresentChildUser->data.PointID);

		if (*s_PresentChildUser->data.pValue < 999){
			svc_LED14Seg_ShowUint(s_PresentChildUser->data.Unit, Alignment_Right);
		}
		return;
	}

	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_ARCSTART_AUTO, s_MappingValueAuto);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_STRP_DC, s_MappingValueSTRP);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_STRP_AC, s_MappingValueSTRP);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_TUNGSTEN, s_MappingValueTUNG);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_SPECIAL_MODE, s_MappingValueSpecialMode);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_SAME_ENEP, s_MappingValueRaitoENEP);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_COMA, s_MappingValueCOMA);

	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_STIK, s_MappingValueOnOff);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_LIFT, s_MappingValueOnOff);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_VRD, s_MappingValueOnOff);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_HFC, s_MappingValueOnOff);

	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildUser->data.ID, MENULV2_ID_DEF, s_MappingValueYesNo);
}

static void DisplaySingleLED(void){
	for (uint16_t IDSLed = 0; IDSLed < SLEDID_COUNT; IDSLed++){
		svc_LEDSingle_TurnOFF(IDSLed);
	}
}

static void DisplayChild(void){
	DisplayNameChild();
	DisplayValueChild();
	DisplaySingleLED();
}

void svc_MenuUserInteraction(void){
	if (mid_ManagerMenu_getMenuLV1() != MENULV1_ID_USER){
		mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_USER, &s_PresentChildUser);

		return;
	}

	mid_ManagerMenu_NavigationChild(&s_PresentChildUser);

	DisplayChild();

	mid_ManagerMenu_AdjustParamterByEncoder(&s_PresentChildUser);
}

void svc_MenuUserInital (void){
	for (uint16_t id = MENULV2_ID_START_USER; id < MENULV2_ID_FINAL_USER; id++){
		mid_ManagerMenu_setItem(id, MenuUserInit[id - MENULV2_ID_START_USER]);
	}

	mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_USER, &s_PresentChildUser);
}
