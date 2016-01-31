// CommPort.cpp : Defines the entry point for the console application.
//

#include<windows.h>
#include<iostream>
#include<memory.h>
#include<string.h>
#include<queue>
#include "./CommPort.h"

using std::queue;
using namespace std;

HANDLE commPort = INVALID_HANDLE_VALUE;
TCHAR* myComm = TEXT("COM3");
DCB dcb;
queue <char> myQueue;

CommPort::CommPort()
{
	commPort = CreateFile(
		myComm,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
		);

	if (commPort == INVALID_HANDLE_VALUE) {

		printf("Error!!!!");
		exit(1);

	}
	else {
		printf("Successfully connected.\n\n");
	}

	dcb.DCBlength = sizeof(DCB);

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;

	/*if (!::SetCommState(commPort, &dcb)) {
		return false;
	}*/
}

void CommPort::update()
{
	DWORD nRead;
	int i = 0;
	bool flag = true;
	while (flag) {
		char buff[2];
		ReadFile(commPort, buff, 1, &nRead, NULL);
		//std::cout << buff[0];
		if (buff[0] != '\0') {
			myQueue.push(buff[0]);
		}
		i++;
		if (i == 5 * 6) {
			flag = false;
		}
	}

	std::cout << myQueue.size();

	while (!myQueue.empty()) {
		std::cout << myQueue.front();
		myQueue.pop();
	}


	CloseHandle(commPort);
	//return 0;
}

queue<char> CommPort::getActions()
{
	return queue<char>();
}
