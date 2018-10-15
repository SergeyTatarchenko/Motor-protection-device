#include "FreeRTOSConfig.h"
#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include "mcp23x17.h"
#include "pcf8574.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "adc.h"

void vBlink(void *pvParameters){
		Init_LCD_1602();
		LCD_WriteText();
//	for(;;){
//		GPIOC->BSRR |=GPIO_BSRR_BS_9;
//		vTaskDelay(500);
//		GPIOC->BSRR |=GPIO_BSRR_BR_9;
//		vTaskDelay(500);
//	}
	vTaskDelete(NULL);
}
int main(){
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	
	I2CInit();
	
//	ADC_Init();
	
	xTaskCreate(vBlink, "Program start", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
	/*run RTOS*/
	vTaskStartScheduler();
	
	
	while(1);	
}
