#ifndef MOTOR_PROTECTION_H
#define MOTOR_PROTECTION_H

#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"

#define DEFAULT_VOLTAGE_BUF_SIZE	3
#define DEFAULT_PERIOD_BUF_SIZE		3

#define PHASEMETER_A_START   TIMER_6_START   
#define PHASEMETER_B_START   TIMER_7_START   
#define PHASEMETER_C_START   TIMER_14_START  

#define PHASEMETER_A_STOP    TIMER_6_STOP    
#define PHASEMETER_B_STOP    TIMER_7_STOP    
#define PHASEMETER_C_STOP    TIMER_14_STOP   


#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Voltage;
    uint32_t PhaseB_Voltage;
    uint32_t PhaseC_Voltage;

}CapturedVoltage_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint8_t PhaseA_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];
    uint8_t PhaseB_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];
    uint8_t PhaseC_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];

}VoltageTextLCD_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Period;
    uint32_t PhaseB_Period;
    uint32_t PhaseC_Period;

}CapturedPeriod_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint8_t PhaseA_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseB_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseC_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];

}PeriodLCD_REGISTR;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct{
	
	uint16_t PhaseA_Factor;
	uint16_t PhaseB_Factor;
	uint16_t PhaseC_Factor;
	
}PowerFactor_REGISTR;
#pragma pack(pop)

/*captured voltage from ADC with DMA*/ 
extern CapturedVoltage_REGISTR CapturedVoltage;
extern CapturedVoltage_REGISTR *CapturedVoltagePointer;

/* transmited voltage value in ASCII text */
extern VoltageTextLCD_REGISTR VoltageTextLCD;
extern VoltageTextLCD_REGISTR *VoltageTextLCDPointer;

/*captured period from timers*/ 
extern CapturedPeriod_REGISTR CapturedPeriod;
extern CapturedPeriod_REGISTR *CapturedPeriodPointer;

/* transmited period value in ASCII text */
extern PeriodLCD_REGISTR PeriodLCD;
extern PeriodLCD_REGISTR *PeriodLCDPointer;

/* captured power factor value in mcs */
extern PowerFactor_REGISTR PowerFactor;
extern PowerFactor_REGISTR *PowerFactorPointer;

/*ADC data from DMA transfer*/
extern uint32_t CapturedVoltageArray[3];

/*TIM15/16/17 CCR reg data from DMA*/
extern uint32_t TIM15_CCR1_Array[2];
extern uint32_t TIM16_CCR1_Array[2];
extern uint32_t TIM17_CCR1_Array[2];

extern int TimerWatchDog;
/*-------------------------------------------*/
extern void EnableMetering(void);
extern void DisableMetering(void);
extern uint32_t itoa(int i,uint8_t *buff, uint8_t MesSize);
#endif

