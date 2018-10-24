#include "motor_protection.h"

/*captured voltage from ADC with DMA*/ 
CapturedVoltage_REGISTR CapturedVoltage;
CapturedVoltage_REGISTR *CapturedVoltagePointer;

/*captured period from timers*/ 
CapturedPeriod_REGISTR CapturedPeriod;
CapturedPeriod_REGISTR *CapturedPeriodPointer;

uint32_t CapturedVoltageArray[3];

/* transmited voltage value in ASCII text */
VoltageTextLCD_REGISTR VoltageTextLCD;
VoltageTextLCD_REGISTR *VoltageTextLCDPointer;

/*convert integer to array*/
uint32_t itoa(int i,uint8_t *buff){
	int temp = i;
	int counter = 0;
	uint32_t array_size;
	/*get  number of digits*/
	while(temp != 0){
		temp/=10;
		counter++;
	}
	array_size = (uint32_t)counter;
	temp = i;
	/*array fill*/
	do{
		buff[counter-1] = temp % 10;
		counter--;
		temp/=10;
		
	}while(counter!=0);
	
	for(counter = 0; counter < (DEFAULT_VOLTAGE_BUF_SIZE-array_size); counter++){
		buff[counter] = 0;
	}  
	return array_size;
}
