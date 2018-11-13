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
#include "pcf8574.h"
#include "motor_protection.h"

void vSysInit(void *pvParameters);
void vADC_Conversion(void *pvParameters);
void vI2CTransfer(void *pvParameters);
void vTIM_PeriodConversion(void *pvParameters);

#endif
