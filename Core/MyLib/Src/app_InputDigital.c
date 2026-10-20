#include "app_InputDigital.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "bsp_InputDigital.h"
#include "svc_Button.h"
#include "svc_Faults.h"
#include "svc_Trigger.h"
#include "svc_Encoder.h"

osThreadId_t InputDigitalTaskHandle;

const osThreadAttr_t InputDigitalTask_attributes = {
  .name = "InputDigitalTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};

static void InputDigitalTask(void *argument){
	for(;;){
		svc_Button_UpdateStateButton();
		svc_Faults_UpdateStateDFault();
		svc_Trigger_Update_StateTrigger();

		svc_Encoder_Poll();
		osDelay(1);
	}
}

void app_InputDigital_Init(void){
	bsp_InputDigital_Init();

	svc_Button_BTN_Init();

	svc_Faults_DFault_Init();

//	svc_clear_ModeTriggerHOLD();
	svc_Trigger_set_ModeTriggerHOLD();

	svc_Encoder_Init();

	InputDigitalTaskHandle = osThreadNew(InputDigitalTask, NULL, &InputDigitalTask_attributes);
}
