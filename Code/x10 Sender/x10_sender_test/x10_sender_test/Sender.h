/*
 * Sender.h
 *
 * Created: 02/05/2019 20:25:55
 *  Author: oskar
 */ 


#ifndef SENDER_H_
#define SENDER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

enum commands
{
	ON = 0b00101, OFF = 0b00111
};


class Sender
{
private:
	char m_houseCode;
	bool m_isZeroCross;
	
public:
	Sender(char houseCode);
	
	void setZeroCross(bool isZeroCross);
	void waitForZeroCross();
	void sendBit(char bitVal);
	void sendStart();
	void sendCommand(commands toSend, unsigned char toUnit);
	
};


#endif /* SENDER_H_ */