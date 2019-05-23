/*
* x10Reciever.h
*
* Created: 20/05/2019 20:38:35
* Author: oskar
*/


#ifndef __X10RECIEVER_H__
#define __X10RECIEVER_H__

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"


enum Command
{
	ON  = 0b01011001,
	OFF = 0b01011010
};

enum State
{
	IDLE, RECIEVING
};

class x10Reciever
{
private:
	State m_state;
	unsigned char m_unitNum;
	unsigned char m_houseCode;
	bool m_isActive;
	
	unsigned char m_sBuffer;
	unsigned char m_data[2];
	unsigned char m_suffix;
	
	unsigned char m_count;
	unsigned char m_comCount;
	
public:
	x10Reciever(unsigned char houseCode, unsigned char unitNum);
	void read();
	void execute(Command command);
	
	inline unsigned char getBuff() { return m_data[0]; }

}; //x10Reciever

#endif //__X10RECIEVER_H__
