#include "svc_MenuFrontInteraction.h"

#define TIMER_COMEBACK_HOME 3 //3s

static MIDMenuHeaderId_t s_PresentHeader = HEADER_ID_AMPER;
static MIDMenuHeaderId_t s_PreviousHeader = HEADER_ID_AMPER;

static ListMenuItem_t *s_PresentChild;
static uint32_t TimerComeBackHome = 0;
static bool IsBlinkLED = false;

static const BSP_BTN_ID_t s_MappingBTN[HEADER_ID_COUNT] = {
	[HEADER_ID_AMPER] 			= BSP_BTN_ID_Amper,
	[HEADER_ID_PROCESS] 		= BSP_BTN_ID_Process,
	[HEADER_ID_POLARITY] 		= BSP_BTN_ID_Polarity,
	[HEADER_ID_MODE] 			= BSP_BTN_ID_Mode,
	[HEADER_ID_PULSE] 			= BSP_BTN_ID_Pulser,
	[HEADER_ID_SEQUENCE] 		= BSP_BTN_ID_Sequence,
	[HEADER_ID_FORCE] 			= BSP_BTN_ID_Sequence,
	[HEADER_ID_GAS] 			= BSP_BTN_ID_Gas,
	[HEADER_ID_AC_SHAPE] 		= BSP_BTN_ID_ACShape,
	[HEADER_ID_AC_PARAMETER] 	= BSP_BTN_ID_ACParameter,
	[HEADER_ID_EXTERNAL] 		= BSP_BTN_ID_External,
	[HEADER_ID_MEMORY] 			= BSP_BTN_ID_Memory
};

// Mapping Single LED
static const SVC_LEDSingleID_t s_MappingSingleLEDProcess[PROCESS_ID_COUNT] = {
		[PROCESS_MMA] = SLEDID_MMA,
		[PROCESS_TIG] = SLEDID_TIG
};

static const SVC_LEDSingleID_t s_MappingSingleLEDMode[MODE_ID_COUNT] = {
		[MODE_STD] 	= SLEDID_STD,
		[MODE_HOLD] = SLEDID_HOLD
};

static const SVC_LEDSingleID_t s_MappingSingleLEDPolarity[POLARITY_ID_COUNT] = {
		[POLARITY_DC] = SLEDID_DC,
		[POLARITY_AC] = SLEDID_AC
};

static const SVC_LEDSingleID_t s_MappingSingleLEDACShape[AC_SHAPE_ID_COUNT] = {
		[AC_SOFTSQUARE] = SLEDID_SoftSquare,
		[AC_SQUARE] 	= SLEDID_Square,
		[AC_TRIANGLE] 	= SLEDID_Triangle,
		[AC_SINE] 		= SLEDID_Sine,
};

// Mapping label name
static const char s_MappingNameProcess[PROCESS_ID_COUNT][8] = {
		[PROCESS_MMA] = "M M A",
		[PROCESS_TIG] = "T I G"
};

// Mapping label value
static const char s_MappingValueMemory[CHANEL_ID_COUNT][8] = {
		[ID_CH1] 	= "C H - 1",
		[ID_CH2] 	= "C H - 2",
		[ID_CH3] 	= "C H - 3",
		[ID_CH4] 	= "C H - 4",
		[ID_CH5] 	= "C H - 5",
		[ID_CH6] 	= "C H - 6",
		[ID_CH7] 	= "C H - 7",
};

static const char s_MappingValueProcess[PROCESS_ID_COUNT + 2][8] = {
		[PROCESS_MMA] 		= "S M A W",
		[PROCESS_TIG] 		= "    H F",
		[PROCESS_MMA + 2] 	= "  V R D",
		[PROCESS_TIG + 2] 	= "L I F T",
};

static const char s_MappingValuePolarity[POLARITY_ID_COUNT][8] = {
		[POLARITY_DC] 	= "    D C",
		[POLARITY_AC] 	= "    A C"
};

static const char s_MappingValueMode[MODE_ID_COUNT][8] = {
		[MODE_STD] 		= "  S T D",
		[MODE_HOLD] 	= "H O L D"
};

