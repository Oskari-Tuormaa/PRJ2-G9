/*
 * GccApplication1.cpp
 *
 * Created: 21/05/2019 15:50:58
 * Author : oskar
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = 0xff;
	
    /* Replace with your application code */
    while (1) 
    {
		PORTB = 0xff;
		_delay_ms(1000);
		PORTB = 0x00;
		_delay_ms(1000);
    }
}

