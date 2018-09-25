/*
Client.cpp 
Client entry point

@author Navid Assadian
*/
#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "Data.h"

#define BUFFERSIZE 512
int _tmain(int argc, TCHAR *argv[])
{
	char data[BUFFERSIZE] = ("Hello Server");
	HANDLE hPipe;
	LPTSTR lpvMessage = LPTSTR(data);
	TCHAR  chBuf[BUFFERSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	if (argc > 1)
		lpvMessage = argv[1];

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// Send a message to the pipe server. 
	while(1)
	{
		std::string command;
		std::string message;
		Data tmp_data;
		/* It can be sending a data to server (R), 
		   Changing a especific data (C), 
		   sending just a simple string message (S) or
		   Exit the program */
		while (1)
		{
			printf("Commands: \n 1- Register a data (R) \n 2- Change a registered data (C)\n 3- Send a message to the server (S)\n 4- Exit (E)\n");

			std::cin >> command;
			//If the input command was wrong
			if (!(command == "R" || command == "C" || command == "S" || command == "E" || command == "r" || command == "c" || command == "s" || command == "e"))
			{
				printf("Please enter a right command: R , C, S or E\n");
				continue;
			}
			if (command == "C" || command == "c")
			{
				printf("Under Construction!!\n Try again later");
				continue;
			}

			break;
		}
		//Exit the program
		if (command == "E" || command == "e")
			break;
		else
		{
			//Send a simple string
			if (command == "S" || command == "s")
			{
				printf("Type your message in a single line:\n");
				getchar();
				getline(std::cin, message);
				lpvMessage = LPTSTR(message.c_str());
				cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
				fSuccess = WriteFile(
					hPipe,                  // pipe handle 
					lpvMessage,             // message 
					cbToWrite,              // message length 
					&cbWritten,             // bytes written 
					NULL);                  // not overlapped 
				cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
				printf(("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

			}
			//Send an object to the server
			else if (command == "R" || command == "r")
			{
				printf("You want to Register a new data. Perfect!!!\n You just need to enter an Integer, a real number, and a string\n");
				lpvMessage = TEXT("R");
				cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
				fSuccess = WriteFile(
					hPipe,                  // pipe handle 
					lpvMessage,             // message 
					cbToWrite,              // message length 
					&cbWritten,             // bytes written 
					NULL);                  // not overlapped 

				// Reading the Data info and creating a data object
				int tmp_int;
				double tmp_double;
				std::string tmp_string;
				printf("Int Value: ");
				std::cin >> tmp_int;
				printf("Float Value: ");
				std::cin >> tmp_double;
				printf("String Value: ");
				getchar();
				getline(std::cin, tmp_string);
				tmp_data.set_int(tmp_int);
				tmp_data.set_double(tmp_double);
				tmp_data.set_string(tmp_string.c_str());
				char* tmp_msg= new char[BUFFERSIZE];
				tmp_data.Ser(tmp_msg);					//Serializing the data object
				Data test;
				test.Unser(LPSTR(tmp_msg));				//Just to test if the unserialization works
				lpvMessage = LPTSTR(tmp_msg);
				cbToWrite = tmp_data.size() * sizeof(TCHAR);
				fSuccess = WriteFile(
					hPipe,                  // pipe handle 
					lpvMessage,             // message 
					cbToWrite,              // message length 
					&cbWritten,             // bytes written 
					NULL);                  // not overlapped 
			}

		}


		fSuccess = WriteFile(
			hPipe,                  // pipe handle 
			lpvMessage,             // message 
			cbToWrite,              // message length 
			&cbWritten,             // bytes written 
			NULL);                  // not overlapped 

		if (!fSuccess)
		{
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}

		printf("\nMessage sent to server, receiving reply as follows:\n");

		do
		{
			// Read from the pipe. 

			fSuccess = ReadFile(
				hPipe,    // pipe handle 
				chBuf,    // buffer to receive reply 
				BUFFERSIZE * sizeof(TCHAR),  // size of buffer 
				&cbRead,  // number of bytes read 
				NULL);    // not overlapped 

			if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
				break;

			_tprintf(TEXT("\"%s\"\n"), chBuf);
		} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

		if (!fSuccess)
		{
			_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}
	}
	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();

	CloseHandle(hPipe);

	return 0;
}