static const char s_MappingValueACShape[AC_SHAPE_ID_COUNT][8] = {
		[AC_SOFTSQUARE]	= "S O F T",
		[AC_SQUARE] 	= "A D V S",
		[AC_SINE]		= "S I N E",
		[AC_TRIANGLE]	= "  T R I"
};

static const char s_MappingValueOnOff[ONOFF_ID_COUNT][8] = {
		[ID_OFF]	= "  O F F",
		[ID_ON] 	= "    O N"
};

static MIDMenuLV2Id_t s_LogChild[HEADER_ID_COUNT] = {
	[HEADER_ID_AMPER] 			= MENULV2_ID_AMPER,
	[HEADER_ID_PROCESS] 		= MENULV2_ID_PROCESS,
	[HEADER_ID_POLARITY] 		= MENULV2_ID_POLARITY,
	[HEADER_ID_MODE] 			= MENULV2_ID_MODE,
	[HEADER_ID_PULSE] 			= MENULV2_ID_START_PULSE,
	[HEADER_ID_SEQUENCE] 		= MENULV2_ID_START_SEQUENCE,
	[HEADER_ID_FORCE] 			= MENULV2_ID_START_FORCE,
	[HEADER_ID_GAS] 			= MENULV2_ID_START_GAS,
	[HEADER_ID_AC_SHAPE] 		= MENULV2_ID_AC_SHAPE,
	[HEADER_ID_AC_PARAMETER] 	= MENULV2_ID_START_AC_PARAM,
	[HEADER_ID_EXTERNAL] 		= MENULV2_ID_START_EXTERNAL,
	[HEADER_ID_MEMORY] 			= MENULV2_ID_MEMORY
};

