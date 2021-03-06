/******************************************************************************
* File Name          : pcf8574.h
* Author             : Tatarchenko S.
* Version            : v 1.2
* Description        : header for pcf8574.c 
*******************************************************************************/
#ifndef PCF8574_H
#define PCF8574_H

#include "stdint.h"

/*----------------------------------------------------------------------------*/
#define DDRAM_adress_row_0	0x00
#define DDRAM_adress_row_1	0x40
/*-------------setup commands for lcd1602 module in 4 bit mode----------------*/
#define CMD1    0x34,0x30                                    /*8 bit interface*/
#define CMD2    0x24,0x20                          /*switch to 4 bit interface*/
#define CMD3    0x24,0x20,0x84,0x80            /*4 bit interface, use two rows*/
#define CMD4    0x04,0x00,0x64,0x60                              /*display off*/
#define CMD5    0x04,0x00,0xC4,0xC0                               /*display on*/
#define CMD6    0x0C,0x08,0x1C,0x18                            /*clear display*/
#define CMD7    0x04,0x00,0x04,0x00                                /*zero byte*/
#define CMD8    0x14,0x10,0x44,0x40                      /*cursor moving right*/
#define CMD9    0x8C,0x88,0x0C,0x08     /*display led on, set DRAM adress 0x00*/
/*---------------------local function prototypes------------------------------*/

/*LCD display function  prototypes*/
uint8_t LCD_Init1602(void);
uint8_t LCD_Reset(void);
uint8_t LCD_SetDRAM_Adress(uint8_t DRAM_adress);
uint8_t LCD_ClearDisplay(void);
void    LCD_SendChar(char CharToSend);
void    LCD_WriteLine(char *array);

/*STM32F0 include i2c implementation*/
#include "stm32f0xx_i2c.h"
/************************/
/*delay implementation*/
#include "FreeRTOS.h"
#include "task.h"
#define DELAY   vTaskDelay
/************************/
#define PCF8574_ADRESS  0x27


#ifndef DELAY
#error "delay implementation not defined"
#endif



#endif
