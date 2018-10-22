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

/*ASCII code chart*/
static uint8_t _V_[]={LETTER_V};
static uint8_t _o_[]={LETTER_o};
static uint8_t _l_[]={LETTER_l};
static uint8_t _t_[]={LETTER_t};
static uint8_t _a_[]={LETTER_a};
static uint8_t _g_[]={LETTER_g};
static uint8_t _e_[]={LETTER_e};
static uint8_t PRB[]={CHAR_PRB};

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

uint_least8_t LCD_Reset(void){
	uint_least8_t state;
	return state;
}
uint_least8_t LCD_WriteText(){
	uint_least8_t state;
	
	state =  I2CSendData(PCF8574_ADRESS,_V_,sizeof(_V_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_o_,sizeof(_o_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_l_,sizeof(_l_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_t_,sizeof(_t_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_a_,sizeof(_a_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_g_,sizeof(_g_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_e_,sizeof(_e_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,PRB,sizeof(PRB));
	DELAY(3);
	
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