static const ListMenuItem_t MenuFrontInit[MENULV2_ID_FINAL_FRONT - MENULV2_ID_START_FRONT] = {
	[MENULV2_ID_AMPER] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AMPER,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 100, .Step = 1,
					.Name = "A M P R", .IsSpecialLeft = true, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_AMPER, .Unit = "A"}
	},
	[MENULV2_ID_PROCESS] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_PROCESS,
			.data = {.Min = PROCESS_ID + 1, .Max = PROCESS_ID_COUNT - 1, .Def = PROCESS_MMA, .Step = 1,
					.Name = "P R O C", .IsSpecialLeft = true, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_1,
					.ID = MENULV2_ID_PROCESS, .Unit = " "}
	},
	[MENULV2_ID_POLARITY] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_POLARITY,
			.data = {.Min = POLARITY_ID + 1, .Max = POLARITY_ID_COUNT - 1, .Def = POLARITY_DC, .Step = 1,
					.Name = "P O L A", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Unused_1,
					.ID = MENULV2_ID_POLARITY, .Unit = " "}
	},
	[MENULV2_ID_MODE] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_MODE,
			.data = {.Min = MODE_ID + 1, .Max = MODE_ID_COUNT - 1, .Def = MODE_STD, .Step = 1,
					.Name = "O U T", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_1,
					.ID = MENULV2_ID_MODE, .Unit = " "}
	},

	[MENULV2_ID_PULSE_ENABLE] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_PULSE_FREQUENCY,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "P P S", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_PULSE_ENABLE, .Unit = " "}
	},
	[MENULV2_ID_PULSE_FREQUENCY] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_PULSE_DUTY,
			.data = {.Min = 2, .Max = 5000, .Def = 20, .Step = 1, .PointID = PointID_E1,
					.Name = "P P S", .IsSpecialLeft = false, .IsSpecialRight = false,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_PPSHz,
					.ID = MENULV2_ID_PULSE_FREQUENCY, .Unit = "H"}
	},
	[MENULV2_ID_PULSE_DUTY] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_PULSE_BKGND,
			.data = {.Min = 10, .Max = 90, .Def = 50, .Step = 1,
					.Name = "P K T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_PPSDuty,
					.ID = MENULV2_ID_PULSE_DUTY, .Unit = "%"}
	},
	[MENULV2_ID_PULSE_BKGND] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_PULSE_FREQUENCY,
			.data = {.Min = 5, .Max = 95, .Def = 50, .Step = 1,
					.Name = "B K A", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_BKGNDA,
					.ID = MENULV2_ID_PULSE_BKGND, .Unit = "%"}
	},

	[MENULV2_ID_SEQUENCE_START] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_SEQUENCE_UP,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 5, .Step = 1,
					.Name = "I N T L", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_IntialA_HotC,
					.ID = MENULV2_ID_SEQUENCE_START, .Unit = "A"}
	},
	[MENULV2_ID_SEQUENCE_UP] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_SEQUENCE_DOWN,
			.data = {.Min = 0, .Max = 20, .Def = 5, .Step = 1,
					.Name = "I S L P", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_UpSlope_HotT,
					.ID = MENULV2_ID_SEQUENCE_UP, .Unit = "s"}
	},
	[MENULV2_ID_SEQUENCE_DOWN] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_SEQUENCE_FINAL,
			.data = {.Min = 0, .Max = 20, .Def = 5, .Step = 1,
					.Name = "F S L P", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_DownSlope,
					.ID = MENULV2_ID_SEQUENCE_DOWN, .Unit = "s"}
	},
	[MENULV2_ID_SEQUENCE_FINAL] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_SEQUENCE_START,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 5, .Step = 1,
					.Name = "F N L", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_FinalA_ARCForce,
					.ID = MENULV2_ID_SEQUENCE_FINAL, .Unit = "A"}
	},

	[MENULV2_ID_FORCE_HOTC] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_FORCE_HOTT,
			.data = {.Min = 50, .Max = 300, .Def = 80, .Step = 1,
					.Name = "H O T C", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_IntialA_HotC,
					.ID = MENULV2_ID_FORCE_HOTC, .Unit = "A"}
	},
	[MENULV2_ID_FORCE_HOTT] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_FORCE_ARCF,
			.data = {.Min = 1, .Max = 50, .Def = 25, .Step = 1,
					.Name = "H O T T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_UpSlope_HotT,
					.ID = MENULV2_ID_FORCE_HOTT, .Unit = "m"}
	},
	[MENULV2_ID_FORCE_ARCF] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_FORCE_HOTC,
			.data = {.Min = 70, .Max = 200, .Def = 100, .Step = 1,
					.Name = "A R C F", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_FinalA_ARCForce,
					.ID = MENULV2_ID_FORCE_ARCF, .Unit = "A"}
	},

	[MENULV2_ID_GAS_PRE] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_GAS_POST,
			.data = {.Min = 0, .Max = 200, .Def = 1, .Step = 1,
					.Name = "P R E", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E1,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Preflow,
					.ID = MENULV2_ID_GAS_PRE, .Unit = "s"}
	},
	[MENULV2_ID_GAS_POST] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_GAS_PRE,
			.data = {.Min = 1, .Max = 80, .Def = 8, .Step = 1,
					.Name = "P O S T", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Postflow,
					.ID = MENULV2_ID_GAS_POST, .Unit = "s"}
	},

	[MENULV2_ID_AC_SHAPE] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_SHAPE,
			.data = {.Min = AC_SHAPE_ID + 1, .Max = AC_SHAPE_ID_COUNT - 1, .Def = AC_SOFTSQUARE, .Step = 1,
					.Name = "S H A P", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_1,
					.ID = MENULV2_ID_AC_SHAPE, .Unit = " "}
	},

	[MENULV2_ID_AC_PARAM_EN] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_PARAM_EP,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 100, .Step = 1,
					.Name = "E N", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_ENA,
					.ID = MENULV2_ID_AC_PARAM_EN, .Unit = "A"}
	},
	[MENULV2_ID_AC_PARAM_EP] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_PARAM_BALANCE,
			.data = {.Min = 5, .Max = MaxPreset, .Def = 5, .Step = 1,
					.Name = "E P", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_EPA,
					.ID = MENULV2_ID_AC_PARAM_EP, .Unit = "A"}
	},
	[MENULV2_ID_AC_PARAM_K_ENEP] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_PARAM_K_ENEP,
			.data = {.Min = 1, .Max = 999, .Def = 100, .Step = 1,
					.Name = "RAITO NP", .IsSpecialLeft = true, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = true, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_AC_PARAM_K_ENEP, .Unit = " "}
	},
	[MENULV2_ID_AC_PARAM_BALANCE] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_PARAM_FREQUENCY,
			.data = {.Min = 15, .Max = 50, .Def = 30, .Step = 1,
					.Name = "B A L", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_Balance,
					.ID = MENULV2_ID_AC_PARAM_BALANCE, .Unit = "%"}
	},
	[MENULV2_ID_AC_PARAM_FREQUENCY] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_AC_PARAM_EN,
			.data = {.Min = 20, .Max = 200, .Def = 50, .Step = 1,
					.Name = "F R E Q", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = true, .SLEDId = SLEDID_ACFrequancy,
					.ID = MENULV2_ID_AC_PARAM_FREQUENCY, .Unit = "H"}
	},

	[MENULV2_ID_EXTERNAL_WIFI] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_EXTERNAL_DEVICENET,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "W I F I", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = IsOption, .AllowAccel = false, .SLEDId = SLEDID_Wifi,
					.ID = MENULV2_ID_EXTERNAL_WIFI, .Unit = " "}
	},
	[MENULV2_ID_EXTERNAL_DEVICENET] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_EXTERNAL_RTU,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "D D N", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = IsOption, .AllowAccel = false, .SLEDId = SLEDID_DeviceNet,
					.ID = MENULV2_ID_EXTERNAL_DEVICENET, .Unit = " "}
	},
	[MENULV2_ID_EXTERNAL_RTU] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_EXTERNAL_ROBOT,
			.data = {.Min = 10000, .Max = 10247, .Def = 0, .Step = 1,
					.Name = "R T U", .IsSpecialLeft = false, .IsSpecialRight = false, .PointID = PointID_E0,
					.IsHidden = IsOption, .AllowAccel = true, .SLEDId = SLEDID_Modbus,
					.ID = MENULV2_ID_EXTERNAL_RTU, .Unit = " "}
	},
	[MENULV2_ID_EXTERNAL_ROBOT] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_EXTERNAL_WIFI,
			.data = {.Min = ONOFF_ID + 1, .Max = ONOFF_ID_COUNT - 1, .Def = ID_OFF, .Step = 1,
					.Name = "A A T O", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = IsOption, .AllowAccel = false, .SLEDId = SLEDID_RobotAnalog,
					.ID = MENULV2_ID_EXTERNAL_ROBOT, .Unit = " "}
	},

	[MENULV2_ID_MEMORY] = {.IDHome = MENULV2_ID_AMPER, .IDNext = MENULV2_ID_MEMORY,
			.data = {.Min = CHANEL_ID + 1, .Max = CHANEL_ID_COUNT - 1, .Def = ID_CH1, .Step = 1,
					.Name = "M E M", .IsSpecialLeft = false, .IsSpecialRight = true, .PointID = PointID_E0,
					.IsHidden = false, .AllowAccel = false, .SLEDId = SLEDID_Unused_0,
					.ID = MENULV2_ID_MEMORY, .Unit = " "}
	}
};

