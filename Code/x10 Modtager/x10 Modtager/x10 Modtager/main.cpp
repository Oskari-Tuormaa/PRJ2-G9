/*
 * GccApplication1.cpp
 *
 * Created: 20/05/2019 20:16:18
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "x10Reciever.h"


x10Reciever x10rec(0b0110, 0b0110);


// 3.333 ms after zero cross.
ISR(TIMER3_OVF_vect)
{
	// Turn off timer and read.
	TCCR3B = 0b00000000;
	x10rec.read();
}

// 6.667 ms after zero cross.
ISR(TIMER4_OVF_vect)
{
	// Turn off timer and read.
	TCCR4B = 0b00000000;
	x10rec.read();
}

// Interrupt leg detects zero cross.
ISR(INT0_vect)
{
	// Initiate read.
	x10rec.read();
	
	// Turn on 3.333 ms timer.
	TCCR3B = 0b00000001;
	TCNT3  = 0xFFFF - 53333;
	
	// Turn on 6.667 ms timer.
	TCCR4B = 0b00000011;
	TCNT4  = 0xFFFF - 1666;
}

int main(void)
{
	sei();
	
	// Setup timer3 for 2.778 ms delay.
	TCCR3A = 0b00000000;
	TCCR3B = 0b00000000;
	TIMSK3 = 0b00000001;
	
	// Setup timer4 for 5.556 ms delay.
	TCCR4A = 0b00000000;
	TCCR4B = 0b00000000;
	TIMSK4 = 0b00000001;
	
	// Setup int0 for Zero-Cross Detection
	EICRA = 0b00000010;
	EIMSK = 0b00000001;
	
	DDRA = 0x00;
	DDRB = 0x00;
	DDRD = 0x00;
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

