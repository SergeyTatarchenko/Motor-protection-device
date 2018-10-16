#ifndef USER_TASKS_H
#define USER_TASKS_H


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "pcf8574.h"

#include "stm32f0xx_adc.h"
#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"

#include "FreeRTOSConfig.h"

void vSysInit(void *pvParameters);
void vInitLCD(void *pvParameters);

void vADCConversion(void *pvParameters);
void vI2CTransfer(void *pvParameters);

#endif
