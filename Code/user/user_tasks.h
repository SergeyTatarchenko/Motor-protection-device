#ifndef USER_TASKS_H
#define USER_TASKS_H

/*----------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/*----------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_serial.h"
/*----------------------*/
#include "pcf8574.h"
#include "motor_protection.h"
/*----------------------*/
extern uint32_t ContentSwitching;
/*----------------------*/

/*3 level peiority*/
void _task_main(void *pvParameters);
void _task_led(void *pvParameters);

/*4 level peiority*/
void _task_error_handler(void *pvParameters);
/*----------------------*/	
void SysInit(void);
void mcu_gpio_init(void);
void mcu_peripheral_init(void);
/*----------------------*/
void frequency_conversion(void);
void adc_conversion(void);
void power_factor_conversion(void);
void phase_rotation_conversion(void);

/*semaphore for error handler*/
extern xSemaphoreHandle xErrorHandler;
/*mutex for I2C , perform correct transmit */
extern xSemaphoreHandle xMutex_BUS_BUSY;
/*----------------------

\----------------------*/
void i2c_transfer(void);
void text_ascii_conversion(void);

/*user functions*/
void led_0_invertor(void);
void led_1_invertor(void);

void LCD_DrawWorkspace(void);
void LCD_SetLoadingWindow(void);

#endif
