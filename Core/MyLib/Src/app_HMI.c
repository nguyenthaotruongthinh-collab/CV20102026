#include "app_HMI.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "svc_HMIInteraction.h"
#include "mid_ManagerMenu.h"
#include "svc_MenuFrontInteraction.h"
#include "svc_MenuUserInteraction.h"
#include "svc_MenuTechInteraction.h"
#include "svc_MenuHiddenInteraction.h"

osThreadId_t HMITaskHandle;

const osThreadAttr_t HMITask_attributes = {
  .name = "HMITask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

static void HMITask(void *argument){
	for(;;){
		svc_HMIInteraction();
		mid_ManagerMenu_HiddenItem();
		svc_MenuFrontInteraction();
		svc_MenuUserInteraction();
		svc_MenuTechInteraction();
		svc_MenuHiddenInteraction();
		osDelay(1);
	}
}

void app_HMI_Init(void){
	svc_MenuFrontInital();
	svc_MenuUserInital();
	svc_MenuTechInital();
	svc_MenuHiddenInital();
	HMITaskHandle = osThreadNew(HMITask, NULL, &HMITask_attributes);
}
