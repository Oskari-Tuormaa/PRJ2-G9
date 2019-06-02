#include "x10Reciever.h"

x10Reciever::x10Reciever(unsigned char houseCode, unsigned char unitNum)
{
	m_state = IDLE;
	m_houseCode = houseCode;
	m_unitNum = unitNum;
	
	m_sBuffer = 0;
	m_data[0] = 0;
	m_data[1] = 0;
	
	m_count = 0;
}

void x10Reciever::read()
{
	_delay_ms(0.1);
	switch(m_state)
	{
	case IDLE:
		// Add new value to small buffer.
		m_sBuffer = m_sBuffer << 1;
		
		if ((PINH & (1<<5)) != 0)
		{
			m_sBuffer |= 1;
		}
		
		// If the correct start bit sequence is read, enter Recieving state.
		if ((m_sBuffer & 0b00001111) == 0b00001110)
		{
			m_state = RECIEVING;
			m_count = 0;
		}
		break;
		
	case RECIEVING:
		// Add carry from [0] to [1] in data array.
		m_data[1] = m_data[1] << 1;
		if ((m_data[0] & (1 << 7)) != 0)
			m_data[1] |= 1;
	
		// Add carry from suffix to [0] in data array.
		m_data[0] = m_data[0] << 1;
		if ((m_suffix & 0b10) != 0)
			m_data[0] |= 1;
		
		// Add new value to suffix.
		m_suffix = m_suffix << 1;
		if ((PINH & (1<<5)) != 0)
			m_suffix |= 1;
		
		// Increment counter.
		m_count++;
		
		// If both m_data and m_suffix is filled.
		if (m_count >= 18)
		{
			m_state = IDLE;
			
			// If house code doesn't match, ignore message.
			if (m_houseCode != m_data[1])
				return;
			
			switch(m_suffix & 0b11)
			{
			// Suffix == UNIT CODE.
			case 0b01:
				// If message IS for this unit.
				if (m_unitNum == m_data[0])
				{
					m_isActive = true;
					m_comCount = 0;
				}
				// If message is NOT for this unit.
				else
					m_isActive = false;
				break;
			
			// Suffix == COMMAND CODE.
			case 0b10:
				m_comCount++;
			
				// If reciever HASN'T been activated --> Message ISN'T for this unit.
				if (!m_isActive)
					return;
				
				// If this is the first recieved command, ignore.
				if (m_comCount < 2)
					return;
					
				// Do given command.
				switch (m_data[0])
				{
				case ON:
					execute(ON);
					break;
				
				case OFF:
					execute(OFF);
					break;
				}
				break;
			}
		}
		break;
	}
}

void x10Reciever::execute(Command command)
{
	switch (command)
	{
	case ON:
		PORTB = 0xff;
		break;
		
	case OFF:
		PORTB = 0x00;
		break;
	}
}

