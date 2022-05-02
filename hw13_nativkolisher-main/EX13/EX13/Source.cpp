#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include <iostream>
#include <exception>
#include "Server.h"
#include <thread>
#include <vector>;
#include <vector>;

int main()
{
	WSAInitializer wsaInit;
	Server myServer;
	std::thread connect(&Server::handle_messages, myServer, 8826);
	std::this_thread::sleep_for(std::chrono::seconds(600));			//GET TIME OUT AFTER 10 MIN
	std::cout << "TIMEOUT" << std::endl;
	connect.detach();
	system("PAUSE");
	return 0;
}
