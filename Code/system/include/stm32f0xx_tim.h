#ifndef TIM_H
#define TIM_H

#include "stm32f0xx.h"
/*-----------local define-----------------------*/
#define TIMER_PSC   48  /*get 1 MHz timer frequency*/

#define TIMER_6_START   (TIM6->CR1 |=  TIM_CR1_CEN)
#define TIMER_7_START   (TIM7->CR1 |=  TIM_CR1_CEN)
#define TIMER_14_START  (TIM14->CR1|=  TIM_CR1_CEN)

#define TIMER_6_STOP    (TIM6->CR1 &= ~TIM_CR1_CEN)
#define TIMER_7_STOP    (TIM7->CR1 &= ~TIM_CR1_CEN)
#define TIMER_14_STOP   (TIM14->CR1&= ~TIM_CR1_CEN)


/*-----------global function prototypes---------*/
extern void GeneralTimerConfig(void);
extern void EnableGeneralTimers(void);
extern void DisableGeneralTimers(void);

#endif
