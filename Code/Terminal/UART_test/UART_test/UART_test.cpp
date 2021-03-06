#include "SerialPort.h"
#include <iostream>
#include <string>

#define MAX_DATA_LENGTH 8

char port_name[] = "\\\\.\\COM12";
char incomingData[MAX_DATA_LENGTH];
bool active = false;

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
		std::cout <<
			"Welcome to Arduino Lightcontrol!\n"
			"Please choose between these menu options:\n"
			"\t1 - Turn ON lights.\n"
			"\t2 - Turn OFF lights.\n"
			"\t3 - Turn " << (active ? "OFF" : "ON") << " system.\n";
		char data;
		bool validInput = true;
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
		case '3':
			toSend[0] = (3 << 4) + 0b0000;
			break;
		default:
			validInput = false;
		}

		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

		if (validInput)
		{
			arduino.writeSerialPort(toSend, MAX_DATA_LENGTH);

			Sleep(10);

			int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);

			switch (*incomingData)
			{
			case '0':
				std::cout << "Please insert security code before accessing locked controls.\n";
				break;
			case 'A':
				active = true;
				break;
			case 'D':
				active = false;
				break;
			}
		}
	}
}