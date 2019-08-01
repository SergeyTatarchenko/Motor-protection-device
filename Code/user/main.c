#include "user_tasks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"


/*semaphore for error handler*/
xSemaphoreHandle xErrorHandler;
/*mutex for I2C , perform correct transmit */
xSemaphoreHandle xMutex_BUS_BUSY;

int Init_(void);

/*main*/
int main(){
	if(Init_())
	{
		vTaskStartScheduler();/*run RTOS*/
	}
	else{GREEN_LED_ON;}
	while(1);	
}

/*board init*/
int Init_(){
	BaseType_t TaskCreation;
	/*all peripherals init*/
	SysInit();
	xErrorHandler = xSemaphoreCreateBinary();
	xMutex_BUS_BUSY = xSemaphoreCreateMutex();
	
	TaskCreation =	xTaskCreate(&error_handler_TASK,"error handler",configMINIMAL_STACK_SIZE, NULL, 4 , NULL );
	TaskCreation &=	xTaskCreate(&main_TASK,"main cycle",configMINIMAL_STACK_SIZE, NULL, 3 , NULL );	
	if(TaskCreation == pdTRUE)
	{
		/*user button interrupt enable (for demo)*/
		NVIC_EnableIRQ(EXTI0_1_IRQn);
		return TRUE;
	}
	else
	{
		/*no memory space*/
		return FALSE;
	}	
}
