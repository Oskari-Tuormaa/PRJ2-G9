/*
 * x10EMUL.cpp
 *
 * Created: 21/05/2019 14:58:30
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define ARRAY_LENGTH 100

//char code[] = "1110011010010110100101111001101001011010010100000011100110100101011001101110011010010101100110000000"; // ON
char code[] = "1110011010010110100101111001101001011010010100000011100110100101011010101110011010010101101010000000"; // OFF
unsigned char index = 101;


ISR(INT0_vect)
{
	if (code[index] == '1')
		PORTB |= (1<<5);
	else
		PORTB &= ~(1<<5);
	
	if (!(index > ARRAY_LENGTH))
	{
		DDRB = 0xff;
		index++;
	} else
	{
		DDRB = 0x00;
	}
}

ISR(INT1_vect)
{
	index = 0;
}

int main(void)
{
	sei();
	
	DDRB = 0xff;
	
	EICRA = 0b00001010;
	EIMSK = 0b00000011;
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

