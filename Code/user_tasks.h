#ifndef USER_TASKS_H
#define USER_TASKS_H



#include "pcf8574.h"

#include "stm32f0xx_adc.h"
#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"

#include "stdlib.h"

uint32_t itoa(int i,uint8_t *buff);

void vSysInit(void *pvParameters);
void vInitLCD(void *pvParameters);

void vADC_Conversion(void *pvParameters);
void vI2CTransfer(void *pvParameters);

#endif
