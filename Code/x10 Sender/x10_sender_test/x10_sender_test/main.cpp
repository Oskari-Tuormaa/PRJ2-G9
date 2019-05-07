/*
 * x10_sender_test.cpp
 *
 * Created: 02-05-2019 15:06:07
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Sender.h"


ISR(INT0_vect)
{
	TCNT2 = 0;
	TCCR2B = 0b00001001;
}

ISR(TIMER1_OVF_vect)
{
	TCCR0B = 0b00000000;
}

int main(void)
{
	Sender x10Sender(0b0110);
	
	DDRD |= 0b11111110;
	
	// Setup Zero Cross Delay Timer
	TCCR2A = 0b00000000;
	TCCR2B = 0b00001000;
	OCR2A = 44448;
	
	// Setup Zero Cross Interrupt
	EICRA = 0b00000000;
	EIMSK = 0b00000001;
    
    while (1) 
    {
		
    }
}

