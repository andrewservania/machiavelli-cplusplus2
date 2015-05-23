#pragma once
#include <string>
#include <iostream>
#include <memory>
#include "ClientCommand.h"
#include "Socket.h"

class MachiavelliClient
{
private:
	std::string serverIPaddress = "127.0.0.1";
	int serverPortnumber = 1080;
	std::shared_ptr<ClientSocket> clientSocket;
public:
	MachiavelliClient();
	~MachiavelliClient();
	std::string loadASCIIart(std::ifstream& file);
	void showASCIIart();
	void attempToConnectToServer(std::string serverIP, int serverPort);
	void listenToMessagesFromServer();
	void standByForConsoleMessageToServer();
	void clearScreen();
	

};

