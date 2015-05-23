#include "stdafx.h"
#include "Server.h"

#include <thread>
#include <chrono>


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
			cerr << "Server listening" << '\n';
			Socket* client = nullptr;

			while ((client = server.accept()) != nullptr) {

					mGame->addPlayer(client, client->get_dotted_ip()); ///add new connected client to mPlayers in game
					printf("A player has connected\n");

				// communicate with client over new socket in separate thread
				thread handler{ &Server::handleClient, this, client };
				handler.detach(); // detaching is usually ugly, but in this case the right thing to do
				cerr << "Server listening again" << '\n';
				if (mGame->getAmountOfConnectedPlayers() == 2){
					thread gameStartedMessagingHandler{ &Server::sendMessageToAllPlayers, this, "GAMESTARTED\n" };
					gameStartedMessagingHandler.detach();
					//break;
				}

			}
		}
		catch (const exception& ex) {
			cerr << ex.what() << ", resuming..." << '\n';
		}

	}

}

void Server::handleClient(Socket* socket)
{
	std::shared_ptr<Socket> client{ socket };
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
			catch (...)/// To properly deal with clients who suddenly disconnect
			{
				cerr << "Client " << client->get_dotted_ip() << " with socket: " << client->get() << " has disconnected\n";
				mGame->removeLastDisconnectedPlayer(client);
				break;
			}

			if (cmd == "quit") {
				client->write("Bye!\n");
				break; // out of game loop, will end this thread and close connection
			}
			handleIncomingMessage(client, cmd);

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
std::string serverName = "MACHIAVELLI-SERVER: ";
void Server::consumeCommand()
{
	while (true) {
		ClientCommand command;
		queue.get(command); // will block here unless there still are command objects in the queue
		std::shared_ptr<Socket> client{ command.get_client() };
		if (client) {
			try {
				// TODO handle command here
				if (mGame->getAmountOfConnectedPlayers() < 2){

					client->write(serverName+"Please wait for the other player to connect first!\nThank you!\n");

				}
				else{
					std::string message = command.get_cmd();

					if (message == "0"){
						CharacterCard topCard = mGame->peekCharacter();

						client->write(serverName + "Card name: " + topCard.getName() + " Card ID: " + std::to_string(topCard.getID()) + "\n");
					}
					else if (message == "1"){

					}
					else if (message == "2"){

					}
					else if (message == "3"){

					}
					else if (message == "4"){

					}
					else if (message == "5"){

					}
					else if (message == "6"){

					}
					else{
						client->write("Hey, you wrote: '");
						client->write(command.get_cmd());
						client->write("', but I'm not doing anything with it.\n");
					}


				}




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

void Server::pingPlayers()
{
	while (true){
		if (!mGame->mPlayers.empty()){
			//	throw std::logic_error("The method or operation is not implemented.");
			mGame->mPlayers.size();
			
			for (size_t i = 0; i < mGame->mPlayers.size(); i++){
				std::this_thread::sleep_for((std::chrono::seconds(1)));
				mGame->getPlayer(i).get()->sendMessage("Ping Message: Hi! Welcome to Machiavelli!");
			}
		}
		else{
			std::this_thread::sleep_for((std::chrono::seconds(1)));
			printf("\nWaiting for clients to connect..");
		}

	}

	
}

void Server::handleIncomingMessage(std::shared_ptr<Socket> client, std::string message)
{
	// read first line of request
	// string cmd = client->readline();
	cerr << "client (" << client->get() << ") said: " << message << '\n';




}

void Server::sendMessageToAllPlayers(std::string message)
{
	
		mGame->mPlayers.at(0)->getPlayerClient()->write(message);
		mGame->mPlayers.at(1)->getPlayerClient()->write(message);
	
}