static bool SkipHeader(MIDMenuHeaderId_t ID){

	return mid_ManagerMenu_HiddenHeader(ID);
}

static void NavigationHeader(MIDMenuHeaderId_t ID){
	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(s_MappingBTN[ID]);

	if (BTN == SVC_BTN_STATE_RELEASED) {return;}

	if (SkipHeader(ID) == true) {return;}

	s_PresentHeader = ID;
	svc_Timer_set_TZero(&TimerComeBackHome);
}

static void NavigationChild (MIDMenuHeaderId_t Previous, MIDMenuHeaderId_t Present, ListMenuItem_t **OutChild){
	uint16_t IDHeader = Previous;
	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(s_MappingBTN[IDHeader]);
	static SVC_BTN_STATE_t PreviousBTN[HEADER_ID_COUNT];
	bool SkipMenu = false;

	for (uint16_t ID = 0; ID < HEADER_ID_COUNT; ID++){
		if (ID == IDHeader){continue;}

		PreviousBTN[ID] = SVC_BTN_STATE_RELEASED;
	}

	if (Present != Previous){
		mid_ManagerMenu_getAdressItem(s_LogChild[Present], OutChild);
	}

	if (Present == HEADER_ID_PULSE && mid_ManagerMenu_getParamItem(MENULV2_ID_PULSE_ENABLE) == ID_OFF){
		mid_ManagerMenu_getAdressItem(MENULV2_ID_PULSE_ENABLE, OutChild);
		s_LogChild[MENULV2_ID_PULSE_ENABLE] = (*OutChild)->data.ID;
		return;
	}

	if (BTN != SVC_BTN_STATE_RELEASED){
		IsBlinkLED = false;
		PreviousBTN[IDHeader] = BTN;
	} else if (PreviousBTN[IDHeader] != SVC_BTN_STATE_RELEASED){
		PreviousBTN[IDHeader] = BTN;
		do {
			mid_ManagerMenu_getAdressItem((*OutChild)->IDNext, OutChild);
			s_LogChild[IDHeader] = (*OutChild)->data.ID;
			SkipMenu = (*OutChild)->data.IsHidden;
		} while (SkipMenu == true);
	}
}

