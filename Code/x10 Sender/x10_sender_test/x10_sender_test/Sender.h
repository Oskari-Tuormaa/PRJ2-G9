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

enum commands
{
	ON = 0b0010, OFF = 0b0011
};

enum state
{
	IDLE, SENDING
};


class Sender
{
private:
	char  m_houseCode;
	state m_currentState;
	char  m_buffer[100];
	char  m_dynIndex;
	
public:
	Sender(char houseCode);
	
	void zeroCross();
	void sendCommand(commands command, char unit);
	
	state getState();
	char getNext();
};


#endif /* SENDER_H_ */