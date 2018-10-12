#include "pcf8574.h"

static uint8_t command_1[]={CMD1};
static uint8_t command_2[]={CMD2};
static uint8_t command_3[]={CMD3};
static uint8_t command_4[]={CMD4};
static uint8_t command_5[]={CMD5};
static uint8_t command_6[]={CMD6};
static uint8_t command_7[]={CMD7};
static uint8_t command_8[]={CMD8};
static uint8_t command_9[]={CMD9};

uint_least8_t Init_LCD_1602(void){

	I2CSendData(PCF8574_ADRESS,command_1,sizeof(command_1));
//  DELAY(5);
//	I2CSendData(PCF8574_ADRESS,command_2,sizeof(command_2));
}

uint_least8_t LCD_Reset(void){

}

uint_least8_t LCD_WriteText(uint8_t DRAM_adress,uint8_t *data){

}
