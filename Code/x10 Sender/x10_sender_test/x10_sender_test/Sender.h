#ifndef SENDER_H_
#define SENDER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

enum Commands
{
	ON = 0b0010, OFF = 0b0011
};

enum State
{
	IDLE, SENDING
};


class Sender
{
private:
	char  m_houseCode;
	State m_currentState;
	char  m_buffer[101];
	char  m_dynIndex;
	
public:
	Sender(char houseCode);
	
	void zeroCross();
	void sendCommand(Commands command, char unit);
	
	State getState();
	char getNext();
};


#endif /* SENDER_H_ */