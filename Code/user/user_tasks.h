#ifndef USER_TASKS_H
#define USER_TASKS_H

/*----------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/*----------------------------------------------------------------------*/
#include "pcf8574.h"
#include "motor_protection.h"
/*----------------------------------------------------------------------*/

extern uint32_t ContentSwitching;
extern xSemaphoreHandle xErrorHandler;		/*semaphore for error handler*/
extern xSemaphoreHandle xMutex_BUS_BUSY;	/*mutex for I2C , perform correct transmit */
/*tasks */
void _task_main(void *pvParameters);
void _task_led(void *pvParameters);
void _task_error_handler(void *pvParameters);
/*functions */
void frequency_conversion(void);
void adc_conversion(void);
void power_factor_conversion(void);
void phase_rotation_conversion(void);
void i2c_transfer(void);
void text_ascii_conversion(void);
void LCD_DrawWorkspace(void);
void LCD_SetLoadingWindow(void);

#endif
