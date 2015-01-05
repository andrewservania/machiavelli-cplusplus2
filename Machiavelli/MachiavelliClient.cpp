// Machiavelli.cpp : Defines the entry point for the console application.
//

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
	std::cout << "Welcome to...\n";
	std::ifstream Reader("MachiavelliText.txt");
	std::string Art = loadASCIIart(Reader);
	std::cout << Art << std::endl;
	Reader.close();
}


int _tmain(int argc, _TCHAR* argv[])
{


	std::string serverIP = "127.0.0.1";
	int serverPort = 1080;

	std::unique_ptr<ClientSocket> clientSocket(new ClientSocket(serverIP.c_str(),serverPort));
	std::cout << clientSocket->readline();
	int i = 0;
	std::string messageToServer = "";
	std::cout << "\n Client initialized..\n";
	showASCIIart();
	std::cout <<"by Andrew Servania & Sven van den Berg\n";


	while (true){
		i++;
		messageToServer = std::cin.get();
		clientSocket->write(std::to_string(i) + messageToServer+ "\r\n");
		std::cout << clientSocket->readline() + "\r\n";

	}


	return 0;
}

