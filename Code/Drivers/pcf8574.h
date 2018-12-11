#ifndef PCF8574_H
#define PCF8574_H

#include "stm32f0xx_i2c.h"
#include "motor_protection.h"
/************************/
#include "FreeRTOS.h"
#include "task.h"
/*delay implementation*/
#define DELAY   vTaskDelay
/************************/

#define PCF8574_ADRESS  0x27

#define DDRAM_adress_row_0	0x00
#define DDRAM_adress_row_1	0x40

/*setup commands for lcd1602 module*/
#define CMD1    0x34,0x30               /*8 bit interface*/           
#define CMD2    0x24,0x20               /*switch to 4 bit interface*/
#define CMD3    0x24,0x20,0x84,0x80     /*4 bit interface, use two rows*/
#define CMD4    0x04,0x00,0x64,0x60     /*display off*/
#define CMD5    0x04,0x00,0xC4,0xC0     /*display on*/
#define CMD6    0x0C,0x08,0x1C,0x18     /*clear display*/
#define CMD7    0x04,0x00,0x04,0x00     /*zero byte*/
#define CMD8    0x14,0x10,0x44,0x40     /*cursor moving right*/
#define CMD9    0x8C,0x88,0x0C,0x08     /*display led on, set DRAM adress 0x00*/

#define LETTER_A    0x4D,0x49,0x1D,0x19     /*A*/
#define LETTER_B    0x4D,0x49,0x2D,0x29     /*B*/
#define LETTER_C    0x4D,0x49,0x3D,0x39     /*C*/
#define LETTER_L    0x4D,0x49,0xCD,0xC9     /*L*/
#define LETTER_O    0x4D,0x49,0xFD,0xF9     /*O*/
#define LETTER_D    0x4D,0x49,0x4D,0x49     /*D*/
#define LETTER_I    0x4D,0x49,0x9D,0x99     /*I*/
#define LETTER_N    0x4D,0x49,0xED,0xE9     /*N*/
#define LETTER_G    0x4D,0x49,0x7D,0x79     /*G*/


#define CHAR_TCH    0x2D,0x29,0xED,0xE9     /*.*/
#define CHAR_PRB    0x2D,0x29,0x0D,0x09     /* */

/*global variables*/
extern uint8_t CharArray[4];

/*service functions*/
extern uint_least8_t Init_LCD_1602(void);
extern uint_least8_t LCD_Reset(void);
extern uint_least8_t LCD_SetDRAM_Adress(uint8_t DRAM_adress);
extern void LCD_SendChar(uint8_t CharToSend);
extern uint_least8_t LCD_ClearDisplay(void);

/*user functions*/
extern uint_least8_t LCD_DrawWorkspace(void);
extern uint_least8_t LCD_DrawBootWindow(void);

#endif
