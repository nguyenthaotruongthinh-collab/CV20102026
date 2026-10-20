#include "svc_MenuTechInteraction.h"

static ListMenuItem_t *s_PresentChildTech;

// Mapping label value
static const char s_MappingValueTank[TANK_ID_COUNT][8] = {
		[TANK_ID_AUTO] 		= "A U T O",
		[TANK_ID_OFF] 		= "  O F F",
		[TANK_ID_ON] 		= "    O N"
};

static const char s_MappingValuePShape[PSHAPE_ID_COUNT][8] = {
		[PSHAPE_SQUARE] 	= "A D V S",
		[PSHAPE_TRIANGLE] 	= "  T R I",
		[PSHAPE_SINE] 		= "S I N E",
};

static const char s_MappingValueSYNC[SYNC_ID_COUNT][8] = {
		[SYNC_OFF] 		= "  O F F",
		[SYNC_TWIN] 	= "T W I N",
		[SYNC_GSYC] 	= "G S Y C"
};

static const char s_MappingValueGSYC[GSYC_ID_COUNT][8] = {
		[GSYC_MSTR]	= "M S T R",
		[GSYC_SLAV]	= "S L A V"
};

static const ListMenuItem_t MenuTechInit[MENULV2_ID_FINAL_TECH - MENULV2_ID_START_TECH] = {
	[MENULV2_ID_OPEN_TECH - MENULV2_ID_START_TECH] = {.IDHome = MENULV2_ID_OPEN_TECH, .IDNext = MENULV2_ID_TANKPOWER,
			.data = {.Min = 0, .Max = 0, .Def = 0, .Step = 1,
					.Name = "T E C H", .IsSpecialLeft = true, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_OPEN_TECH, .Unit = " "}
	},
	[MENULV2_ID_TANKPOWER - MENULV2_ID_START_TECH] = {.IDHome = MENULV2_ID_OPEN_TECH, .IDNext = MENULV2_ID_PULSER_SHAPE,
			.data = {.Min = TANK_ID + 1, .Max = TANK_ID_COUNT - 1, .Def = TANK_ID_OFF, .Step = 1,
					.Name = "T A N K", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_TANKPOWER, .Unit = " "}
	},
	[MENULV2_ID_PULSER_SHAPE - MENULV2_ID_START_TECH] = {.IDHome = MENULV2_ID_OPEN_TECH, .IDNext = MENULV2_ID_SYNC,
			.data = {.Min = PSHAPE_ID + 1, .Max = PSHAPE_ID_COUNT - 1, .Def = PSHAPE_SQUARE, .Step = 1,
					.Name = "D P  S P", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_PULSER_SHAPE, .Unit = " "}
	},
	[MENULV2_ID_SYNC - MENULV2_ID_START_TECH] = {.IDHome = MENULV2_ID_OPEN_TECH, .IDNext = MENULV2_ID_GSYC,
			.data = {.Min = SYNC_ID + 1, .Max = SYNC_ID_COUNT - 1, .Def = SYNC_OFF, .Step = 1,
					.Name = "S Y N C", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_SYNC, .Unit = " "}
	},
	[MENULV2_ID_GSYC - MENULV2_ID_START_TECH] = {.IDHome = MENULV2_ID_OPEN_TECH, .IDNext = MENULV2_ID_TANKPOWER,
			.data = {.Min = GSYC_ID + 1, .Max = GSYC_ID_COUNT - 1, .Def = GSYC_MSTR, .Step = 1,
					.Name = "G S Y C", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_GSYC, .Unit = " "}
	}
};

static void DisplayHeaderMenu(void){
	if (s_PresentChildTech->data.ID != MENULV2_ID_OPEN_TECH){return;}

	svc_LED14Seg_ShowString("T E C H", Alignment_Left);
	svc_LED14Seg_ShowString("M E N U", Alignment_Right);
}

static void DisplayNameChild(void){
	if (s_PresentChildTech->data.IsSpecialLeft == false){
		svc_LED14Seg_ShowString(s_PresentChildTech->data.Name, Alignment_Left);
		return;
	}

	DisplayHeaderMenu();
}

static void DisplayValueChild(void){
	if (s_PresentChildTech->data.IsSpecialRight == false){
		svc_LED14Seg_ShowNumber(*s_PresentChildTech->data.pValue, Alignment_Right, s_PresentChildTech->data.PointID);

		if (*s_PresentChildTech->data.pValue < 999){
			svc_LED14Seg_ShowUint(s_PresentChildTech->data.Unit, Alignment_Right);
		}
		return;
	}

	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildTech->data.ID, MENULV2_ID_TANKPOWER, s_MappingValueTank);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildTech->data.ID, MENULV2_ID_PULSER_SHAPE, s_MappingValuePShape);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildTech->data.ID, MENULV2_ID_SYNC, s_MappingValueSYNC);
	mid_ManagerMenu_DisplayValueSpecial(s_PresentChildTech->data.ID, MENULV2_ID_GSYC, s_MappingValueGSYC);
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

void svc_MenuTechInteraction(void){
	if (mid_ManagerMenu_getMenuLV1() != MENULV1_ID_TECH){
		mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_TECH, &s_PresentChildTech);

		return;
	}

	mid_ManagerMenu_NavigationChild(&s_PresentChildTech);

	DisplayChild();

	mid_ManagerMenu_AdjustParamterByEncoder(&s_PresentChildTech);
}

void svc_MenuTechInital (void){
	for (uint16_t id = MENULV2_ID_START_TECH; id < MENULV2_ID_FINAL_TECH; id++){
		mid_ManagerMenu_setItem(id, MenuTechInit[id - MENULV2_ID_START_TECH]);
	}

	mid_ManagerMenu_getAdressItem(MENULV2_ID_OPEN_TECH, &s_PresentChildTech);
}
