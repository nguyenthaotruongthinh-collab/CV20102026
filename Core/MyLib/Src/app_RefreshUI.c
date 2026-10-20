#include "app_RefreshUI.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "bsp_LED595.h"
#include "svc_LED14Seg.h"
#include "svc_LEDSingle.h"

osThreadId_t RefreshUITaskHandle;

const osThreadAttr_t RefreshUITask_attributes = {
  .name = "RefreshUITask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

static void RefreshUITask(void *argument){
	for(;;){
		svc_LED14Seg_Reffresh();
		svc_LEDSingle_Reffresh();
		osDelay(1);
	}
}

void app_RefreshUI_Init(void){
	bsp_LED595_Init();
	svc_LEDSingle_Init();
	svc_LED14Seg_Init();

	RefreshUITaskHandle = osThreadNew(RefreshUITask, NULL, &RefreshUITask_attributes);
}
