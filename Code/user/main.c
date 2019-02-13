#include "user_tasks.h"

#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOSConfig.h"

int main(){
	
	xTaskCreate(&check_state_TASK,"program start", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	
	/*run RTOS*/
	vTaskStartScheduler();
	
	while(1);	
}
