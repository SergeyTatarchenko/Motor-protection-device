#include "motor_protection.h"

/*captured voltage from ADC with DMA*/ 
CapturedVoltage_REGISTR CapturedVoltage;
CapturedVoltage_REGISTR *CapturedVoltagePointer;

/* transmited voltage value in ASCII text */
VoltageTextLCD_REGISTR VoltageTextLCD;
VoltageTextLCD_REGISTR *VoltageTextLCDPointer;

/*captured period from timers*/ 
CapturedPeriod_REGISTR CapturedPeriod;
CapturedPeriod_REGISTR *CapturedPeriodPointer;

/* transmited period value in ASCII text */
PeriodLCD_REGISTR PeriodLCD;
PeriodLCD_REGISTR *PeriodLCDPointer;

/* captured power factor value in mcs */
PowerFactor_REGISTR PowerFactor;
PowerFactor_REGISTR *PowerFactorPointer;

/* transmited power factor value in ASCII text  */
PowerFactorLCD_REGISTR PowerFactorLCD;
PowerFactorLCD_REGISTR *PowerFactorLCDPointer;

/*ADC data from DMA transfer*/
uint32_t CapturedVoltageArray[3];

/*TIM15/16/17 CCR reg data from DMA*/
uint32_t TIM15_CCR1_Array[2];
uint32_t TIM16_CCR1_Array[2];
uint32_t TIM17_CCR1_Array[2];

int TimerWatchDog = 10;


/*enable interrupt on phasemeter*/
void EnableMetering(void){
	
	NVIC_EnableIRQ(EXTI0_1_IRQn);
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/*disable interrupt on phasemeter*/
void DisableMetering(void){
	
	NVIC_DisableIRQ(EXTI0_1_IRQn);
	NVIC_DisableIRQ(EXTI2_3_IRQn);
	NVIC_DisableIRQ(EXTI4_15_IRQn);
	
}

/*Check status of power network*/ 
uint_least8_t CheckPowerNetwork(void){
	
	uint_least8_t status;
	
	
	
	return status;
 }

/*calculate power factor, return result in deg */
uint16_t CalcPowerFactor(uint16_t shift, uint32_t period){

	uint16_t cosine;
	if((period>0) && (shift>0)){
		cosine = ((uint32_t)shift*180)/((uint32_t)period);	
	}else{
		cosine = 0;
	}
	return cosine;

}

/*convert integer to array*/
uint32_t itoa(int i,uint8_t *buff,uint8_t MesSize){
	int temp = i;
	int counter = 0;
	int offset;
	uint32_t array_size;
	if( i > 0){
		/*get  number of digits*/
		while(temp != 0){
			temp/=10;
			counter++;
		}
		array_size = (uint32_t)counter;
		temp = i;
		/*array fill*/
		offset = MesSize - counter;
		do{
			buff[(counter-1)+offset] = temp % 10;
			counter--;
			temp/=10;
		
			}
		while(counter!=0);
		if(array_size < MesSize){
			for(counter = 0; counter < (MesSize-array_size); counter++){
			buff[counter] = 0;
			}
		}
	} 
	return array_size;
}
