#include "user_tasks.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"


/*semaphore for error handler*/
xSemaphoreHandle xErrorHandler;
/*mutex for I2C , perform correct transmit */
xSemaphoreHandle xMutex_BUS_BUSY;

void Init_(void);

/*main*/
int main(){
	
	Init_();	
	/*run RTOS*/
	
	vTaskStartScheduler();
	while(1);	
}

/*board init*/
void Init_(){
	BaseType_t TaskCreation;
	/*all peripherals init*/
	SysInit();
	xErrorHandler = xSemaphoreCreateBinary();
	xMutex_BUS_BUSY = xSemaphoreCreateMutex();

	if((xErrorHandler != NULL) && (xMutex_BUS_BUSY != NULL) ){				
	TaskCreation =	xTaskCreate(&error_handler_TASK,"error handler",configMINIMAL_STACK_SIZE, NULL, 4 , NULL );
	TaskCreation &=	xTaskCreate(&main_TASK,"main cycle",configMINIMAL_STACK_SIZE, NULL, 3 , NULL );	
		if(TaskCreation == pdTRUE){
			/*user button interrupt enable (for demo)*/
			NVIC_EnableIRQ(EXTI0_1_IRQn);
		}else{
			/*no memory space*/
			GREEN_LED_ON;
		}	
	}else{
			/*no memory space*/
	}
}
