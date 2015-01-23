#include "stdafx.h"
#include "Server.h"

static Sync_queue<ClientCommand> queue;

Server::Server()
{
	mGame = make_unique<Game>();

	// create a server socket
	
}


Server::~Server()
{

}

void Server::runServer()
{
	listenForClients();
}

void Server::stopServer()
{

}

void Server::listenForClients()
{
	// start command consumer thread
	thread consumer{ &Server::consumeCommand, this };
	consumer.detach(); // detaching is usually ugly, but in this case the right thing to do

	// create a server socket
	ServerSocket server(Server::tcp_port);

	while (true) {
		try {
			// wait for connection from client; will create new socket
			cerr << "server listening" << '\n';
			Socket* client = nullptr;

			while ((client = server.accept()) != nullptr) {
				// communicate with client over new socket in separate thread
				thread handler{ &Server::handleClient, this, client };
				handler.detach(); // detaching is usually ugly, but in this case the right thing to do
				cerr << "server listening again" << '\n';
			}
		}
		catch (const exception& ex) {
			cerr << ex.what() << ", resuming..." << '\n';
		}
	}

}

void Server::handleClient(Socket* socket)
{
	shared_ptr<Socket> client{ socket };
	client->write("Welcome to Server 1.0! To quit, type 'quit'.\n");
	client->write(Server::prompt);

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

			// read first line of request
			// string cmd = client->readline();
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

void Server::consumeCommand()
{
	while (true) {
		ClientCommand command;
		queue.get(command); // will block here unless there still are command objects in the queue
		shared_ptr<Socket> client{ command.get_client() };
		if (client) {
			try {
				// TODO handle command here
				client->write("Hey, you wrote: '");
				client->write(command.get_cmd());
				client->write("', but I'm not doing anything with it.\n");
			}
			catch (const exception& ex) {
				client->write("Sorry, ");
				client->write(ex.what());
				client->write("\n");
			}
			catch (...) {
				client->write("Sorry, something went wrong during handling of your request.\n");
			}
			client->write(Server::prompt);
		}
		else {
			cerr << "trying to handle command for client who has disappeared...\n";
		}
	}
}