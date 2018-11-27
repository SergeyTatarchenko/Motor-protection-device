#include "user_tasks.h"

#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOSConfig.h"

int main(){
	
	xTaskCreate(vSysInit,"start", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	
	/*run RTOS*/
	vTaskStartScheduler();
	
	while(1);	
}
