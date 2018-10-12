#ifndef PCF8574_H
#define PCF8574_H

#include "stm32f0xx_i2c.h"

/*delay implementation*/
#define DELAY   vTaskDelay

#define PCF8574_ADRESS  0x4E

/*setup commands for lcd1602 module*/
#define CMD1    0x34,0x30               /*8 bit interface*/           
#define CMD2    0x24,0x20               /*switch to 4 bit interface*/
#define CMD3    0x24,0x20,0x84,0x80     /*4 bit interface, use two rows*/
#define CMD4    0x04,0x00,0x64,0x60     /*display off*/
#define CMD5    0x04,0x00,0xC4,0xC0     /*display on*/
#define CMD6    0x04,0x00,0x14,0x10     /*clear display*/
#define CMD7    0x04,0x00,0x04,0x00     /*zero byte*/
#define CMD8    0x14,0x10,0x44,0x40     /*cursor moving right*/
#define CMD9    0xCC,0xC8,0x7C,0x78     /*display led on*/

extern uint_least8_t Init_LCD_1602(void);
extern uint_least8_t LCD_Reset(void);
extern uint_least8_t LCD_WriteText(uint8_t DRAM_adress,uint8_t *data);

#endif