#ifndef TIM_H
#define TIM_H

#include "stm32f0xx.h"
/*-----------local define-----------------------*/
#define TIMER_PSC_1   (uint16_t)(48-1)  /*get 1 MHz timer frequency*/
#define TIMER_PSC_2   48000-1 	 				/*get 1 kHz timer frequency*/

#define TIMER_3_START   (TIM3->CR1 |=  TIM_CR1_CEN)
#define TIMER_6_START   (TIM6->CR1 |=  TIM_CR1_CEN)
#define TIMER_14_START  (TIM14->CR1|=  TIM_CR1_CEN)

#define TIMER_3_STOP    (TIM3->CR1 &= ~TIM_CR1_CEN)
#define TIMER_6_STOP    (TIM6->CR1 &= ~TIM_CR1_CEN)
#define TIMER_14_STOP   (TIM14->CR1&= ~TIM_CR1_CEN)


#define CLEAR_BUFFER_TIM3   (TIM3->CNT =  0)
#define CLEAR_BUFFER_TIM6   (TIM6->CNT =  0)
#define CLEAR_BUFFER_TIM14  (TIM14->CNT = 0)

/*-----------global function prototypes---------*/

extern void GeneralTimerConfig(void);
extern void EnableGeneralTimers(void);
extern void DisableGeneralTimers(void);

#endif
