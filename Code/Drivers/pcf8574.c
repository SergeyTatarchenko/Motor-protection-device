#include "pcf8574.h"

/**/
static uint8_t command_1[]={CMD1};
static uint8_t command_2[]={CMD2};
static uint8_t command_3[]={CMD3};
static uint8_t command_4[]={CMD4};
static uint8_t command_5[]={CMD5};
static uint8_t command_6[]={CMD6};
static uint8_t command_7[]={CMD7};
static uint8_t command_8[]={CMD8};
static uint8_t command_9[]={CMD9};

/*lcd basic configuration */
uint_least8_t Init_LCD_1602(void){
	
	uint_least8_t state;
	DELAY(100);
	state =  I2CSendData(PCF8574_ADRESS,command_1,sizeof(command_1));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_2,sizeof(command_2));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_3,sizeof(command_3));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_4,sizeof(command_4));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_5,sizeof(command_5));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_6,sizeof(command_6));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_7,sizeof(command_7));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_8,sizeof(command_8));
	DELAY(10);
	state &= I2CSendData(PCF8574_ADRESS,command_9,sizeof(command_9));
	DELAY(10);
	return state;
}

uint_least8_t LCD_Reset(){
	uint_least8_t state;
	
	return state;
}
uint_least8_t LCD_ClearDisplay(){
	
	uint_least8_t state;
	
	state &= I2CSendData(PCF8574_ADRESS,command_6,sizeof(command_6));
	DELAY(10);
	
	return state;
}

uint_least8_t LCD_SetDRAM_Adress(uint8_t DRAM_adress){
	
	uint8_t buf[4] = {0x0C,0x08,0x0C,0x08};
	uint8_t letter;
	uint8_t temp;
	uint8_t state;
	
	letter = DRAM_adress;
	letter |= 0x80;
	
	temp = letter;
	/*high part of byte*/
	temp &= 0xF0;
	buf[0] |= temp;
	buf[1] |= temp;
	
	temp = letter;
	/*low part of byte*/
	temp &= 0x0F;
	buf[2] |= temp<<4;
	buf[3] |= temp<<4;
	
	state = I2CSendData(PCF8574_ADRESS,buf,sizeof(buf));
	DELAY(1);
	return state;
}

void LCD_SendChar(uint8_t CharToSend){
	uint8_t temp;
	/*vatiable for transmit */
	uint8_t VoltageBuf[4] = {0x0D,0x09,0x0D,0x09};
	temp = CharToSend;
		
	/*high part of byte*/
	temp &= 0xF0;
	VoltageBuf[0] |= temp;
	VoltageBuf[1] |= temp;
	
	temp = CharToSend;
	/*low part of byte*/
	temp &= 0x0F;
	VoltageBuf[2] |= temp<<4;
	VoltageBuf[3] |= temp<<4;

	I2CSendData(PCF8574_ADRESS,VoltageBuf,sizeof(VoltageBuf));
	DELAY(2);
}

void LCD_DrawWorkspace(){
		
	LCD_SetDRAM_Adress(0x01);
	LCD_SendChar('A');
	LCD_SendChar('=');
	LCD_SetDRAM_Adress(0x06);
	LCD_SendChar('v');
	LCD_SetDRAM_Adress(0x08);
	LCD_SendChar('T');
	LCD_SendChar('=');
	LCD_SetDRAM_Adress(0x0D);
	LCD_SendChar('H');
	LCD_SendChar('z');
	LCD_SetDRAM_Adress(0x41);
	LCD_SendChar('c');
	LCD_SendChar('o');
	LCD_SendChar('s');
	LCD_SendChar('f');
	LCD_SendChar('=');
}

void LCD_DrawBootWindow(){

	LCD_SetDRAM_Adress(0x05);
	LCD_SendChar('B');
	LCD_SendChar('O');
	LCD_SendChar('O');
	LCD_SendChar('T');
}
