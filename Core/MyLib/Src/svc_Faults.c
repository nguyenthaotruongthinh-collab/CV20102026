#include "svc_Faults.h"
#include "svc_Timer.h"
#include <stdbool.h>

const uint8_t CountDFault = (uint8_t)SVC_DFAULT_ID_NORMAL - (uint8_t)SVC_DFAULT_ID_OCP;

static bool s_Skip_DFault[(uint8_t)SVC_DFAULT_ID_NORMAL - (uint8_t)SVC_DFAULT_ID_OCP] = {false};
static SVC_DFaults_ID s_DFaultID = SVC_DFAULT_ID_NORMAL;

static SVC_DFaults_ID svc_SkipDFault (SVC_DFaults_ID ID){
	if (s_Skip_DFault[(uint8_t)ID - (uint8_t)SVC_DFAULT_ID_OCP] == true){
		return SVC_DFAULT_ID_NORMAL;
	}

	return ID;
}

void svc_Faults_DFault_Init(void){
	for (uint8_t i = 0; i < CountDFault; i++){
		s_Skip_DFault[i] = false;
	}
}

void svc_Faults_UpdateStateDFault(void){
	uint8_t FaultCode = bsp_InputDigital_Fault_ReadPin();

	switch (FaultCode){
		case (uint8_t)SVC_DFAULT_ID_OCP:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OCP);
		break;
		case (uint8_t)SVC_DFAULT_ID_OCS:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OCS);
		break;
		case (uint8_t)SVC_DFAULT_ID_UV:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_UV);
		break;
		case (uint8_t)SVC_DFAULT_ID_UVP:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_UVP);
		break;
		case (uint8_t)SVC_DFAULT_ID_OV:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OV);
		break;
		case (uint8_t)SVC_DFAULT_ID_OVS:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OVS);
		break;
		case (uint8_t)SVC_DFAULT_ID_OT1:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OT1);
		break;
		case (uint8_t)SVC_DFAULT_ID_OT2:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_OT2);
		break;
		case (uint8_t)SVC_DFAULT_ID_WATER:
			s_DFaultID = svc_SkipDFault(SVC_DFAULT_ID_WATER);
		break;
		case (uint8_t)SVC_DFAULT_ID_NORMAL:
			s_DFaultID = SVC_DFAULT_ID_NORMAL;
		break;
		default:
			s_DFaultID = SVC_DFAULT_ID_NORMAL;
		break;
	}
}

void svc_Faults_set_Skip_DFault(SVC_DFaults_ID ID){
	s_Skip_DFault[(uint8_t)ID - (uint8_t)SVC_DFAULT_ID_OCP] = true;
}

void svc_Faults_Clear_Skip_DFault(SVC_DFaults_ID ID){
	s_Skip_DFault[(uint8_t)ID - (uint8_t)SVC_DFAULT_ID_OCP] = false;
}

SVC_DFaults_ID svc_Faults_get_DFaultID(void){
	return s_DFaultID;
}
