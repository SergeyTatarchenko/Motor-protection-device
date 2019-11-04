#ifndef MOTOR_PROTECTION_H
#define MOTOR_PROTECTION_H

#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "constants.h"

#define DEFAULT_VOLTAGE_BUF_SIZE            3
#define DEFAULT_PERIOD_BUF_SIZE             3
#define DEFAULT_POWER_FACTOR_BUF_SIZE       3

#define PHASEMETER_A_VALUE  TIM3->CNT
#define PHASEMETER_B_VALUE  TIM6->CNT
#define PHASEMETER_C_VALUE  TIM14->CNT

#define PHASEMETER_A_START   TIMER_3_START   
#define PHASEMETER_B_START   TIMER_6_START   
#define PHASEMETER_C_START   TIMER_14_START  

#define PHASEMETER_A_STOP    TIMER_3_STOP    
#define PHASEMETER_B_STOP    TIMER_6_STOP    
#define PHASEMETER_C_STOP    TIMER_14_STOP   

#define PHASEMETER_A_IRQ	(EXTI_IMR_MR1|EXTI_IMR_MR2)	
#define PHASEMETER_B_IRQ	(EXTI_IMR_MR3|EXTI_IMR_MR4)	
#define PHASEMETER_C_IRQ	(EXTI_IMR_MR5|EXTI_IMR_MR6)

#define PHROT_CHECK_IRQ     (EXTI_IMR_MR1|EXTI_IMR_MR3|EXTI_IMR_MR5)

#define PHASEMETER_DEFAULT	(EXTI_IMR_MR0)

#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Voltage_rms;
    uint32_t PhaseB_Voltage_rms;
    uint32_t PhaseC_Voltage_rms;

    uint32_t PhaseA_Voltage_peak;
    uint32_t PhaseB_Voltage_peak;
    uint32_t PhaseC_Voltage_peak;
    
} CapturedVoltage_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint8_t PhaseA_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];
    uint8_t PhaseB_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];
    uint8_t PhaseC_VoltageArray[DEFAULT_VOLTAGE_BUF_SIZE];

} VoltageTextLCD_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint32_t PhaseA_Period;
    uint32_t PhaseB_Period;
    uint32_t PhaseC_Period;
   
    uint32_t PhaseA_Frequency;
    uint32_t PhaseB_Frequency;
    uint32_t PhaseC_Frequency;

} CapturedPeriod_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{

    uint8_t PhaseA_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseB_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseC_PeriodArray[DEFAULT_PERIOD_BUF_SIZE];
   
    uint8_t PhaseA_FrequencyArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseB_FrequencyArray[DEFAULT_PERIOD_BUF_SIZE];
    uint8_t PhaseC_FrequencyArray[DEFAULT_PERIOD_BUF_SIZE];
    
} PeriodLCD_REGISTR;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct{
	
	uint16_t PhaseA_Factor;
	uint16_t PhaseB_Factor;
	uint16_t PhaseC_Factor;

    uint16_t PhaseA_Cos;
    uint16_t PhaseB_Cos;
    uint16_t PhaseC_Cos;
	
} PowerFactor_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{
	
	uint8_t PhaseA_FactorArray[DEFAULT_POWER_FACTOR_BUF_SIZE];
	uint8_t PhaseB_FactorArray[DEFAULT_POWER_FACTOR_BUF_SIZE];
	uint8_t PhaseC_FactorArray[DEFAULT_POWER_FACTOR_BUF_SIZE];
	
}PowerFactorLCD_REGISTR;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct{

    uint8_t MaxPowerFactor;
	uint8_t MinPowerFactor;
	uint8_t PhaseImbalance;
	uint16_t MaxFrequencyShift;
	uint16_t SetupFrequency;
	uint16_t FrequencyWatchdog;
	
} MotorConfiguration_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{
	
    int_fast16_t FrequencyPhaseA;
    int_fast16_t FrequencyPhaseB;
    int_fast16_t FrequencyPhaseC;
	
} WatchDog_REGISTR;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct{
	/*frequency error*/
	union{
		uint8_t byte;
		struct{
			unsigned PhaseA:1;
			unsigned PhaseB:1;
			unsigned PhaseC:1;
		}bit;	
	}frequency;
	/*phase shift error*/
	union {
		uint8_t byte;
		struct{
			unsigned PhaseA:1;
			unsigned PhaseB:1;
			unsigned PhaseC:1;
		}bit;
	}phase_shift;
	/*phase imbalance* error*/
	union{
		uint8_t byte;
		struct{
			unsigned PhaseA:1;
			unsigned PhaseB:1;
			unsigned PhaseC:1;
		}bit;
	}phase_imbalance;
	/*power factor error*/
	union{
		uint8_t byte;
		struct{
			unsigned PhaseA:1;
			unsigned PhaseB:1;
			unsigned PhaseC:1;
		}bit;		
	}power_factor;
	/*phase failure error*/
	union{
		uint8_t byte;
		struct{
			unsigned PhaseA:1;
			unsigned PhaseB:1;
			unsigned PhaseC:1;
		}bit;		
	}phase_failure;	
} ErrorArray_REGISTR;
#pragma pack(pop)


typedef enum{
	IDLE,			/*initial state after loading and reboot*/
	WORKING,		/*normal mode, drive control enabled*/
	ALARM,			/*alarm mode, triac disabled */		
	PROGRAMMING		/*programming mode, all control disabled*/
}STATE;

extern STATE device_state;

/*initial configuration*/
extern MotorConfiguration_REGISTR MotorConfiguration;
extern MotorConfiguration_REGISTR *MotorConfigurationPointer;
/*error array */
extern ErrorArray_REGISTR ErrorArray;
/*watchdog*/
extern WatchDog_REGISTR WatchDog;
extern WatchDog_REGISTR *WatchDogPointer;


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

/* transmited power factor value in ASCII text  */
extern PowerFactorLCD_REGISTR PowerFactorLCD;
extern PowerFactorLCD_REGISTR *PowerFactorLCDPointer;

/*current phase sequence*/
extern char CurPhaseSeq[3];

/*set phase sequence*/
extern char SetPhaseSeq[3];

/*ADC data from DMA transfer*/
extern uint32_t CapturedVoltageArray[3];

/*TIM15/16/17 CCR reg data from DMA*/
extern uint32_t TIM15_CCR1_Array[2];
extern uint32_t TIM16_CCR1_Array[2];
extern uint32_t TIM17_CCR1_Array[2];

/*-------------------------------------------*/
 void CheckPhaseRotation(void);
 void AngleShearConversion(void);
 uint32_t itoa(int i,uint8_t *buff, uint8_t MesSize);
 uint_least8_t CheckPowerNetwork(void);
 uint16_t CalcPowerFactor(uint16_t shift, uint32_t period);
 
 /*phase imbalance control, return TRUE if phase imbalance exceeds setup value */
uint_least8_t phase_imbalance_control(ErrorArray_REGISTR* error_pointer,MotorConfiguration_REGISTR *configuration,CapturedVoltage_REGISTR *voltage_pointer);
 /*frequency control, return TRUE if frequency diverges from the set*/
 uint_least8_t freq_control(CapturedPeriod_REGISTR *freq_pointer,ErrorArray_REGISTR* error_pointer,MotorConfiguration_REGISTR *configuration);
/*phase failure control*/
uint_least8_t freq_watchdog(WatchDog_REGISTR *watchdog_pointer,ErrorArray_REGISTR* error_pointer);
#endif

