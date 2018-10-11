/*************************************************
* File Name          : mcp23x17.c
* Author             : Tatarchenko S.
* Version            : v 1.0
* Description        : mcp23x17 i2c driver
*************************************************/
#include "mcp23x17.h"
/*-----------local define-----------------------*/

/*-----------local variables--------------------*/
EXP_GPIO_PortConfig EXP_GPIO_CONFIG;
EXP_GPIO_PortConfig *PortConfig;

EXP_GPIO_PortIO EXP_GPIO_A;
EXP_GPIO_PortIO *PortA;
EXP_GPIO_PortIO EXP_GPIO_B;
EXP_GPIO_PortIO *PortB;

EXP_GPIO_PortState EXP_STATE_A;
EXP_GPIO_PortState *PortAState;
EXP_GPIO_PortState EXP_STATE_B;
EXP_GPIO_PortState *PortBState;

/*************************************************
init mcp23x17 
*************************************************/
void MCP23x17_Init(void){	
	uint8_t data[2];
	/*config array*/
	uint8_t config [8];
	
	PortConfig = &EXP_GPIO_CONFIG;	
	
	PortA = &EXP_GPIO_A;
	PortB = &EXP_GPIO_B;
	
	PortAState = &EXP_STATE_A;
	PortBState = &EXP_STATE_B;
	
	/*default config PORTB (0-3) is input
	PORTA (0-3) is output
	PORTB (4-7) is unused (NC on PCB)*/
	
	/* config registers, segregated mapping,
	   set bit BANK = 1*/
	PortConfig->IOCON = 0x80;
	data[0] = IOCONB_ADDR0;
	data[1] = PortConfig->IOCON;
	I2CSendData(MCP23017_ADRESS,data,sizeof(data));
	
	/* config PortA,  pins 0-7 config as output,
	   not inverted output, disable interrupt,
	   enable pull up resistor on pins 0-7;	 
		 */
	PortConfig->IODIR   = 0x00;
	PortConfig->IPOL    = 0x00;
	PortConfig->GPINTEN = 0x00;
	PortConfig->DEFVAL  = 0x00;
	PortConfig->INTCON  = 0x00;
	PortConfig->IOCON   = 0x82;
	PortConfig->GPPU    = 0xFF;
	
	config[0] = IODIRA_ADDR1;
	memcpy(config+1,PortConfig,sizeof(config));
	I2CSendData(MCP23017_ADRESS,config,sizeof(config));
	
	/* config PortB, pins 0-3 is input
	   not inverted input, enable interrupts,pins compare
	   against previous value, pull up enable 
	     */
	PortConfig->IODIR   = 0x0F;
	PortConfig->IPOL    = 0x00;
	PortConfig->GPINTEN = 0x0F;
	PortConfig->DEFVAL  = 0x00;
	PortConfig->INTCON  = 0x00;
	PortConfig->IOCON   = 0x82;
	PortConfig->GPPU    = 0xFF;
	config[0] = IODIRB_ADDR1;
	
	memcpy(config+1,PortConfig,sizeof(config));
	I2CSendData(MCP23017_ADRESS,config,sizeof(config));
}
/*************************************************
get global state output pin mcp23x17 
*************************************************/
_Bool MCP23x17_GetState(int Port){
	_Bool state;
	uint8_t data[2] ={0,0};
	/* forced copy state of chosen port  */
	switch(Port){
		case PORTA:
			
			state = I2CGetData(MCP23017_ADRESS,INTFA_ADDR1,data,sizeof(data));
		    memcpy(PortAState,data,sizeof(data));
			state &= I2CGetData(MCP23017_ADRESS,GPIOA_ADDR1,data,sizeof(data));
		    memcpy(PortA,data,sizeof(data));
			break;
		case PORTB:
			state = I2CGetData(MCP23017_ADRESS,INTFB_ADDR1,data,sizeof(data));
			memcpy(PortBState,data,sizeof(data));	
			state &= I2CGetData(MCP23017_ADRESS,GPIOB_ADDR1,data,sizeof(data));
			memcpy(PortB,data,sizeof(data));	
		break;
		default:
			break;
	}
	return state;
}
/*************************************************
set state output pin mcp23x17 
*************************************************/
_Bool MCP23x17_SetOutPin(EXP_GPIO_PortIO *pointer,int bit,int bit_state){
	/*output array*/
	uint8_t output [3];
	_Bool state = 0;
	
	switch(bit_state){
		case 1:
			pointer->OLAT |= (1<<bit);
			break;
		case 0:
			pointer->OLAT &= ~(1<<bit);
			break;
		default:
			pointer->OLAT &= ~(1<<bit);
			break;
	}
	output[0] = GPIOA_ADDR1;
	memcpy(output+1,pointer,sizeof(output)-1);
	state = I2CSendData(MCP23017_ADRESS,output,sizeof(output));
	return state;
}
/*************************************************
set state output byte mcp23x17 
*************************************************/
_Bool MCP23x17_SetOutByte(EXP_GPIO_PortIO *pointer,uint8_t byte){
	/*output array*/
	uint8_t output [3];
	_Bool state = 0;
	pointer->OLAT = byte;
	output[0] = GPIOA_ADDR1;
	memcpy(output+1,pointer,sizeof(output)-1);
	state = I2CSendData(MCP23017_ADRESS,output,sizeof(output));
	return state;
}

