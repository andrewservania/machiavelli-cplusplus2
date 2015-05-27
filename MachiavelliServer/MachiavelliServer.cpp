// MachiavelliServer.cpp : Defines the entry point for the console application.
// Andrew Servania
//Studentennummer: 2036129
//Sven van den Berg
//Studentennummer: 2056189
//Avans Hogeschool Den Bosch
//Datum: 09-01-2015
//Vak: C++2

#include "stdafx.h"
#include <thread>
#include <iostream>
#include <exception>
#include <memory>
#include <ctime>
#include "Server.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	/*Necessary for random card-stack shuffling*/
	std::srand(unsigned(std::time(0)));
	Server server = Server();
	server.runServer();
	
	//mGame = make_unique<Game>();
	//
	//// start command consumer thread
	//thread consumer{ consume_command };
	//consumer.detach(); // detaching is usually ugly, but in this case the right thing to do

	//// create a server socket
	//ServerSocket server(socketexample::tcp_port);
	//std::cout << "Status: SERVER STARTED\n";

	//bool gameStarted = false;
	//Socket* client = nullptr;

	//while (mGame->waitForClients()) {
	//	try {
	//		// wait for connection from client; will create new socket
	//		cerr << "Status: Server listening..." << '\n';

	//		while (((client = server.accept()) != nullptr)) {
	//			// communicate with client over new socket in separate thread
	//			thread handler{ handle_client, client };
	//			handler.detach(); // detaching is usually ugly, but in this case the right thing to do
	//			//std::cout << client->get_dotted_ip() << endl;
	//			//cerr << "Server listening again" << '\n';

	//			cerr << "Status: Server listening for client input..\n";
	//			string ip = client->get_dotted_ip();
	//			mGame->addPlayer(client, ip);
	//			if (!mGame->waitForClients()){
	//				break;
	//			}
	//		}
	//	}
	//	catch (const exception& ex) {
	//		cerr << ex.what() << ", resuming..." << '\n';
	//	}
	//}
	//while (!mGame->waitForClients()){
	//	try {
	//		if (!gameStarted){
	//			mGame->initGame();
	//			gameStarted = true;
	//		}
	//			
	//		// communicate with client over new socket in separate thread
	//		thread handler{ handle_client, nullptr};	
	//		handler.detach(); // detaching is usually ugly, but in this case the right thing to do
	//		mGame->run();
	//	}
	//	catch (const exception& ex) {
	//		cerr << ex.what() << ", resuming..." << '\n';
	//	}
	//		
	//}
	//return 0;
}

