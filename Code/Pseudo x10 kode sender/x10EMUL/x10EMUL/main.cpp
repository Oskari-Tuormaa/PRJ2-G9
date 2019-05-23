/*
 * x10EMUL.cpp
 *
 * Created: 21/05/2019 14:58:30
 * Author : oskar
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define ARRAY_LENGTH 100

//char code[] = "1110011010010110100101111001101001011010010100000011100110100101011001101110011010010101100110000000"; // ON
char code[] = "1110011010010110100101111001101001011010010100000011100110100101011010101110011010010101101010000000"; // OFF
unsigned char index = 101;


ISR(INT0_vect)
{
	if (code[index] == '0')
	{
		PORTB &= ~(1<<5);
		_delay_ms(1);
		PORTB |= (1<<5);
	}
	
	if (!(index > ARRAY_LENGTH))
		index++;
}

ISR(INT1_vect)
{
	index = 0;
}

int main(void)
{
	sei();
	
	DDRB = 0xff;
	PORTB |= (1<<5);
	
	EICRA = 0b00001010;
	EIMSK = 0b00000011;
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

