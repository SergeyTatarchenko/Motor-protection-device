#ifndef PCF8574_H
#define PCF8574_H

#include "stm32f0xx_i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*delay implementation*/
#define DELAY   vTaskDelay

#define PCF8574_ADRESS  0x27

#define DDRAM_adress_row_0	0x00
#define DDRAM_adress_row_1	0x40

/*setup commands for lcd1602 module*/
#define CMD1    0x34,0x30               /*8 bit interface*/           
#define CMD2    0x24,0x20               /*switch to 4 bit interface*/
#define CMD3    0x24,0x20,0x84,0x80     /*4 bit interface, use two rows*/
#define CMD4    0x04,0x00,0x64,0x60     /*display off*/
#define CMD5    0x04,0x00,0xC4,0xC0     /*display on*/
#define CMD6    0x04,0x00,0x14,0x10     /*clear display*/
#define CMD7    0x04,0x00,0x04,0x00     /*zero byte*/
#define CMD8    0x14,0x10,0x44,0x40     /*cursor moving right*/
#define CMD9    0x8C,0x88,0x0C,0x08     /*display led on, set DRAM adress 0x00*/


#define LETTER_V    0x5D,0x59,0x6D,0x69     /*V*/
#define LETTER_o    0x6D,0x69,0xFD,0xF9     /*o*/
#define LETTER_l    0x6D,0x69,0xCD,0xC9     /*l*/
#define LETTER_t    0x7D,0x79,0x4D,0x49     /*t*/
#define LETTER_a    0x6D,0x69,0x1D,0x19     /*a*/
#define LETTER_g    0x6D,0x69,0x7D,0x79     /*g*/
#define LETTER_e    0x6D,0x69,0x5D,0x59     /*e*/

extern uint_least8_t Init_LCD_1602(void);
extern uint_least8_t LCD_Reset(void);
extern uint_least8_t LCD_WriteText(void);
extern uint_least8_t LCD_WriteLetter(uint8_t *data);

#endif
