/*
 * ArduinoUARTTest.cpp
 *
 * Created: 11-05-2019 16:43:31
 * Author : oskar
 */ 

#include <avr/io.h>
#include "uart.h"


int main(void)
{
	DDRB = 0xFF;
	
	InitUART(9600, 8, 'N');
	
    /* Replace with your application code */
    while (1) 
    {
		char command = ReadChar();
		
		switch(command)
		{
		case '0':
			PORTB = 0xFF;
			break;
		case '1':
			PORTB = 0x00;
			break;
		}
    }
}

