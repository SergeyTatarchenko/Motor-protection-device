#include "util.h"
#include "stdlib.h"

/*************************************************
Convert integer to array
return number bit
*************************************************/
uint16_t itoa(int num, uint8_t *buff)
{
	uint16_t i = 0;
	uint16_t j = 0;
	uint8_t temp = 0;
	char sig = ' ';
	
	if(num < 0) 
	{
		num = abs(num);
		sig = '-';
	}
	
	do
	{
		*(buff+i) = '0' + num%10;
		num /= 10;
		i++;
	}while(num > 0);
	
	
	if(sig == '-')
	{
		*(buff+i) = sig;
	}else{
		i--;
	}
	
//	
	for(j = i; j > i/2; j--)
	{
		temp = *(buff + j);
		*(buff + j) = *(buff + i - j);
		*(buff + i - j) = temp;
	}
	i++;
	*(buff+i) = '\0';
	
	return i;
}
