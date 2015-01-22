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
#include "Game.h"
#include "Player.h"

using namespace std;

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"

unique_ptr<Game> mGame;

namespace socketexample {
	const int tcp_port{ 1080 };
	const std::string prompt{ "> " };
}

static Sync_queue<ClientCommand> queue;

void consume_command() // runs in its own thread
{
	while (true) {
		ClientCommand command;
		queue.get(command); // will block here unless there still are command objects in the queue
		shared_ptr<Socket> client{ command.get_client() };
		if (client) {
			try {
				// TODO handle command here
				 
			}
			catch (const exception& ex) {
				client->write("Sorry, ");
				client->write(ex.what());
				client->write("\n");
			}
			catch (...) {
				client->write("Sorry, something went wrong during handling of your request.\n");
			}
			client->write(socketexample::prompt);
		}
		else {
			cerr << "trying to handle command for client who has disappeared...\n";
		}
	}
}

void handle_client(Socket* socket) // this function runs in a separate thread
{
	shared_ptr<Socket> client{ socket };
	
	client->write("CONNECTED TO SERVER 1.0! To quit, type 'quit'.\n");
	client->write(socketexample::prompt);

	while (true) { // game loop
		try {
			// read first line of request
			string cmd = "";
			try
			{
				cmd = client->readline();
			}
			catch (...)
			{
				cerr << "Client " << client->get_dotted_ip() << " with socket: " << client->get() << " has disconnected\n";
				mGame->removeLastDisconnectedPlayer(client);

					break;
			}

		//	string cmd = client->readline();
			cerr << "client (" << client->get() << ") said: " << cmd << '\n';

			if (cmd == "quit") {
				client->write("Bye!\n");
				break; // out of game loop, will end this thread and close connection
			}

			ClientCommand command{ cmd, client };
			queue.put(command);

		}
		catch (const exception& ex) {
			client->write("ERROR: ");
			client->write(ex.what());
			client->write("\n");
		}
		catch (...) {
			client->write("ERROR: something went wrong during handling of your request. Sorry!\n");
		}
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	mGame = make_unique<Game>();
	
	// start command consumer thread
	thread consumer{ consume_command };
	consumer.detach(); // detaching is usually ugly, but in this case the right thing to do

	// create a server socket
	ServerSocket server(socketexample::tcp_port);
	std::cout << "Status: SERVER STARTED\n";

	bool gameStarted = false;
	Socket* client = nullptr;

	while (mGame->waitForClients()) {
		try {
			// wait for connection from client; will create new socket
			cerr << "Status: Server listening..." << '\n';
			Socket* client = nullptr;

			while (((client = server.accept()) != nullptr)) {
				// communicate with client over new socket in separate thread
				thread handler{ handle_client, client };
				handler.detach(); // detaching is usually ugly, but in this case the right thing to do
				//std::cout << client->get_dotted_ip() << endl;
				//cerr << "Server listening again" << '\n';

				cerr << "Status: Server listening for client input..\n";
				string ip = client->get_dotted_ip();
				mGame->addPlayer(client, ip);
				if (!mGame->waitForClients()){
					break;
				}



			}
		}
		catch (const exception& ex) {
			cerr << ex.what() << ", resuming..." << '\n';
		}
	}
	while (!mGame->waitForClients()){
		try {
			if (!gameStarted){
				mGame->initGame();
				gameStarted = true;
			}
				
			// communicate with client over new socket in separate thread
			thread handler{ handle_client, client};	
			handler.detach(); // detaching is usually ugly, but in this case the right thing to do
			mGame->run();
		}
		catch (const exception& ex) {
			cerr << ex.what() << ", resuming..." << '\n';
		}
			
	}
	return 0;
}

