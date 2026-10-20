#ifndef APPLICATION_USER_MYLIB_INC_SVC_LEDSINGLE_H_
#define APPLICATION_USER_MYLIB_INC_SVC_LEDSINGLE_H_

#include "bsp_LED595.h"
#include <stdint.h>

typedef enum {
	SLEDID_MMA = 0,
	SLEDID_TIG,
	SLEDID_DC,
	SLEDID_AC,
	SLEDID_HOLD,
	SLEDID_STD,
	SLEDID_Unused_0,
	SLEDID_Unused_1,
	SLEDID_Unused_2,
	SLEDID_BKGNDA,
	SLEDID_PPSDuty,
	SLEDID_PPSHz,
	SLEDID_FinalA_ARCForce,
	SLEDID_DownSlope,
	SLEDID_UpSlope_HotT,
	SLEDID_IntialA_HotC,
	SLEDID_Postflow,
	SLEDID_Preflow,
	SLEDID_Sine,
	SLEDID_Triangle,
	SLEDID_Square,
	SLEDID_SoftSquare,
	SLEDID_ACFrequancy,
	SLEDID_Balance,
	SLEDID_EPA,
	SLEDID_ENA,
	SLEDID_RobotAnalog,
	SLEDID_Modbus,
	SLEDID_DeviceNet,
	SLEDID_Wifi,
	SLEDID_Unused_3,
	SLEDID_Unused_4,
	SLEDID_ARCOn,
	SLEDID_Warning,
	SLEDID_SDCard,
	SLEDID_Unused_5,
	SLEDID_Unused_6,
	SLEDID_Unused_7,
	SLEDID_Unused_8,
	SLEDID_Unused_9,
	SLEDID_COUNT
}SVC_LEDSingleID_t;

void svc_LEDSingle_TurnOn(SVC_LEDSingleID_t ID);
void svc_LEDSingle_TurnOFF(SVC_LEDSingleID_t ID);
void svc_LEDSingle_Blink(SVC_LEDSingleID_t ID);

void svc_LEDSingle_Init(void);
void svc_LEDSingle_Reffresh(void);

#endif /* APPLICATION_USER_MYLIB_INC_SVC_LEDSINGLE_H_ */
