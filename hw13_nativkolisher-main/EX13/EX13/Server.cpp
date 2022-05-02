#include "Server.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <algorithm>    
#include <vector>
#include <map>
#include "fileHandle.h"
#include <fstream>

//the queue of the messages
std::queue <std::string> msg;


Server::Server()
{

	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(_serverSocket);
	}
	catch (...) {}
}

void Server::handle_messages(int port)
{
	std::string s = "";
	std::thread th(&Server::serve, this, 8826);
	while (true)
	{
		if (!msg.empty())
		{
			//get from queue
			s = msg.front();
			msg.pop();
			//get the socket
			std::string soc = s.substr(0, 3);
			SOCKET tmp = (stoi(soc));
			s = s.substr(3);
			//std::cout << s << std::endl;
			//send message to client
			send(tmp, s.c_str(), s.size(), 0);
		}
	}

	th.detach();
}

void Server::serve(int port)
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		acceptClient();
	}
	
}


void Server::acceptClient()
{

	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	std::thread t(&Server::clientHandler, this, client_socket);
	t.detach();
}

/*
convert to string by index
*/
std::string Server::convertToString(char* a, int start, int end)
{
	int i;
	std::string s = "";
	for (i = start; i < end; i++) {
		s = s + a[i];
	}
	return s;
}


void Server::clientHandler(SOCKET clientSocket)
{
	std::cout << "NEW CLIENT\n";

	//initialize strings
	std::string name = "";			//the name of the client
	std::string update = "";		//the update message to client
	std::string all_users = "";			//all clients with & example: nativ&mom&brother
	std::string length_ofnames = "";	//length of all names with 0:  5 -> 00005
	std::string len_ofuser = "";		//length of user with 0:  5 -> 05
	std::string len_ofread = "";		//lenght of messages with 0:  5 -> 00005
	std::string file_name = "";			
	std::string getter_name = "";		//the reciver username 
	std::string message_len = "";
	std::string message = "";
	std::string tmp = "";
	std::string write_message = "";
	std::string input_text = "";
	//for alphabeticlly sort using sort method of vector
	std::vector<std::string> names_sort;
	//initialize length
	int len = 0, msg_len = 0;
	//for register 
	bool start = true;					

	char m[50];
	while (true)
	{
		recv(clientSocket, m, 49, 0);
		len = (m[3] - ASCI_TO_INT) * 10 + (m[4] - ASCI_TO_INT);		//ASCI to len of name
		//std::cout << "receaved: " << m << std::endl;

		if (m[2] == '0' && start)		//REGISTER
		{
			std::string client_name = convertToString(m, USERSTART, USERSTART + len);	//cut the name
			std::cout << "Client name is: " << client_name << std::endl;

			this->SignIn(client_name);
			name = client_name;
			start = false;
		}

		else if (m[2] == '4')		//UPDATE
		{
			getter_name = convertToString(m, USERSTART, USERSTART + len);	//cut the name
			msg_len = std::stoi(convertToString(m, USERSTART + len, USERSTART + len + USERSTART));		
			tmp = convertToString(m, USERSTART + len + USERSTART, USERSTART + len + USERSTART + msg_len);	//cut the message
			if (msg_len > 0)
			{
				message = tmp;
				std::cout << "NEW MESSAGE!" << std::endl;
				std::cout << message << std::endl;
			}

			//create file name
			names_sort.clear();
			names_sort.push_back(getter_name);
			names_sort.push_back(name);
			std::sort(names_sort.begin(), names_sort.end());		//sort alphabetically
			file_name = names_sort.front() + "&" + names_sort.back();

			if (msg_len > 0)
			{
				std::cout << "FILE NAME: " << file_name << std::endl;
				//write the message to file
				write_message = "&MAGSH_MESSAGE&&Author&" + name + "&DATA&" + message;
				writeMessagesToUsersFile(file_name, write_message);
			}
		}

		all_users = getAllUsers();				//get users
		input_text = readAdminFile(file_name);	//get the messages

		//ADD LEADING ZEROS TO LENGTH 
		length_ofnames = std::string(USERSTART - min(USERSTART, std::to_string(all_users.length()).length()), '0') + std::to_string(all_users.length());
		len_ofuser = std::string(NAMESTART - min(NAMESTART, std::to_string(getter_name.length()).length()), '0') + std::to_string(getter_name.length());
		len_ofread = std::string(USERSTART - min(USERSTART, std::to_string(input_text.length()).length()), '0') + std::to_string(input_text.length());

		//create message to client (update) + send socket
		if (input_text == "")    //check if there messages
		{
			message = "";
			message_len = "00000";        //no bytes to read
		}
		else      //already has comunication & file
		{
			message = input_text;
			message_len = len_ofread;        
		}
		std::string s = std::to_string(clientSocket) + "101" + message_len + message
			+ len_ofuser + getter_name +
			length_ofnames + all_users;

		msg.push(s);
	}
}
