#ifndef DMA_H
#define DMA_H

#include "stm32f0xx.h"

#include "motor_protection.h"

extern void DMA_InitADC(void);
extern void DMA_InitTIM15(void);
extern void DMA_InitTIM16(void);
extern void DMA_InitTIM17(void);
#endif
