/*
 * svc_Faults.h
 *
 *  Created on: Sep 25, 2026
 *      Author: ACER
 */

#ifndef APPLICATION_USER_MYLIB_INC_SVC_FAULTS_H_
#define APPLICATION_USER_MYLIB_INC_SVC_FAULTS_H_

#include "bsp_InputDigital.h"

typedef enum{
	SVC_DFAULT_ID_OCP		= 0x06,
	SVC_DFAULT_ID_OCS		= 0x07,
	SVC_DFAULT_ID_UV		= 0x08,
	SVC_DFAULT_ID_UVP		= 0x09,
	SVC_DFAULT_ID_OV		= 0x0a,
	SVC_DFAULT_ID_OVS		= 0x0b,
	SVC_DFAULT_ID_OT1		= 0x0c,
	SVC_DFAULT_ID_OT2		= 0x0d,
	SVC_DFAULT_ID_WATER		= 0x0e,
	SVC_DFAULT_ID_NORMAL	= 0x0f
}SVC_DFaults_ID;

void svc_Faults_DFault_Init(void);
void svc_Faults_UpdateStateDFault(void);
void svc_Faults_set_Skip_DFault(SVC_DFaults_ID ID);
void svc_Faults_Clear_Skip_DFault(SVC_DFaults_ID ID);
SVC_DFaults_ID svc_Faults_get_DFaultID(void);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_FAULTS_H_ */
