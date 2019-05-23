/*
 * x10_sender_test.cpp
 *
 * Created: 02-05-2019 15:06:07
 * Author : oskar
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>

#include "Sender.h"
#include "uart_int.h"

Sender x10Sender(0b0110);
bool locked = true;
bool active = false;
bool lightOn = false;


void delay(int t)
{
	for (int i = 0; i < t; i++)
	{
		_delay_ms(1000);
	}
}

// Received UART Message
ISR(USART0_RX_vect)
{
	char com = ReadChar();
	
	switch(com >> 4)
	{
	case 1:
		SendChar('1');
		lightOn = true;
		x10Sender.sendCommand(ON , com & 0xF);
		break;
	case 2:
		SendChar('1');
		lightOn = false;
		x10Sender.sendCommand(OFF, com & 0xF);
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
		locked = !locked;
		SendChar('1');
	}
}

// 1 ms after 120 kHz signal was started.
ISR(TIMER1_OVF_vect)
{
	// Turn off 120 kHz signal and timer.
	TCCR0B = 0b00000000;
	TCCR1B = 0b00000000;
	
	// Make sure output is low.
	DDRB  &= 0b01111111;
}

// 3.333 ms after zero cross.
ISR(TIMER3_OVF_vect)
{
	// Turn off timer and initiate zero crossing.
	TCCR3B = 0b00000000;
	x10Sender.zeroCross();
}

// 6.667 ms after zero cross.
ISR(TIMER4_OVF_vect)
{
	// Turn off timer and initiate zero crossing.
	TCCR4B = 0b00000000;
	x10Sender.zeroCross();
}

// Interrupt leg detects zero cross.
ISR(INT2_vect)
{
	// Initiate zero crossing.
	x10Sender.zeroCross();
	
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
	
	InitUART(9600, 8, 'N', 1);
	
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
	EICRA = 0b00100000;
	EIMSK = 0b00000100;
	
	DDRA = 0x00;
	DDRB = 0xFF;
	DDRD = 0x00;
    
    while (1) 
    {
	    if (locked)
			PORTB |= (1<<1);
	    else
			PORTB &= ~(1<<1);
		if (active)
		{
			int t = 1 + rand() % 9;
			delay(t);
			lightOn = !lightOn;
			
			if (lightOn)
				//PORTB |= (1<<2);
				x10Sender.sendCommand(ON, 0b0110);
			else
				//PORTB &= ~(1<<2);	
				x10Sender.sendCommand(OFF, 0b0110);
		}
    }
}

