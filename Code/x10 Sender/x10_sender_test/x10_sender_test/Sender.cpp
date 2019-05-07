/*
 * Sender.cpp
 *
 * Created: 02/05/2019 20:25:46
 *  Author: oskar
 */ 

#include "Sender.h"

 Sender::Sender(char houseCode)
{
	m_houseCode = houseCode;
	m_isZeroCross = false;
	
	DDRB &= (1 << 7);
	
	sei();
}

void Sender::setZeroCross(bool isZeroCross)
{
	m_isZeroCross = isZeroCross;
}

void Sender::waitForZeroCross()
{
	while(!m_isZeroCross);
}

void Sender::sendBit(char bitVal)
{
	if (bitVal == 0)
		waitForZeroCross();
	
	waitForZeroCross();
	TCCR0A = 0b01000010;
	TCCR0B = 0b00000001;
	OCR0A = 66;
	
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000001;
	TIMSK1 = 0b00000001;
	TCNT1 = (pow(2, 16) - 1) - 16000;
	
	if (bitVal == 1)
		waitForZeroCross();
}

void Sender::sendStart()
{
	for (int i = 0; i < 3; i++)
	{
		waitForZeroCross();
		TCCR0A = 0b01000010;
		TCCR0B = 0b00000001;
		OCR0A = 66;
		
		TCCR1A = 0b00000000;
		TCCR1B = 0b00000001;
		TIMSK1 = 0b00000001;
		TCNT1 = (pow(2, 16) - 1) - 16000;
	}
	
	waitForZeroCross();
}

void Sender::sendCommand(commands toSend, unsigned char toUnit)
{
	unsigned char command = toSend;
	
	for (char i = 0; i < 2; i++)
	{
		sendStart();
		for (char j = 0; j < 4; j++)
			sendBit(m_houseCode >> j);
		
		for (char j = 0; j < 4; j++)
			sendBit(toUnit >> j);
		
		sendBit(0);
	}
	
	for (char i = 0; i < 6; i++)
		waitForZeroCross();
	
	for (char i = 0; i < 2; i++)
	{
		sendStart();
		for (char j = 0; j < 4; j++)
			sendBit(m_houseCode >> j);
		
		for (char j = 0; j < 4; j++)
			sendBit(command >> j);
		
		sendBit(0);
	}

	for (char i = 0; i < 6; i++)
		waitForZeroCross();
}
