#ifndef DMA_H
#define DMA_H

#include "stm32f0xx.h"

#include "motor_protection.h"
#include "OBJ_MODEL.h"

void DMA_InitADC(void);
void DMA_init_for_frequency_measurment(void);
void DMA_init_for_USART(void);
void DMA_Ch4_Reload(uint8_t *message, int new_buf_size);
#endif