static void UpdatePreviousHeader (MIDMenuHeaderId_t PresentID, MIDMenuHeaderId_t *Previous){
	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(s_MappingBTN[PresentID]);

	if (BTN == SVC_BTN_STATE_RELEASED){
		*Previous = PresentID;
	}
}

static void ComeBackHome(void){
	if (s_PresentHeader == HEADER_ID_AMPER){
		svc_Timer_set_TZero(&TimerComeBackHome);
		IsBlinkLED = false;
		return;
	}

	if (svc_Timer_DeltaT_s(TimerComeBackHome) < TIMER_COMEBACK_HOME) {return;}

	s_PresentHeader = HEADER_ID_AMPER;
}

static bool IsAdjustParamterByBTN(MIDMenuHeaderId_t IDHeader){
	if (IDHeader == HEADER_ID_MEMORY){ return true;}
	if (IDHeader == HEADER_ID_PROCESS) { return true;}
	if (IDHeader == HEADER_ID_MODE) { return true;}
	if (IDHeader == HEADER_ID_POLARITY) { return true;}
	if (IDHeader == HEADER_ID_AC_SHAPE) { return true;}

	return false;
}

static void AdjustParamterByBTN (MIDMenuHeaderId_t Previous, ListMenuItem_t **Itemp){
	int32_t Temp = *(*Itemp)->data.pValue; //(*Itemp)->data.Value;

	uint16_t IDHeader = Previous;

	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(s_MappingBTN[IDHeader]);
	static SVC_BTN_STATE_t PreviousBTN[HEADER_ID_COUNT];


	for (uint16_t ID = 0; ID < HEADER_ID_COUNT; ID++){
		if (ID == IDHeader){continue;}

		PreviousBTN[ID] = SVC_BTN_STATE_RELEASED;
	}

	if (BTN != SVC_BTN_STATE_RELEASED){
		PreviousBTN[IDHeader] = BTN;
	} else if (PreviousBTN[IDHeader] != SVC_BTN_STATE_RELEASED){
		PreviousBTN[IDHeader] = BTN;

		Temp++;
		mid_ManagerMenu_RoundRobin(&Temp, (*Itemp)->data.Min, (*Itemp)->data.Max);
		*(*Itemp)->data.pValue = Temp;
	}
}

static void AdjustParamterByEncoder (ListMenuItem_t **Itemp){
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

	if (Raw != 0){
		svc_Timer_set_TZero(&TimerComeBackHome);
		IsBlinkLED = true;
	}

	Temp += Raw*Step;

	if (Temp <= 0){
		Temp = 0;
	}
	mid_ManagerMenu_LimitValue(&Temp, (*Itemp)->data.Min, (*Itemp)->data.Max);

	*(*Itemp)->data.pValue = Temp;
}

static void AdjustParamter(void){
	if (IsAdjustParamterByBTN(s_PresentHeader) == true){
		AdjustParamterByBTN(s_PreviousHeader, &s_PresentChild);
		return;
	}

	AdjustParamterByEncoder(&s_PresentChild);
}


