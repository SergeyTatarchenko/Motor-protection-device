#include <stm32f0xx.h>
#include "user_tasks.h"
#include "mcu_configuration.h"

uint32_t ContentSwitching = 3; /*debug with LCD*/
STATE device_state = IDLE;

/*
error handler task
*/
void _task_error_handler(void *pvParameters)
{	
	for(;;){	
		/*error waiting*/
		xSemaphoreTake(xErrorHandler,portMAX_DELAY);
	}		
}

/*
led driver 1 ms tick (debug mode)
*/
void _task_led(void *pvParameters)
{	
	static int tick = 0, tick_reload = 1000000;
	/*config led on PB1 and PB0*/
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER |= (GPIO_MODER_MODER0_0|GPIO_MODER_MODER1_0);
	
	led_0_off;
	led_1_off;
	for(;;)
	{
		switch(device_state)
		{
			case IDLE:
				led_1_off;
				if(tick%100 == 0)
				{
				led_0_invertor();	
				}
			break;				
			case WORKING:
				led_0_on;
				led_1_off;
				break;
			case ALARM:
				led_0_off;
				led_1_on;	
				break;
			case PROGRAMMING:
				if(tick%1000 == 0)
				{
					led_1_invertor();
					led_0_invertor();
				}
				break;
		}
		/*tick update*/
		if(tick <= tick_reload)
		{
			tick++;
		}
		else
		{
			tick = 0;
		}	
		vTaskDelay(1);
	}
}

/*main cycle 1 ms tick*/
void _task_main(void *pvParameters)
{	
	static uint8_t tick = 0,tick_reload = 100;
	static const uint8_t adc_timing = 5,freq_timing = 25,i2c_update = 50;
	ADC_on;
	EnableEXTI_Interupts();
	
	if(LCD_ENABLE)
	{
		LCD_Init1602();
		LCD_SetLoadingWindow();
		LCD_ClearDisplay();
		LCD_DrawWorkspace();
	}
	for(;;){
		//power_factor_conversion();
		
		
		if(tick%adc_timing == 0)
		{
			adc_conversion();			/*adc conversion*/  
		}
		if(tick%freq_timing == 0)
		{
			frequency_conversion();		/*frequency and phase failure conversion*/
		}
		if(LCD_ENABLE)
		{
			if(tick%i2c_update == 0)
			{	
				text_ascii_conversion();
				i2c_transfer();	
			}
		}
		/*tick update*/
		if(tick < tick_reload)
		{
			tick++;
		}
		else
		{
			tick = 0;
		}
		vTaskDelay(1);
	}
}

void frequency_conversion(){
	
	DisableGeneralTimers();	
	/*get period and frequency value*/
	if(TIM15_CCR1_Array[1] > TIM15_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseA = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseA_Period = (TIM15_CCR1_Array[1]-TIM15_CCR1_Array[0]);
		TIM15->CNT = 0;
		TIM15->CCR1 = 0;
		TIM15_CCR1_Array[0]=0;
		TIM15_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseA_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseA_Period) + 1;
	}
	else{
		if(WatchDogPointer->FrequencyPhaseA > 0){
			WatchDogPointer->FrequencyPhaseA--;
		}
	}	
	if(TIM16_CCR1_Array[1] > TIM16_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseB = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseB_Period = (TIM16_CCR1_Array[1]-TIM16_CCR1_Array[0]);
		TIM16->CNT = 0;
		TIM16->CCR1 = 0;
		TIM16_CCR1_Array[0]=0;
		TIM16_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseB_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseB_Period);  
	}else{
		if(WatchDogPointer->FrequencyPhaseB > 0){
			WatchDogPointer->FrequencyPhaseB--;
		}
	}
	if(TIM17_CCR1_Array[1] > TIM17_CCR1_Array[0]){
		WatchDogPointer->FrequencyPhaseC = MotorConfigurationPointer->FrequencyWatchdog;
		CapturedPeriodPointer->PhaseC_Period = (TIM17_CCR1_Array[1]-TIM17_CCR1_Array[0]);
			TIM17->CNT = 0;
			TIM17->CCR1 = 0;
			TIM17_CCR1_Array[0]=0;
			TIM17_CCR1_Array[1]=0;
		/*get value in Hz*/
		CapturedPeriodPointer->PhaseC_Frequency = (1*TIMER_10US)/(CapturedPeriodPointer->PhaseC_Period);  
	}else{
		if(WatchDogPointer->FrequencyPhaseC > 0){
			WatchDogPointer->FrequencyPhaseC--;
		}
	}
	
	EnableGeneralTimers();	
}

