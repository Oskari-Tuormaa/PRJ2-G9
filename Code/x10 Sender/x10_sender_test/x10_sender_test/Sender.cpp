#include "Sender.h"

Sender::Sender(char houseCode)
{
	m_houseCode = houseCode;
	m_currentState = IDLE;
	
	// Setup buffer with standard template.
	char temp[101] = 
		"1110000000000000000001"
		"1110000000000000000001"
		"000000"
		"1110000000000000000010"
		"1110000000000000000010"
		"000000";
	
	for (int i = 0; i < 101; i++)
	{
		m_buffer[i] = temp[i];
	}
	
	// Loop through house code and add house code to buffer.
	for (int i = 0; i < 4; i++)
	{
		// If bit position 'i' of the house code is 1.
		if (((m_houseCode >> (3 - i)) & 1) == 1)
		{
			m_buffer[4  + i * 2] = '1';
			m_buffer[5  + i * 2] = '0';
			
			m_buffer[26 + i * 2] = '1';
			m_buffer[27 + i * 2] = '0';
			
			m_buffer[54 + i * 2] = '1';
			m_buffer[55 + i * 2] = '0';
			
			m_buffer[76 + i * 2] = '1';
			m_buffer[77 + i * 2] = '0';
		}
		// If bit position 'i' of the house code is 0.
		else
		{
			m_buffer[4  + i * 2] = '0';
			m_buffer[5  + i * 2] = '1';
			
			m_buffer[26 + i * 2] = '0';
			m_buffer[27 + i * 2] = '1';
			
			m_buffer[54 + i * 2] = '0';
			m_buffer[55 + i * 2] = '1';
			
			m_buffer[76 + i * 2] = '0';
			m_buffer[77 + i * 2] = '1';
		}
	}
}

void Sender::zeroCross()
{
	// If Unit doesn't have command to send, exit function.
	if (m_currentState == IDLE) return;
	
	// If next bit to send is '1', activate 120kHz signal.
	if (m_buffer[m_dynIndex] == '1')
	{
		TCCR0B = 0b00000001;
		TCCR1B = 0b00000001;
		TCNT1  = 0xffff - 16000;
		DDRB  |= 0b10000000;
	}
	
	// Increment dynamic index.
	m_dynIndex++;
	
	// If we've reached the end of buffer, set state to IDLE.
	if (m_dynIndex > 100)
	{
		m_currentState = IDLE;
	}
}

void Sender::sendCommand(Commands command, char unit)
{
	// If Unit is currently sending command, exit function.
	if (m_currentState == SENDING) return;
	
	// Loop through unit and command. Add both to buffer.
	for (int i = 0; i < 4; i++)
	{
		// If bit position 'i' of unit is 1.
		if (((unit >> (3 - i)) & 1) == 1)
		{
			m_buffer[12 + i * 2] = '1';
			m_buffer[13 + i * 2] = '0';
			
			m_buffer[34 + i * 2] = '1';
			m_buffer[35 + i * 2] = '0';
		}
		// If bit position 'i' of unit is 0.
		else
		{
			m_buffer[12 + i * 2] = '0';
			m_buffer[13 + i * 2] = '1';
			
			m_buffer[34 + i * 2] = '0';
			m_buffer[35 + i * 2] = '1';
		}
		
		// If bit position 'i' of command is 1.
		if (((command >> (3 - i)) & 1) == 1)
		{
			m_buffer[62 + i * 2] = '1';
			m_buffer[63 + i * 2] = '0';
			
			m_buffer[84 + i * 2] = '1';
			m_buffer[85 + i * 2] = '0';
		}
		// If bit position 'i' of command is 0.
		else
		{
			m_buffer[62 + i * 2] = '0';
			m_buffer[63 + i * 2] = '1';
			
			m_buffer[84 + i * 2] = '0';
			m_buffer[85 + i * 2] = '1';
		}
	}
	
	// Change state to SENDING.
	m_currentState = SENDING;
	
	// Reset dynamic index to 0.
	m_dynIndex = 0;
}

State Sender::getState()
{
	return m_currentState;
}

char Sender::getNext()
{
	return m_buffer[m_dynIndex];
}

