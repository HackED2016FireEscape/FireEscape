// CommPort.cpp : Defines the entry point for the console application.
//

#include<windows.h>
#include<iostream>
#include<memory.h>
#include<string.h>
#include<queue>
#include "./CommPort.h"
#include "engine.h"

using std::queue;
using namespace std;

HANDLE commPort = INVALID_HANDLE_VALUE;
TCHAR* myComm = TEXT("COM6");
DCB dcb;
queue <char> myQueue;
bool openCOM = false;

CommPort::CommPort()
{
	
}

CommPort::~CommPort() {
	CloseHandle(commPort);
}

void CommPort::update()
{
	if (openCOM) {
		Engine& e = Engine::getInstance();

		DWORD nRead;
		int i = 0;
		bool flag = true;
		while (flag) {
			char buff[1];
			ReadFile(commPort, buff, 1, &nRead, NULL);
			std::cout << buff[0];
			if (buff[0] != '\0' && buff[0] != '\n' && buff[0] != '\r' && buff[0] != ' ') {
				e.actionMutex.lock();
				e.getActions().push(buff[0]);
				e.actionMutex.unlock();
			}
		}
	}



	//CloseHandle(commPort);
	//return 0;
}

queue<char> CommPort::getActions()
{
	return queue<char>();
}

bool CommPort::init()
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

		printf("No COM available.");
		openCOM = false;
		return false;

	}
	else {
		printf("Successfully connected.\n\n");
		openCOM = true;
	}

	dcb.DCBlength = sizeof(DCB);

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;

	if (!::SetCommState(commPort, &dcb)) {
		return false;
	}

	return true;
}
