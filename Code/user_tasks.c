#include "user_tasks.h"

uint8_t TCH[4] ={CHAR_TCH};

/*dynamic voltage array size */
uint32_t VoltageAraySize;

/*pointer to dymanic voltage array  */
uint8_t *VoltageArrayPointer;

/*convert integer to dynamic array (need to use vPortFree !!!)
  return arrray size */
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
	/*dynamic memory allocation*/
	buff = (uint8_t*) pvPortMalloc(counter);
	/*array fill*/
	do{
		buff[counter] = temp % 10;
		counter--;
	}while(counter!=0);
	
	return array_size;
}

/*init peripherals and start other tasks*/
void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	/*init I2C1*/
	I2CInit();
	/*inut ADC1*/
	ADC_Init();
	
	xTaskCreate(vInitLCD, "Init LCD", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
	xTaskCreate(vADC_Conversion, "ADC convertion", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	xTaskCreate(vI2CTransfer, 	"I2C Transmit", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
	
	/*delete task*/
	vTaskDelete(NULL);
}

/*LCD1602 init task*/
void vInitLCD(void *pvParameters){
	uint_least8_t state;	
	state = Init_LCD_1602();
		if(state){
			vTaskDelete(NULL);
		}else{
			/*add error handler */
			vTaskDelete(NULL);
		}		
}

/*Create voltage array for lcd transmit, calc value from ADC1*/
void vADC_Conversion(void *pvParameters){
	/*variable for ADC value*/
	uint32_t VoltageValue;
	for(;;){
		/*get ADC value in mV*/
		VoltageValue = ADC_CalcValue();
		/*create dymanic array*/
		VoltageAraySize = itoa(VoltageValue,VoltageArrayPointer);
		vTaskDelay(50);
	}
}

/*transmit current voltage to lcd*/	
void vI2CTransfer(void *pvParameters){
	uint8_t counter ;
	uint8_t letter;
	uint8_t temp;
	uint8_t state;
	/*vatiable for transmit */
	uint8_t VoltageBuf[4]= {0,0,0,0};
	
	VoltageBuf[0] |= 0x0D;
	VoltageBuf[2] |= 0x0D;

	VoltageBuf[1] |= 0x09;
	VoltageBuf[3] |= 0x09;
	
	for(;;){
		if(VoltageAraySize>3){
			
			for(counter = 0;counter <VoltageAraySize;counter++){
			
				/*clear buffer*/
				VoltageBuf[0] &= 0x0D;
				VoltageBuf[2] &= 0x0D;
				VoltageBuf[1] &= 0x09;
				VoltageBuf[3] &= 0x09;
	
				/*create ASCII hex*/
				letter = VoltageArrayPointer[counter]+ 0x30;
				
				temp = letter;
				/*high part of byte*/
				temp &= 0xF0;
				VoltageBuf[0] |= temp;
				VoltageBuf[1] |= temp;
		
				temp = letter;
				/*low part of byte*/
				temp &= 0x0F;
				VoltageBuf[2] |= temp;
				VoltageBuf[3] |= temp;
			
				if(counter == (VoltageAraySize-4)){
					I2CSendData(PCF8574_ADRESS,TCH,sizeof(TCH));
					vTaskDelay(2);		
				}
				state &= I2CSendData(PCF8574_ADRESS,VoltageBuf,sizeof(VoltageBuf));
				vTaskDelay(2);	
			}	
		}else{
			
		}
		vPortFree(VoltageArrayPointer);
		vTaskDelay(200);
	}
}
