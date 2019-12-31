/*-----------------------------------------*/	
#include "user_tasks.h"
/*-----------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
/*-----------------------------------------*/
/*semaphore for error handler*/
xSemaphoreHandle xErrorHandler;
/*mutex for I2C , perform correct transmit */
xSemaphoreHandle xMutex_BUS_BUSY;

int Init_(void);

int main()
{
	SysInit();/*all peripherals init*/
	if(Init_())
	{
		vTaskStartScheduler();/*run RTOS*/
	}
	while(1);	
}

/*board init*/
int Init_()
{
	BaseType_t TaskCreation;
	xErrorHandler = xSemaphoreCreateBinary();
	xMutex_BUS_BUSY = xSemaphoreCreateMutex();
	TaskCreation =	xTaskCreate(&_task_error_handler,"error handler",configMINIMAL_STACK_SIZE, NULL, 4 , NULL );
	TaskCreation &=	xTaskCreate(&_task_main,"main cycle",configMINIMAL_STACK_SIZE, NULL, 3 , NULL );	
	TaskCreation &=	xTaskCreate(&_task_led ,"led",configMINIMAL_STACK_SIZE, NULL, 3 , NULL );

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
