/******************************************************************************
* File Name          : pcf8574.c
* Author             : Tatarchenko S.
* Version            : v 1.2
* Description        : Driver for remote 8-bit I/O expander for I2C-bus 
*******************************************************************************/
#include "pcf8574.h"
/*----------------------------------------------------------------------------*/

/*  
* name : InitLCD1602
* description : lcd 1602 basic configuration for Arduino i2c lcd module;
* 4 bit mode
*/
uint8_t LCD_Init1602( void )
{
    uint8_t state,command_1[]={CMD1},
    command_2[]={CMD2},command_3[]={CMD3},
    command_4[]={CMD4},command_5[]={CMD5},
    command_6[]={CMD6},command_7[]={CMD7},
    command_8[]={CMD8},command_9[]={CMD9};

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

/*
* name : LCD_ClearDisplay
* description : Arduino i2c lcd module: clean;
* 4 bit mode
*/
uint8_t LCD_ClearDisplay()
{
    uint8_t command_6[]={CMD6},state;
    state = I2CSendData(PCF8574_ADRESS,command_6,sizeof(command_6));
    DELAY(10);
    return state;
}

/*
* name : LCD_SetDRAM_Adress
* description : Arduino i2c lcd module: setup DRAM adress and text pointer;
* 4 bit mode
*/

uint8_t LCD_SetDRAM_Adress(uint8_t DRAM_adress)
{
    uint8_t buf[4] = {0x0C,0x08,0x0C,0x08};
    uint8_t letter,temp,state;

    letter = DRAM_adress;
    letter |= 0x80;
    temp = letter;                                         /*high part of byte*/
    temp &= 0xF0;
    buf[0] |= temp;
    buf[1] |= temp;
    temp = letter;                                          /*low part of byte*/
    temp &= 0x0F;
    buf[2] |= temp<<4;
    buf[3] |= temp<<4;
    state = I2CSendData(PCF8574_ADRESS,buf,sizeof(buf));
    DELAY(1);
    return state;
}

/*
* name : LCD_SendChar
* description : Arduino i2c lcd module: send single letter to lcd;
* 4 bit mode
*/
void LCD_SendChar(char CharToSend)
{
    uint8_t temp,buff[4] = {0x0D,0x09,0x0D,0x09};
    temp = CharToSend;
    temp &= 0xF0;                                          /*high part of byte*/
    buff[0] |= temp;
    buff[1] |= temp;
    temp = CharToSend;
    temp &= 0x0F;                                           /*low part of byte*/
    buff[2] |= temp<<4;
    buff[3] |= temp<<4;
    I2CSendData(PCF8574_ADRESS,buff,sizeof(buff));
    DELAY(2);
}

/*
* name : LCD_WriteLine
* description : Arduino i2c lcd module: lcd write array of chars;
* 4 bit mode
*/
void LCD_WriteLine(char *array)
{
    int max_line_size = 40,array_size = sizeof(array);
    if(array_size > max_line_size)
    {
        return;
    }
    for(int i = 0; i < max_line_size; i++)
    {
        if(array[i] == '\0')
        {
            break;
        }
        LCD_SendChar(array[i]);
    }
}
