/*
 * ArduinoUARTTest.cpp
 *
 * Created: 11-05-2019 16:43:31
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_int.h"

bool locked = true;
bool active = false;


ISR(USART0_RX_vect)
{
	char command = ReadChar();
	
	switch(command >> 4)
	{
	case 1:
		SendChar('1');
		PORTB |= (1 << 0);
		break;
	case 2:
		SendChar('1');
		PORTB &= ~(1 << 0);
		break;
	case 3:
		if (locked)
			SendChar('0');
		else
		{
			active = !active;
			SendChar(active ? 'A' : 'D');
		}
		break;
	case 4:
		SendChar('1');
		locked = !locked;
		break;
	}
}

int main(void)
{
	sei();
	
	InitUART(9600, 8, 'N', 1);
	
	DDRB = 0xFF;
	
    while (1) 
    {
	    active ? (PORTB |= (1 << 1)) : (PORTB &= ~(1 << 1));
	    locked ? (PORTB |= (1 << 2)) : (PORTB &= ~(1 << 2));
    }
}

