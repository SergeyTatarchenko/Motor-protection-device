#include "user_tasks.h"

int main(){

	xTaskCreate(vSysInit, "Program start", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	/*run RTOS*/
	vTaskStartScheduler();
	
	while(1);	
}
