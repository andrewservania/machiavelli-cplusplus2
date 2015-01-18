// Machiavelli.cpp : Defines the entry point for the console application.
// Andrew Servania
//Studentennummer: 2036129
//Sven van den Berg
//Studentennummer: 2056189
//Avans Hogeschool Den Bosch
//Datum: 09-01-2015
//Vak: C++2

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "Socket.h"
#include <string>
#include "ClientCommand.h"


std::string loadASCIIart(std::ifstream& File){
	std::string lines = "";

	if (File){

		while (File.good()){
			std::string temporaryLine;
			std::getline(File, temporaryLine);
			temporaryLine += "\n";
			lines += temporaryLine;
		}
		
	}
	return lines;
}

void showASCIIart(){
	std::cout << "\nWelcome to...\n";
	std::ifstream Reader("MachiavelliText.txt");
	std::string Art = loadASCIIart(Reader);
	std::cout << Art << std::endl;
	Reader.close();
}


int _tmain(int argc, _TCHAR* argv[])
{


	std::string serverIP = "127.0.0.1";
	std::string messageToServer = "";
	int serverPort = 1080;
	int messageID = 0;
	int maxMessageLength = 25;
	std::unique_ptr<ClientSocket> clientSocket(new ClientSocket(serverIP.c_str(),serverPort));
	std::cout << clientSocket->readline();

	showASCIIart();

	std::cout <<"by Andrew Servania & Sven van den Berg\n";


	while (true){
		messageID++;
		
		std::cin >> messageToServer;

		// std::to_string(messageID) + messageToServer + "\r\n";

		clientSocket->write(messageToServer + "\r\n");
		std::cout << clientSocket->readline() + "\r\n";

	}


	return 0;
}

