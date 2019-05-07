/*
 * x10_sender_test.cpp
 *
 * Created: 02-05-2019 15:06:07
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Sender.h"

Sender x10Sender(0b0110);


// 1 ms after 120 kHz signal was started.
ISR(TIMER1_OVF_vect)
{
	// Turn off 120 kHz signal and timer.
	TCCR0B = 0b00000000;
	TCCR1B = 0b00000000;
	
	// Make sure output is low.
	DDRB  &= 0b01111111;
}

// 2.778 ms after zero cross.
ISR(TIMER3_OVF_vect)
{
	// Turn off timer and initiate zero crossing.
	TCCR3B = 0b00000000;
	x10Sender.zeroCross();
}

// 5.556 ms after zero cross.
ISR(TIMER4_OVF_vect)
{
	// Turn off timer and initiate zero crossing.
	TCCR4B = 0b00000000;
	x10Sender.zeroCross();
}

// Interrupt leg detects zero cross.
ISR(INT0_vect)
{
	// Initiate zero crossing.
	x10Sender.zeroCross();
	
	// Turn on 2.778 ms timer.
	TCCR3B = 0b00000001;
	TCNT3  = 0xFFFF - 44447;
	
	// Turn on 5.556 ms timer.
	TCCR4B = 0b00000011;
	TCNT4  = 0xFFFF - 1388;
}


int main(void)
{
	sei();
	
	// Setup timer0 to transmit 120 kHz signal.
	TCCR0A = 0b01000010;
	TCCR0B = 0b00000000;
	OCR0A  = 66;
	
	// Setup timer1 for 1 ms delay.
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000000;
	TIMSK1 = 0b00000001;
	
	// Setup timer3 for 2.778 ms delay.
	TCCR3A = 0b00000000;
	TCCR3B = 0b00000000;
	TIMSK3 = 0b00000001;
	
	// Setup timer4 for 5.556 ms delay.
	TCCR4A = 0b00000000;
	TCCR4B = 0b00000000;
	TIMSK4 = 0b00000001;
	
	// Setup int0 for Zero-Cross Detection
	EICRA = 0b00000001;
	EIMSK = 0b00000001;
	
	DDRA = 0x00;
	DDRD = 0x00;
	
	// 1110 01101001 01101001 01 1110 01101001 01101001 01 000000
    
    while (1) 
    {
		if ((PINA & 1) == 0)
			x10Sender.sendCommand(ON, 0b0110);
			
		if ((PINA & 2) == 0)
			x10Sender.sendCommand(OFF, 0b0110);
    }
}