void adc_conversion(){

	float temp;
	/*get ADC value in mV*/
	CapturedVoltagePointer->PhaseA_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[0]);
	if(CapturedVoltagePointer->PhaseA_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseA_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseA_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseA_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseA_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseA_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseA_Voltage_rms = 0;

	}

	CapturedVoltagePointer->PhaseB_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[1]);
	if(CapturedVoltagePointer->PhaseB_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseB_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseB_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseB_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseB_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseB_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseB_Voltage_rms = 0;

	}

	CapturedVoltagePointer->PhaseC_Voltage_peak = ADC_CalcValue(CapturedVoltageArray[2]);
	if(CapturedVoltagePointer->PhaseC_Voltage_peak > 200){
		/*include voltage drop on diode*/
		CapturedVoltagePointer->PhaseC_Voltage_peak += (uint32_t)DIODE_DROP;
		/*input voltage divider*/
		CapturedVoltagePointer->PhaseC_Voltage_peak *= VOLTAGE_PREDIV;
		
		/*get rms voltage for display (demo) */
		temp = (float)CapturedVoltagePointer->PhaseC_Voltage_peak;
		temp = temp * 0.707;
		CapturedVoltagePointer->PhaseC_Voltage_rms = ((uint32_t)temp/10);
		/**/
	}else{
		CapturedVoltagePointer->PhaseC_Voltage_peak = 0;
		CapturedVoltagePointer->PhaseC_Voltage_rms = 0;

	}
}
void power_factor_conversion(){
	
	EXTI->IMR &= PHASEMETER_DEFAULT;	
	/*phase A*/
	EXTI->IMR |= PHASEMETER_A_IRQ;
	/*wait for conversion*/
	vTaskDelay(25);
	EXTI->IMR &= ~PHASEMETER_A_IRQ;
	/*add check Km function*/
	//PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,CapturedPeriodPointer->PhaseA_Period);
	PowerFactorPointer->PhaseA_Cos = CalcPowerFactor(PowerFactorPointer->PhaseA_Factor,1818);
	//PowerFactorPointer->PhaseA_Cos=45;
	if(PowerFactorPointer->PhaseA_Factor>150)
	{
		BLUE_LED_ON;
	}
//	/*---------------------*/	
//	/*phase B*/
//	EXTI->IMR |= PHASEMETER_B_IRQ;
//	/*wait for conversion*/
//	vTaskDelay(25);	
//	EXTI->IMR &= ~PHASEMETER_B_IRQ;
//	PowerFactorPointer->PhaseB_Cos = CalcPowerFactor(PowerFactorPointer->PhaseB_Factor,CapturedPeriodPointer->PhaseB_Period);
//	/*---------------------*/
//	/*phase C*/
//	EXTI->IMR |= PHASEMETER_C_IRQ;
//	/*wait for conversion*/
//	vTaskDelay(25);
//	EXTI->IMR &= ~PHASEMETER_C_IRQ;
//	PowerFactorPointer->PhaseC_Cos = CalcPowerFactor(PowerFactorPointer->PhaseC_Factor,CapturedPeriodPointer->PhaseC_Period);
//	/*---------------------*/
	EXTI->IMR &= PHASEMETER_DEFAULT;
}

