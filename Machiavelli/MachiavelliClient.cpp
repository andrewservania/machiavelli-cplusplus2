#include "stdafx.h"
#include "MachiavelliClient.h"
#include <thread>
#include <fstream>
#include <windows.h>

MachiavelliClient::MachiavelliClient()
{
	attempToConnectToServer(serverIPaddress, serverPortnumber);

	while (clientSocket != nullptr)
	{
		try{
			std::thread clientHandler{  &MachiavelliClient::listenToMessagesFromServer,this }; //Listen to all messages coming from the server.
			clientHandler.detach();
			standByForConsoleMessageToServer();

			
		}
		catch (...){
			break;
		}
	}

}


MachiavelliClient::~MachiavelliClient()
{
}


std::string MachiavelliClient::loadASCIIart(std::ifstream& file)
{
	std::string lines = "";
	if (file){
		while (file.good()){
			std::string temporaryLine;
			std::getline(file, temporaryLine);
			temporaryLine += "\n";
			lines += temporaryLine;
		}
	}

	return lines;
}

void MachiavelliClient::showASCIIart()
{
	std::ifstream Reader("MachiavelliText.txt");
	std::string machiavelliArt = loadASCIIart(Reader);
	std::cout << machiavelliArt << "\n";
	Reader.close();
}

void MachiavelliClient::attempToConnectToServer(std::string serverIP, int serverPort)
{
	while (clientSocket == nullptr)
	{
		try
		{
			std::printf("Attempting to connect to server...\n");
			clientSocket = std::make_shared<ClientSocket>(serverIP.c_str(), serverPort);

		}
		catch (...)
		{
			printf("Cannot connect to server.\n");
		}

	}
	if (clientSocket != nullptr){
		showASCIIart();
		std::cout << "by Andrew Servania\n";
	}
}

void MachiavelliClient::listenToMessagesFromServer()
{
	while (true)
	{
		try
		{
			std::string messageFromServer = clientSocket->readline() + "\r\n";

			if (messageFromServer.find("CLEARSCREEN") != std::string::npos ){ // if the word 'CLEARSCREEN' is found. Execute a clean screen.
				clearScreen();
				
			}
			else
			{
				std::cout << messageFromServer;
			}
		}
		catch (...){
			printf("Whoa! Lost connection to server");
			clientSocket = nullptr;
			attempToConnectToServer(serverIPaddress, serverPortnumber);
			break;
		}
	}
}

void MachiavelliClient::standByForConsoleMessageToServer()
{
	while (true)
	{
		std::string messageToServer = "";
		std::cin >> messageToServer;
		clientSocket->write(messageToServer + "\r\n");
	}
}

//WARNING! WINDOWS COMPATIBLE ONLY AS THIS USES <WINDOWS.H>!
void MachiavelliClient::clearScreen()
{
	#pragma region WINDOWS-COMPATIBLE-ONLY SCREEN CLEARING CODE
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
	#pragma endregion

	
}

