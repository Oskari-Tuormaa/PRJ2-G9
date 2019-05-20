/*
* x10Reciever.h
*
* Created: 20/05/2019 20:38:35
* Author: oskar
*/


#ifndef __X10RECIEVER_H__
#define __X10RECIEVER_H__

#include <avr/io.h>


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
	
	char m_sBuffer;
	char m_data[2];
	char m_suffix;
	
	char m_count;
	
public:
	x10Reciever(unsigned char houseCode, unsigned char unitNum);
	void read();
	void execute(Command command);

}; //x10Reciever

#endif //__X10RECIEVER_H__
