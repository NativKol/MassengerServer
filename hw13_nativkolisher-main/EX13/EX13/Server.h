#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include "Server.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <string>
#include "MessageSender.h"
//#include "fileHandle.h"
#include <fstream>

#define USERSTART 5
#define NAMESTART 2
#define ASCI_TO_INT 48

#define min(a,b) ((a) < (b) ? (a) : (b))

class Server : MessageSender
{
public:
	Server();
	~Server();
	void handle_messages(int port);
	void serve(int port);
	void clientHandler(SOCKET clientSocket);
	void acceptClient();
	std::string convertToString(char* a, int start, int end);

private:
	SOCKET _serverSocket;
};
