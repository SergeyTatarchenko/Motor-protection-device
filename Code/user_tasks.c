#include "user_tasks.h"

void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	/*init I2C1*/
	I2CInit();
	/*inut ADC1*/
	ADC_Init();
	
	xTaskCreate(vInitLCD, "Init LCD", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
	
	/*delete task*/
	vTaskDelete(NULL);
}

void vInitLCD(void *pvParameters){
	uint_least8_t state;	
	state = Init_LCD_1602();
		if(state){
			vTaskDelete(NULL);
		}else{
			/*add error handler */
			vTaskDelete(NULL);
		}		

}

void vADCConversion(void *pvParameters){
}
	
void vI2CTransfer(void *pvParameters){

}
