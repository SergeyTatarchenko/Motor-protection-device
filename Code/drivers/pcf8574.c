/*
	Driver for pcf8574 with lcd1602 or other;
	version 1.1
*/

#include "pcf8574.h"

/*lcd 1602 basic configuration for Arduino i2c lcd module */
uint_least8_t Init_LCD_1602(void)
{
	uint_least8_t state;
	/**/
	uint8_t command_1[]={CMD1};
	/**/
	uint8_t command_2[]={CMD2};
	/**/
	uint8_t command_3[]={CMD3};
	/**/
	uint8_t command_4[]={CMD4};
	/**/
	uint8_t command_5[]={CMD5};
	/**/
	uint8_t command_6[]={CMD6};
	/**/
	uint8_t command_7[]={CMD7};
	/**/
	uint8_t command_8[]={CMD8};
	/**/
	uint8_t command_9[]={CMD9};
 	
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

/* lcd reset and clean */
uint_least8_t LCD_Reset()
{
	uint_least8_t state;
	
	return state;
}

/* clean lcd display */
uint_least8_t LCD_ClearDisplay()
{
	
	uint8_t command_6[]={CMD6};
	uint_least8_t state;
	state &= I2CSendData(PCF8574_ADRESS,command_6,sizeof(command_6));
	DELAY(10);
	
	return state;
}

/*setup DRAM adress and text pointer*/
uint_least8_t LCD_SetDRAM_Adress(uint8_t DRAM_adress)
{
	
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

/*send single letter to lcd*/
void LCD_SendChar(char CharToSend){
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

/*lcd write array of chars*/
void LCD_Write(char *array){
	
	int max_line_size = 40,array_size = sizeof(array);
	
	if(array_size > max_line_size){
			return;
		}
	for(int i = 0;i < max_line_size;i++){
		if(array[i] == '\0'){
			break;
		}
		LCD_SendChar(array[i]);
	}
}
