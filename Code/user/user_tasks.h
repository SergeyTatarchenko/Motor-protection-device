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
#include "pcf8574.h"
#include "motor_protection.h"
/*----------------------*/
#define TRUE	1
#define FALSE	0
/*----------------------*/
extern uint32_t ContentSwitching;
/*----------------------*/

/*3 level peiority*/
void main_TASK(void *pvParameters);
void i2c_transfer_TASK(void *pvParameters);
/*4 level peiority*/
void error_handler_TASK(void *pvParameters);
/*----------------------*/	
void SysInit(void);
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
отображение на дисплее (demo)	
----------------------*/
void i2c_transfer(void);
void text_ascii_conversion(void);


#endif
