#include "user_tasks.h"

uint8_t TCH[4] ={CHAR_TCH};

uint8_t _PR_[4] ={CHAR_PRB};
uint8_t _m_[4] ={LETTER_m};
uint8_t _V_[4] ={LETTER_V};

/*fixed array size*/
uint8_t VoltageArray[4] ={0,0,0,0};


uint32_t itoa(int i,uint8_t *buff){
	int temp = i;
	int counter = 0;
	uint32_t array_size;
	/*get  number of digits*/
	while(temp != 0){
		temp/=10;
		counter++;
	}
	temp = i;
	array_size = (uint32_t)counter;
	/*array fill*/
	do{
		buff[counter-1] = temp % 10;
		counter--;
		temp/=10;
		
	}while(counter!=0);
	if(array_size<4){
		buff[0] = 0;
		
	}
	if(array_size<3){
		buff[1] = 0;
		
	}
	if(array_size<2){
		buff[2] = 0;
		
	}
	if(array_size < 1){
		buff[3] = 0;
		
	}
	return array_size;
}

/*init peripherals and start other tasks*/
void vSysInit(void *pvParameters){
	
	/*init led on Discovery board */
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER9_0;
	GPIOC->MODER |= GPIO_MODER_MODER8_0;
	GPIOC->BSRR |= GPIO_BSRR_BS_9;
	
	//	/*init I2C1*/
	I2CInit();
	/*inut ADC1*/
	ADC_Init();
	ADC_on;
	
	Init_LCD_1602();
	LCD_WriteText();
	
	xTaskCreate(vADC_Conversion, "ADC convertion", configMINIMAL_STACK_SIZE, NULL, 5 , NULL );
	xTaskCreate(vI2CTransfer, 	"I2C Transmit", configMINIMAL_STACK_SIZE, NULL, 4, NULL );
//	
	/*delete task*/
	vTaskDelete(NULL);
}
/*transmit current voltage to lcd*/	
void vI2CTransfer(void *pvParameters){
	uint8_t counter ;
	uint8_t letter;
	uint8_t temp;
	/*vatiable for transmit */
	uint8_t VoltageBuf[4];
	for(;;){
			
		
			LCD_SetDRAM_Adress(0x08);
			
		
			for(counter = 0 ; counter < 4; counter++){
					
				letter = VoltageArray[counter] + 0x30;

				/*clear buffer*/
				VoltageBuf[0] = 0x0D;
				VoltageBuf[1] = 0x09;
				VoltageBuf[2] = 0x0D;
				VoltageBuf[3] = 0x09;	
				
				temp = letter;
				/*high part of byte*/
				temp &= 0xF0;
				VoltageBuf[0] |= temp;
				VoltageBuf[1] |= temp;
	
				temp = letter;
				/*low part of byte*/
				temp &= 0x0F;
				VoltageBuf[2] |= temp<<4;
				VoltageBuf[3] |= temp<<4;

				I2CSendData(PCF8574_ADRESS,VoltageBuf,sizeof(VoltageBuf));
					vTaskDelay(2);
			}
			I2CSendData(PCF8574_ADRESS,_PR_,sizeof(_PR_));
			vTaskDelay(2);
			I2CSendData(PCF8574_ADRESS,_m_,sizeof(_m_));
			vTaskDelay(2);	
			I2CSendData(PCF8574_ADRESS,_V_,sizeof(_V_));
			vTaskDelay(2);
			
			vTaskDelay(500);
		}	
}

/*Create voltage array for lcd transmit, calc value from ADC1*/
void vADC_Conversion(void *pvParameters){
	/*variable for ADC value*/
	int VoltageValue;
	for(;;){
		/*get ADC value in mV*/
		VoltageValue = ADC_CalcValue();
		/*array fill*/
		if(VoltageValue > 3300){
			VoltageValue = 0;
		}
		itoa(VoltageValue,VoltageArray);
		vTaskDelay(50);
	}
}
