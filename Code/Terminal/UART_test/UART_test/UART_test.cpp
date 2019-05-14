#include "SerialPort.h"
#include <iostream>
#include <string>

#define MAX_DATA_LENGTH 8

char port_name[] = "\\\\.\\COM12";

int main()
{
	SerialPort arduino(port_name);

	if (!arduino.isConnected())
	{
		std::cout << "Couldn't connect to Arduino\n";
		return -1;
	}

	while (arduino.isConnected())
	{
		for (int i = 0; i < 50; i++)
			std::cout << "\n";

		std::cout <<
			"Welcome to Arduino Lightcontrol!\n"
			"Please choose between these menu options:\n"
			"\t1 - Turn ON lights.\n"
			"\t2 - Turn OFF lights.\n";
		char data;
		bool validData = true;
		char toSend[1];
		std::cin >> data;

		switch (data)
		{
		case '1':
			toSend[0] = (1 << 4) + 0b0110;
			break;
		case '2':
			toSend[0] = (2 << 4) + 0b0110;
			break;
		default:
			validData = false;
		}

		arduino.writeSerialPort(toSend, MAX_DATA_LENGTH);
	}
}