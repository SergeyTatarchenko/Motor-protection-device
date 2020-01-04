/*----------------------------------------------------------------------
mcu config v 1.1
-----------------------------------------------------------------------*/
#ifndef MCU_CONFIG
#define MCU_CONFIG

#include <stm32f0xx.h>
#include "stm32f0xx_adc.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_serial.h"
#include "stm32f0xx_dma.h"
#include "stm32f0xx_ext.h"
/*----------------------------------------------------------------------*/
#include "motor_protection.h"
#include "constants.h"
/*----------------------------------------------------------------------*/
#define IWDG_RELOAD	(IWDG->KR = 0xAAAA)
/*----------------------------------------------------------------------*/
void SysInit(void);
void mcu_gpio_init(void);
void mcu_peripheral_init(void);
/*----------------------------------------------------------------------*/
void led_0_invertor(void);
void led_1_invertor(void);
/*----------------------------------------------------------------------*/

#endif
