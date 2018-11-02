#ifndef MOTOR_PROTECTION_H
#define MOTOR_PROTECTION_H

#include "stm32f0xx.h"
#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Voltage;
    uint32_t PhaseB_Voltage;
    uint32_t PhaseC_Voltage;

}CapturedVoltage_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint8_t PhaseA_VoltageArray[3];
    uint8_t PhaseB_VoltageArray[3];
    uint8_t PhaseC_VoltageArray[3];
}VoltageTextLCD_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Period;
    uint32_t PhaseB_Period;
    uint32_t PhaseC_Period;

}CapturedPeriod_REGISTR;
#pragma pack(pop)


/*captured voltage from ADC with DMA*/ 
extern CapturedVoltage_REGISTR CapturedVoltage;
extern CapturedVoltage_REGISTR *CapturedVoltagePointer;

/*captured period from timers*/ 
extern CapturedPeriod_REGISTR CapturedPeriod;
extern CapturedPeriod_REGISTR *CapturedPeriodPointer;

extern uint32_t CapturedVoltageArray[3];


extern uint8_t CapturedPeriodTimer[6];


extern uint32_t TIM15_CCR1;
extern uint32_t *TIM15_CCR1_Pointer;


extern uint32_t TestArray[2];

/* transmited voltage value in ASCII text */
extern VoltageTextLCD_REGISTR VoltageTextLCD;
extern VoltageTextLCD_REGISTR *VoltageTextLCDPointer;

#define DEFAULT_VOLTAGE_BUF_SIZE	3


/*-------------------------------------------*/
extern uint32_t itoa(int i,uint8_t *buff, uint8_t MesSize);
#endif

