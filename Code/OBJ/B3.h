#ifndef B72_H_
#define	B72_H_

#include "OBJ_MODEL.h"
#include "obj_ID.h"
#include "hw_support.h"
/*-------------------------------------------------*/
#define	ID_NETWORK			0x01
#define	ID_DEVICE			0x03
#define ID_REMOTE_CNTRL		0x02
#define	FLAG_RX_ALL			0xFF
/*
v 0.5
1) create object name  example   " #define obj_name	(IND_obj_NULL + x)"
2) create object init like "obj_name_init  obj_index,class,snap,handler	
3) add 	object init to 	_obj_cofig_	
*/
/*-----------------------------------------------------------------------------------------------------------------------\
           name               |      index          |   class     | type     | hw_snap   |    delay    |   Handler        | 
\-----------------------------------------------------------------------------------------------------------------------*/ 
#define _obj_STATUS_init       obj_STATUS           ,IND_obj_CAS  ,obj_soft  ,   NULL     ,NULL        ,board_START
/*--------------------------------------------------------------------------------------------------------------------*/


/*init struct for obj model setup*/
#define _obj_cofig_	\
{_obj_STATUS_init}

/*--------------------------------------------------------------------------------------*/
/*obj handlers*/
void board_START(OBJ_STRUCT *obj);
/*-------------------------------------------------*/
void ADC0_Handler(OBJ_STRUCT *obj);
void ADC1_Handler(OBJ_STRUCT *obj);
void ADC2_Handler(OBJ_STRUCT *obj);
void ADC3_Handler(OBJ_STRUCT *obj);
void ADC4_Handler(OBJ_STRUCT *obj);
void ADC5_Handler(OBJ_STRUCT *obj);
/*-------------------------------------------------*/
void USART_Handler(OBJ_STRUCT *obj);
/*-------------------------------------------------*/
void Write_config_Handler(OBJ_STRUCT *obj);
/*-------------------------------------------------*/
void PWM_freq_config_Handler(OBJ_STRUCT *obj);
void PWM_Control_Handler(OBJ_STRUCT *obj);
void BUCK_Mode_Handler(OBJ_STRUCT *obj);
void BOOST_Mode_Handler(OBJ_STRUCT *obj);
void PID_COEF_Handler(OBJ_STRUCT *obj);
void PID_Control_Handler(OBJ_STRUCT *obj);
void KM_Off_Handler(OBJ_STRUCT *obj);
#endif