static void TurnOnOff_Pulse(ListMenuItem_t **OutChild){
	uint16_t Temp = mid_ManagerMenu_getParamItem(MENULV2_ID_PULSE_ENABLE);

	SVC_BTN_STATE_t BTN = svc_Button_get_StateBtn(s_MappingBTN[HEADER_ID_PULSE]);
	static SVC_BTN_STATE_t PreviousBTN = SVC_BTN_STATE_RELEASED;

	if (mid_ManagerMenu_getHiddenItem(MENULV2_ID_PULSE_ENABLE) == true){return;}

	if (BTN != SVC_BTN_STATE_HOLD){
		if (PreviousBTN == SVC_BTN_STATE_HOLD){
			if (Temp == ID_ON){
				mid_ManagerMenu_getAdressItem((*OutChild)->IDNext, OutChild);
				s_LogChild[HEADER_ID_PULSE] = (*OutChild)->data.ID;
			} else {
				mid_ManagerMenu_getAdressItem(MENULV2_ID_AMPER, OutChild);
				s_LogChild[HEADER_ID_PULSE] = MENULV2_ID_START_PULSE;
				s_PresentHeader = HEADER_ID_AMPER;
			}

		}
		PreviousBTN = BTN;
		return;
	}

	if (PreviousBTN != SVC_BTN_STATE_HOLD){
		PreviousBTN = SVC_BTN_STATE_HOLD;

		mid_ManagerMenu_getAdressItem(MENULV2_ID_START_PULSE, OutChild);
		s_LogChild[HEADER_ID_PULSE] = (*OutChild)->data.ID;

		if (Temp == ID_OFF){
			mid_ManagerMenu_setParamItem(MENULV2_ID_PULSE_ENABLE, ID_ON);
		} else {
			mid_ManagerMenu_setParamItem(MENULV2_ID_PULSE_ENABLE, ID_OFF);
		}
	}
}

static void DisplayNameAmper(void){
	if (s_PresentHeader != HEADER_ID_AMPER){return;}

	uint16_t VoltFeedback = 0;

	svc_LED14Seg_ShowNumber(VoltFeedback, Alignment_Left, PointID_E1);
	svc_LED14Seg_ShowUint("V", Alignment_Left);
}

static void DisplayValueSpecial(MIDMenuHeaderId_t ID, const char Mapping[][8]){
	if (s_PresentHeader != ID){return;}

	char Display[8];
	memcpy(Display, Mapping[*s_PresentChild->data.pValue], sizeof(Display));

	svc_LED14Seg_ShowString(Display, Alignment_Right);
}

static void DisplayNameSpecial(MIDMenuHeaderId_t ID, const char Mapping[][8]){
	if (s_PresentHeader != ID){return;}

	char Display[8];
	memcpy(Display, Mapping[*s_PresentChild->data.pValue], sizeof(Display));

	svc_LED14Seg_ShowString(Display, Alignment_Left);
}

static void DisplayNameChild(void){
	if (s_PresentChild->data.IsSpecialLeft == false){
		svc_LED14Seg_ShowString(s_PresentChild->data.Name, Alignment_Left);
		return;
	}

	DisplayNameAmper();
	DisplayNameSpecial(HEADER_ID_PROCESS, s_MappingNameProcess);
}

static void DisplayValueChild(void){
	if (s_PresentChild->data.IsSpecialRight == false){
		svc_LED14Seg_ShowNumber(*s_PresentChild->data.pValue, Alignment_Right, s_PresentChild->data.PointID);

		if (*s_PresentChild->data.pValue < 999){
			svc_LED14Seg_ShowUint(s_PresentChild->data.Unit, Alignment_Right);
		}
		return;
	}

	DisplayValueSpecial(HEADER_ID_MEMORY, s_MappingValueMemory);
	DisplayValueSpecial(HEADER_ID_PROCESS, s_MappingValueProcess);
	DisplayValueSpecial(HEADER_ID_POLARITY, s_MappingValuePolarity);
	DisplayValueSpecial(HEADER_ID_MODE, s_MappingValueMode);
	DisplayValueSpecial(HEADER_ID_PULSE, s_MappingValueOnOff);
	DisplayValueSpecial(HEADER_ID_AC_SHAPE, s_MappingValueACShape);
	DisplayValueSpecial(HEADER_ID_EXTERNAL, s_MappingValueOnOff);
}

static void DisplayChild(void){
	DisplayNameChild();
	DisplayValueChild();
}

