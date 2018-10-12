#include "stm32f0xx.h"
#include "stm32f0xx_i2c.h"
#include "mcp23x17.h"
#include "pcf8574.h"

int main(){
	
	I2CInit();
	Init_LCD_1602();
	
	return 0;	
}
