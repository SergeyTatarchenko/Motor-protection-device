
#include "motor_protection.h"
/*----------------------------------------------------------------------*/
/*initial configuration*/
MotorConfiguration_REGISTR MotorConfiguration;
MotorConfiguration_REGISTR *MotorConfigurationPointer;

/*error array */
ErrorArray_REGISTR ErrorArray;
/*watchdog*/
WatchDog_REGISTR WatchDog;
WatchDog_REGISTR *WatchDogPointer;

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
volatile uint32_t CapturedVoltageArray[3];

/*TIM15/16/17 CCR reg data from DMA*/
volatile uint32_t  TIM15_CCR1_Array[2];
volatile uint32_t TIM16_CCR1_Array[2];
volatile uint32_t TIM17_CCR1_Array[2];
/*----------------------------------------------------------------------*/

void PowerControl()
{
	
}

uint_least8_t power_factor_control(MotorConfiguration_REGISTR *configuration,PowerFactor_REGISTR *power_pointer)
{
	uint_least8_t error = FALSE;
	/*overload control*/
	if(power_pointer->PhaseA_Cos > configuration-> MaxPowerFactor)
	{
		
	}
	/*disbalance control*/
	return error;
}
	
/*phase imbalance control, return TRUE if phase imbalance exceeds setup value */
uint_least8_t phase_imbalance_control(ErrorArray_REGISTR* error_pointer,MotorConfiguration_REGISTR *configuration,CapturedVoltage_REGISTR *voltage_pointer)
{
	uint_least8_t error = FALSE;
	uint8_t phase_imb = 0;
	uint32_t buf[3],max_val,min_val;
	buf[0] = voltage_pointer->PhaseA_Voltage_peak;
	buf[1] = voltage_pointer->PhaseB_Voltage_peak;
	buf[2] = voltage_pointer->PhaseC_Voltage_peak;
	max_val = buf[0],min_val = buf[0]; 
	
	/*max value*/
	for(int i = 0;i <3;i++)
	{
		if(buf[i] > max_val)
		{
			max_val = buf[i];
		}
	}
	/*min value*/
	for(int i = 0;i <3;i++)
	{
		if(buf[i] < min_val)
		{
			min_val = buf[i];
		}
	}
	phase_imb = (uint8_t)(((float)min_val/(float)max_val)*100);
	if(phase_imb > configuration->PhaseImbalance)
	{
		error = TRUE;
	}
	return error;
}

 /*frequency control, return TRUE if frequency diverges from the set*/
 uint_least8_t freq_control(CapturedPeriod_REGISTR *freq_pointer,ErrorArray_REGISTR* error_pointer,MotorConfiguration_REGISTR *configuration)
{
	uint_least8_t error = FALSE;
	
	if((freq_pointer->PhaseA_Frequency > (configuration->SetupFrequency + configuration->MaxFrequencyShift))||
	   (freq_pointer->PhaseA_Frequency < (configuration->SetupFrequency - configuration->MaxFrequencyShift)))
	{
		error = TRUE;
		error_pointer->frequency.bit.PhaseA = TRUE;
	}
	else{
		error_pointer->frequency.bit.PhaseA = FALSE;
	}
	if((freq_pointer->PhaseB_Frequency > (configuration->SetupFrequency + configuration->MaxFrequencyShift))||
	   (freq_pointer->PhaseB_Frequency < (configuration->SetupFrequency - configuration->MaxFrequencyShift)))
	{
		error = TRUE;
		error_pointer->frequency.bit.PhaseB = TRUE;
	}
	else{
		error_pointer->frequency.bit.PhaseB = FALSE;
	}
	if((freq_pointer->PhaseC_Frequency > (configuration->SetupFrequency + configuration->MaxFrequencyShift))||
	   (freq_pointer->PhaseC_Frequency < (configuration->SetupFrequency - configuration->MaxFrequencyShift)))
	{
		error = TRUE;
		error_pointer->frequency.bit.PhaseC = TRUE;
	}
	else{
		error_pointer->frequency.bit.PhaseC = FALSE;
	}
	return error;
}
 
/*frequency watchdog,return TRUE if phase failure appears */
uint_least8_t freq_watchdog(WatchDog_REGISTR *watchdog_pointer,ErrorArray_REGISTR* error_pointer)
{
	uint_least8_t error  = FALSE;

	if(watchdog_pointer->FrequencyPhaseA == 0)
	{
		error = TRUE;
		error_pointer->phase_failure.bit.PhaseA = TRUE;
	}
	else
	{
		error_pointer->phase_failure.bit.PhaseA = FALSE;
	}
	if(watchdog_pointer->FrequencyPhaseB == 0)
	{
		error = TRUE;
		error_pointer->phase_failure.bit.PhaseB = TRUE;
	}
	else
	{
		error_pointer->phase_failure.bit.PhaseB = FALSE;
	}
	if(watchdog_pointer->FrequencyPhaseC == 0)
	{
		error = TRUE;
		error_pointer->phase_failure.bit.PhaseC = TRUE;
	}
	else
	{
		error_pointer->phase_failure.bit.PhaseC = FALSE;
	}
	return error;
}


/**/
void CheckPhaseRotation(void)
{

}
/**/
void AngleShearConversion(void)
{

}

/*calculate power factor, return result in deg from tick */
uint16_t CalcPowerFactor(uint16_t shift, uint32_t period)
{
	uint16_t cosine;
	if((period>0) && (shift>0)){
		cosine = ((uint32_t)shift*360)/((uint32_t)period);	
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
	}else if(i == 0){
		for(counter = 0; counter < MesSize; counter++){
			buff[counter] = 0;
			}
	} 
	return array_size;
}