/*create text arrays for lcd*/
void text_ascii_conversion(){
	
	/*convert adc voltage rms value*/
	itoa(CapturedVoltagePointer->PhaseA_Voltage_rms,VoltageTextLCDPointer->PhaseA_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseB_Voltage_rms,VoltageTextLCDPointer->PhaseB_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	itoa(CapturedVoltagePointer->PhaseC_Voltage_rms,VoltageTextLCDPointer->PhaseC_VoltageArray,DEFAULT_VOLTAGE_BUF_SIZE);
	
	/*convert frequency value*/
	itoa(CapturedPeriodPointer->PhaseA_Frequency,PeriodLCDPointer->PhaseA_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	itoa(CapturedPeriodPointer->PhaseB_Frequency,PeriodLCDPointer->PhaseB_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	itoa(CapturedPeriodPointer->PhaseC_Frequency,PeriodLCDPointer->PhaseC_FrequencyArray,DEFAULT_PERIOD_BUF_SIZE);
	
	/*convert power factor value*/
	itoa(PowerFactorPointer->PhaseA_Cos,PowerFactorLCDPointer->PhaseA_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
	itoa(PowerFactorPointer->PhaseB_Cos,PowerFactorLCDPointer->PhaseB_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
	itoa(PowerFactorPointer->PhaseC_Cos,PowerFactorLCDPointer->PhaseC_FactorArray,DEFAULT_POWER_FACTOR_BUF_SIZE);
}


void i2c_transfer(){

	uint8_t counter;
	switch(ContentSwitching){
			
			case PHASE_A:
					LCD_SetDRAM_Adress(DDRAM_adress_row_0);
					LCD_SendChar('A');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseA_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseA_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseA_FactorArray[counter]+0x30);
				}
				
			break;

			case PHASE_B:
				LCD_SetDRAM_Adress(DDRAM_adress_row_0);
				LCD_SendChar('B');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseB_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseB_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseB_FactorArray[counter]+0x30);
				}
				
			break;

			case (PHASE_C -1):
				LCD_SetDRAM_Adress(DDRAM_adress_row_0);
				LCD_SendChar('C');
				/*voltage data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_0+7);
				for(counter = 0 ; counter < DEFAULT_VOLTAGE_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(VoltageTextLCDPointer->PhaseC_VoltageArray[counter]+0x30);
				}
				/*frequency data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+4);
				for(counter = 0 ; counter < DEFAULT_PERIOD_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PeriodLCDPointer->PhaseC_FrequencyArray[counter]+0x30);
				}
				/*power factor data*/
				LCD_SetDRAM_Adress(DDRAM_adress_row_1+11);
				for(counter = 0 ; counter < DEFAULT_POWER_FACTOR_BUF_SIZE; counter++){
					/*phase A */
					LCD_SendChar(PowerFactorLCDPointer->PhaseC_FactorArray[counter]+0x30);
				}
	
			break;

			default:

			break;
		}

}

void send_usart_message(uint8_t *message,uint32_t buf_size)
{
	DMA_Ch4_Reload(message,buf_size);
}


/*EXTI line 0 and 1 interrupt handler*/
void EXTI0_1_IRQHandler(){
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	
	/*interrupt on PA0, user button */
	if(EXTI->PR & EXTI_PR_PR0){
		
		/*switch display image */
		if(ContentSwitching < 3){
			ContentSwitching++;
		}else{
			ContentSwitching = 1;
		}
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR0;
	}
	/*start measuring the phase shift for phase A */
	/*interrupt on PC1 */
	if(EXTI->PR & EXTI_PR_PR1){			
		/*start timer for fhase A shift*/
		PHASEMETER_A_START;
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR1;
	}
		
	
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 2 and 3 interrupt handler*/
void  EXTI2_3_IRQHandler(){
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start measuring the phase shift for phase B*/

	/*interrupt on PC2 */
	if(EXTI->PR & EXTI_PR_PR2){
		
		/*stop timer for fhase A shift*/
		PHASEMETER_A_STOP;
		if(PHASEMETER_A_VALUE > 0){
			/*get value from timer in us*/
			PowerFactorPointer->PhaseA_Factor = PHASEMETER_A_VALUE;
			/*clear buffer*/
			PHASEMETER_A_VALUE = 0;	
		}
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR2;
	}

	/*interrupt on PC3 */
	if(EXTI->PR & EXTI_PR_PR3){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR3;
	}
			
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}

/*EXTI line 4 and 5 interrupt handler*/
void EXTI4_15_IRQHandler(){
	
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*start and stop measuring the phase shift for phase C and stop for measuring phase B */

	/*interrupt on PC4 */
	if(EXTI->PR & EXTI_PR_PR4){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR4;
	}

	/*interrupt on PC5 */
	if(EXTI->PR & EXTI_PR_PR5){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR5;
	}
	
	/*interrupt on PC6 */
	if(EXTI->PR & EXTI_PR_PR5){
		
		/*reset interrupt trigger*/
		EXTI->PR |= EXTI_PR_PR6;
	}

	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
}
void LCD_DrawWorkspace()
{
	LCD_SetDRAM_Adress(DDRAM_adress_row_0 + 2);
	LCD_WriteLine("Vrms=");
	LCD_SetDRAM_Adress(DDRAM_adress_row_1 + 2);
	LCD_WriteLine("F=");
	LCD_SetDRAM_Adress(DDRAM_adress_row_1 + 8);
	LCD_WriteLine("Km=");
}

void LCD_SetLoadingWindow()
{
	LCD_SetDRAM_Adress(DDRAM_adress_row_0);
	LCD_WriteLine("....LOADING....");
	DELAY(150);
	LCD_SetDRAM_Adress(DDRAM_adress_row_1);
	for(int i = 0 ; i < 16; i++)
	{
		LCD_WriteLine(".");
		DELAY(150);
	}
}
