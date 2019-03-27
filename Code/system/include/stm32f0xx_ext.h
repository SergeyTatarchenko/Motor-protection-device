#ifndef EXT_H
#define EXT_H

#include "stm32f0xx.h"
#include "motor_protection.h"
#include "user_tasks.h"

/*-------------------------------------------*/
extern void EXTI_Init(void);
extern void NVIC_Init(void);
extern void DisableEXTI_Interupts(void);
#endif
