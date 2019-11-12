/*-------------------------------------------------
                SOM global config 
-------------------------------------------------*/
#ifndef OBJ_CONFIG_H_
#define	OBJ_CONFIG_H_

#define RTOS_USAGE	TRUE
#define SOM_MODE	APP_MODE
#define DEBUG_MODE TRUE
#define REVISION	3

/*<hw snap>*/
#define NUM_OF_OBJ_ADC	0
#define NUM_OF_OBJ_INPUTS	0
#define NUM_OF_OBJ_OUTPUTS	0
#define NUM_OF_PWM	0
/*</hw snap>*/
/*-----------------------------------------------
************communication channels***************
-----------------------------------------------*/
#define USART_COM_ENABLE	TRUE
#define CAN_COM_ENABLE    	FALSE
/*-----------------------------------------------
************SOM memory usage config**************
-----------------------------------------------*/
#define TARGET = 3	
#include "B3.h"
	
#if SOM_MODE == APP_MODE
	
	#define HARDWARE_OBJECT FALSE
	#define USART_DATA_FAST	TRUE
	#define OBJECT_TIMER	FALSE

	#define	num_of_all_obj		10
	#define MES_BUF_SIZE		5

	#if HARDWARE_OBJECT == TRUE
		#define NUM_OF_HWOBJ    0
	#endif
	#if OBJECT_TIMER == TRUE
		#define NUM_OF_TIMER    2
	#endif
	
	#ifndef TARGET
		#include "DEFAULT.h"
	#endif
	
#endif

#if RTOS_USAGE == TRUE
	#include "RTOS.h"
#endif


#endif

