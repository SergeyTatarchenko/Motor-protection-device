#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f0xx.h"
/*-------------------------------------------*/

void usart_init(void);
void usart_speed(uint32_t baudrate);
#endif
