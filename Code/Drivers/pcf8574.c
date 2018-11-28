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
static uint8_t _A_[]={LETTER_A};
static uint8_t _B_[]={LETTER_B};
static uint8_t _C_[]={LETTER_C};

static uint8_t _L_[]={LETTER_L};
static uint8_t _O_[]={LETTER_O};
static uint8_t _D_[]={LETTER_D};
static uint8_t _I_[]={LETTER_I};
static uint8_t _N_[]={LETTER_N};
static uint8_t _G_[]={LETTER_G};

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
uint_least8_t LCD_ClearDisplay(void){
	
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
	uint8_t VoltageBuf[4];
	temp = CharToSend;
	
	/*clear buffer*/
	VoltageBuf[0] = 0x0D;
	VoltageBuf[1] = 0x09;
	VoltageBuf[2] = 0x0D;
	VoltageBuf[3] = 0x09;
	
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
	vTaskDelay(2);
}

uint_least8_t LCD_DrawWorkspace(){
	
	uint_least8_t state;
	LCD_SetDRAM_Adress(0x01);
	state =  I2CSendData(PCF8574_ADRESS,_A_,sizeof(_A_));
	DELAY(3);
	LCD_SetDRAM_Adress(0x06);
	state =  I2CSendData(PCF8574_ADRESS,_B_,sizeof(_B_));
	DELAY(3);
	LCD_SetDRAM_Adress(0x0B);
	state =  I2CSendData(PCF8574_ADRESS,_C_,sizeof(_C_));
	DELAY(3);
	return state;

}

uint_least8_t LCD_DrawBootWindow(){
	
	uint_least8_t state;
	LCD_SetDRAM_Adress(0x03);
	state =  I2CSendData(PCF8574_ADRESS,_L_,sizeof(_L_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_O_,sizeof(_O_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_A_,sizeof(_A_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_D_,sizeof(_I_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_I_,sizeof(_I_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_N_,sizeof(_N_));
	DELAY(3);
	state =  I2CSendData(PCF8574_ADRESS,_G_,sizeof(_G_));
	DELAY(3);
	return state;
	
}
