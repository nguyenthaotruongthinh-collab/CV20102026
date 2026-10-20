#include "svc_MenuHiddenInteraction.h"

static ListMenuItem_t *s_PresentChildHidden;

static const char s_MappingValueSignalHF[SIGNAL_HF_ID_COUNT][8] = {
		[SIGNAL_HF_HIGH]	= "H I G H",
		[SIGNAL_HF_PWM] 	= "  P W M",
		[SIGNAL_HF_LOW]		= "  L O W"
};

static const char s_MappingValueOnOff[ONOFF_ID_COUNT][8] = {
		[ID_OFF]	= "  O F F",
		[ID_ON] 	= "    O N"
};

static const char s_MappingValueYesNo[YESNO_ID_COUNT][8] = {
		[ID_NO]		= "    N O",
		[ID_YES] 	= "  Y E S"
};

static const ListMenuItem_t MenuHiddenInit[MENULV2_ID_FINAL_HIDDEN - MENULV2_ID_START_HIDDEN] = {
	[MENULV2_ID_OPEN_HIDDEN - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_HULT,
			.data = {.Min = 0, .Max = 0, .Def = 0, .Step = 1,
					.Name = "HIDDEN", .IsSpecialLeft = true, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_OPEN_HIDDEN, .Unit = " "}
	},
	[MENULV2_ID_HULT - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_SIGNAL_HF,
			.data = {.Min = 0, .Max = 100, .Def = 9, .Step = 1,
					.Name = "H U L T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_HULT, .Unit = "m"}
	},
	[MENULV2_ID_SIGNAL_HF - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_FREQUENCE_HF,
			.data = {.Min = SIGNAL_HF_ID + 1, .Max = SIGNAL_HF_ID_COUNT - 1, .Def = SIGNAL_HF_PWM, .Step = 1,
					.Name = "S I H F", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_SIGNAL_HF, .Unit = " "}
	},
	[MENULV2_ID_FREQUENCE_HF - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_DUTY_HF,
			.data = {.Min = 5, .Max = 100, .Def = 50, .Step = 1,
					.Name = "F H F", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_FREQUENCE_HF, .Unit = "H"}
	},
	[MENULV2_ID_DUTY_HF - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_PWM_OFF,
			.data = {.Min = 1, .Max = 99, .Def = 50, .Step = 1,
					.Name = "D H F", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_DUTY_HF, .Unit = "%"}
	},
	[MENULV2_ID_PWM_OFF - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_AST,
			.data = {.Min = 0, .Max = 200, .Def = 0, .Step = 1,
					.Name = "P O F T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_PWM_OFF, .Unit = "m"}
	},
	[MENULV2_ID_AST - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_ASS_FOR_SINE_TRIANGLE,
			.data = {.Min = 50, .Max = 300, .Def = 200, .Step = 1,
					.Name = "A S T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_AST, .Unit = "u"}
	},
	[MENULV2_ID_ASS_FOR_SINE_TRIANGLE - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_IFB,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_ON, .Step = 1,
					.Name = "A F S T", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_ASS_FOR_SINE_TRIANGLE, .Unit = " "}
	},
	[MENULV2_ID_D_IFB - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_VFB,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D I F B", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_IFB, .Unit = "%"}
	},
	[MENULV2_ID_D_VFB - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_MMADC,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D V F B", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_VFB, .Unit = "%"}
	},
	[MENULV2_ID_D_MMADC - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_MMAAC,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D M D C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_MMADC, .Unit = "%"}
	},
	[MENULV2_ID_D_MMAAC - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_TIGDC,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D M A C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_MMAAC, .Unit = "%"}
	},
	[MENULV2_ID_D_TIGDC - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_TIGAC_SOFT,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D T D C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_TIGDC, .Unit = "%"}
	},
	[MENULV2_ID_D_TIGAC_SOFT - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_TIGAC_SQUARE,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D S S Q", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_TIGAC_SOFT, .Unit = "%"}
	},
	[MENULV2_ID_D_TIGAC_SQUARE - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_TIGAC_TRIANGLE,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D S Q U", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_TIGAC_SQUARE, .Unit = "%"}
	},
	[MENULV2_ID_D_TIGAC_TRIANGLE - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_D_TIGAC_SINE,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D T R I", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_TIGAC_TRIANGLE, .Unit = "%"}
	},
	[MENULV2_ID_D_TIGAC_SINE - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_DEFAULT,
			.data = {.Min = 80, .Max = 120, .Def = 100, .Step = 1,
					.Name = "D S I N", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_D_TIGAC_SINE, .Unit = "%"}
	},
	[MENULV2_ID_DEFAULT - MENULV2_ID_START_HIDDEN] = {.IDHome = MENULV2_ID_OPEN_HIDDEN, .IDNext = MENULV2_ID_HULT,
			.data = {.Min = YESNO_ID + 1, .Max = YESNO_ID_COUNT - 1, .Def = ID_NO, .Step = 1,
					.Name = "D E F A", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_DEFAULT, .Unit = " "}
	}
};

static void DisplayHeaderMenu(void){
	if (s_PresentChildHidden->data.ID != MENULV2_ID_OPEN_HIDDEN){return;}

	svc_LED14Seg_ShowString("H I D E", Alignment_Left);
	svc_LED14Seg_ShowString("M E N U", Alignment_Right);
}

static void DisplayNameChild(void){
	if (s_PresentChildHidden->data.IsSpecialLeft == false){
		svc_LED14Seg_ShowString(s_PresentChildHidden->data.Name, Alignment_Left);
		return;
	}

	DisplayHeaderMenu();
}

static void DisplayValueChild(void){
	if (s_PresentChildHidden->data.IsSpecialRight == false){
		svc_LED14Seg_ShowNumber(*s_PresentChildHidden->data.pValue, Alignment_Right, s_PresentChildHidden->data.PointID);

		if (*s_PresentChildHidden->data.pValue < 999){
			svc_LED14Seg_ShowUint(s_PresentChildHidden->data.Unit, Alignment_Right);
		}
		return;
	}

	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildHidden->data.ID, MENULV2_ID_SIGNAL_HF, s_MappingValueSignalHF);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildHidden->data.ID, MENULV2_ID_ASS_FOR_SINE_TRIANGLE, s_MappingValueOnOff);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildHidden->data.ID, MENULV2_ID_DEFAULT, s_MappingValueYesNo);
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

void svc_MenuHiddenInteraction(void){
	if (mid_ManagerMenu_getMenuLV1() != MENULV1_ID_HIDDEN){
		mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_HIDDEN, &s_PresentChildHidden);

		return;
	}

	mid_ManagerMenu_NavigationChild(&s_PresentChildHidden);

	DisplayChild();

	mid_ManagerMenu_AdjustParamterByEncoder(&s_PresentChildHidden);
}

void svc_MenuHiddenInital (void){
	for (uint16_t id = MENULV2_ID_START_HIDDEN; id < MENULV2_ID_FINAL_HIDDEN; id++){
		mid_ManagerMenu_setItem(id, MenuHiddenInit[id - MENULV2_ID_START_HIDDEN]);
	}

	mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_HIDDEN, &s_PresentChildHidden);
}