static void SpecialSingleLED(MIDMenuLV2Id_t ID, const SVC_LEDSingleID_t *Mapping){
	ListMenuItem_t Item = mid_ManagerMenu_getItem(ID);
	uint16_t ValueItem = *Item.data.pValue;
	bool IsHidden = Item.data.IsHidden;

	for (uint16_t i = Item.data.Min; i <= Item.data.Max; i++){
		if (i == ValueItem && IsHidden == false){
			svc_LEDSingle_TurnOn(Mapping[i]);
			continue;
		}

		svc_LEDSingle_TurnOFF(Mapping[i]);
	}
}

static void DisplaySingleLEDChild(void){
	MIDMenuLV2Id_t PresentIDLV2 = s_PresentChild->data.ID;

	for (uint16_t IDLV2 = MENULV2_ID_START_FRONT; IDLV2 < MENULV2_ID_FINAL_FRONT; IDLV2++){
		if(IDLV2 == PresentIDLV2) {continue;}

		svc_LEDSingle_TurnOFF(MenuFrontInit[IDLV2].data.SLEDId);
	}

	if (IsBlinkLED == true){
		svc_LEDSingle_Blink(MenuFrontInit[PresentIDLV2].data.SLEDId);
		return;
	}

	svc_LEDSingle_TurnOn(MenuFrontInit[PresentIDLV2].data.SLEDId);

}

static void DisplaySingleLED(void){
	SpecialSingleLED(MENULV2_ID_PROCESS, s_MappingSingleLEDProcess);
	SpecialSingleLED(MENULV2_ID_POLARITY, s_MappingSingleLEDPolarity);
	SpecialSingleLED(MENULV2_ID_MODE, s_MappingSingleLEDMode);
	SpecialSingleLED(MENULV2_ID_AC_SHAPE, s_MappingSingleLEDACShape);
	DisplaySingleLEDChild();
}

static void MenuFrontDisplay(void){
	DisplayChild();
	DisplaySingleLED();
}

static void MenuFrontNavigation(void){
	for (uint16_t id = 0; id < HEADER_ID_COUNT; id++){
		NavigationHeader(id);
	}

	ComeBackHome();
	NavigationChild(s_PreviousHeader, s_PresentHeader, &s_PresentChild);
	UpdatePreviousHeader (s_PresentHeader, &s_PreviousHeader);
}

static void MenuFrontHiddenItem(void){
	if (mid_ManagerMenu_getParamItem(MENULV2_ID_POLARITY) == POLARITY_DC){
		mid_ManagerMenu_setHiddenItem(MENULV2_ID_AC_SHAPE);
	} else {
		mid_ManagerMenu_clearHiddenItem(MENULV2_ID_AC_SHAPE);
	}

	if (mid_ManagerMenu_getParamItem(MENULV2_ID_PROCESS) == PROCESS_MMA){
		mid_ManagerMenu_setHiddenItem(MENULV2_ID_MODE);
		mid_ManagerMenu_setHiddenItem(MENULV2_ID_PULSE_ENABLE);
		return;
	}

	if (mid_ManagerMenu_getParamItem(MENULV2_ID_PROCESS) == PROCESS_TIG){
		mid_ManagerMenu_clearHiddenItem(MENULV2_ID_MODE);
		mid_ManagerMenu_clearHiddenItem(MENULV2_ID_PULSE_ENABLE);
		return;
	}
}

void svc_MenuFrontInteraction(void){
	if (mid_ManagerMenu_getMenuLV1() != MENULV1_ID_FRONT){
		return;
	}

	s_LogChild[HEADER_ID_AMPER] = MENULV2_ID_AMPER;

	MenuFrontNavigation();

	TurnOnOff_Pulse(&s_PresentChild);

	MenuFrontHiddenItem();
	AdjustParamter();

	MenuFrontDisplay();
}

void svc_MenuFrontInital (void){
	for (uint16_t id = MENULV2_ID_START_FRONT; id < MENULV2_ID_FINAL_FRONT; id++){
		mid_ManagerMenu_setItem(id, MenuFrontInit[id - MENULV2_ID_START_FRONT]);
	}

	mid_ManagerMenu_getAdressItem(MENULV2_ID_AMPER, &s_PresentChild);
}
