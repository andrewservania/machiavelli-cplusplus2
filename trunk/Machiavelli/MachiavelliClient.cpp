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
#include <thread>
#include "Socket.h"
#include <string>
#include "ClientCommand.h"
#include <chrono>
#include <memory>

using namespace std;

///Necessary Global Variables:
 unique_ptr<ClientSocket> clientSocket;
 string serverIP = "127.0.0.1";
 int serverPort = 1080;

 ///Necessary to read a ASCII art .txt file into memory
 string loadASCIIart( ifstream& File){
	 string lines = "";

	if (File){

		while (File.good()){

			 string temporaryLine;
			 getline(File, temporaryLine);
			temporaryLine += "\n";
			lines += temporaryLine;
		}
		
	}
	return lines;
}

 ///To show introduce the user the Machiavelli game when a successful connection has been established
void showASCIIart(){

	 cout << "\nWelcome to...\n";
	 ifstream Reader("MachiavelliText.txt");
	 string Art = loadASCIIart(Reader);
	 cout << Art <<  endl;
	 Reader.close();
}

///To continuously attempt to connect to a Server
void attempToConnectToServer(string serverIP, int serverPort){
	while (clientSocket == nullptr) ///Try to connect to server while no connection has been made yet.
	{
		try{
			printf("Attempting to connect to server...\n");
			clientSocket = make_unique<ClientSocket>(serverIP.c_str(), serverPort);
		}
		catch (...){
			printf("Cannot connect to server.\n");
		}
	}
	if (clientSocket != nullptr){ ///Show ASCII art when connection has been established
		showASCIIart();
		cout << "by Andrew Servania & Sven van den Berg\n";
	}

}


///To make sure all messages coming from the server get received immediately
void listenToMessagesFromServer(){
	while (true){
		try{

			 cout << clientSocket->readline() + "\r\n";
		}
		catch (...){

			printf("Whoa! Lost connection to server\n");
			clientSocket = nullptr;
			attempToConnectToServer(serverIP,serverPort);
			break;
		}
		
	}
	
}

/// To send all messages from the client console to the server
void standByForConsoleMessageToServer(){
	while (true){
		 string messageToServer = "";
		 cin >> messageToServer;
		 clientSocket->write(messageToServer+"\r\n");
	}
}


///Client application start
int _tmain(int argc, _TCHAR* argv[])
{
	 attempToConnectToServer(serverIP,serverPort);

	 while (clientSocket != nullptr){ 
			try{
	    		thread messageReceivalHandler{ listenToMessagesFromServer }; ///To listen to all messages coming from the server. 
				messageReceivalHandler.detach();
				standByForConsoleMessageToServer();
				
				// Game-loop starts here...
			}
			catch (...){
				break;
			}
	 }

	return 0;
}

