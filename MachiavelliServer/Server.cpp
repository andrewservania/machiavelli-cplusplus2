#include "stdafx.h"
#include "Server.h"

#include <thread>
#include <chrono>


static Sync_queue<ClientCommand> queue;


static std::vector<std::shared_ptr<Socket>> connectedClients;
unique_ptr<Game> Server::mGame;

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
				std::shared_ptr<Socket> clientSmartPointer{client };

					mGame->addPlayer(clientSmartPointer); ///add new connected client to mPlayers in game
				
					printf("A player has connected\n");

				// communicate with client over new socket in separate thread
					std::thread handler{ &Server::handleClient, this, clientSmartPointer };
				handler.detach(); // detaching is usually ugly, but in this case the right thing to do

				cerr << "Server listening again" << '\n';


				//The game will start when this if-statement has become true!
				if (mGame->getAmountOfConnectedPlayers() == 2){
					notifyPlayersGameHasStarted();
					mGame->playerOneIdentityNumber = mGame->connectedPlayers.at(0)->getPlayerClient()->get_dotted_ip() + std::to_string(mGame->connectedPlayers.at(0)->getPlayerClient()->get());
					mGame->playerTwoIdentityNumber = mGame->connectedPlayers.at(1)->getPlayerClient()->get_dotted_ip() + std::to_string(mGame->connectedPlayers.at(1)->getPlayerClient()->get());
					mGame->currentGameState = Game::KING_PEEKS_AT_TOP_CARD_AND_DISCARDS;
					//break;
				}




			}
		}
		catch (const exception& ex) {
			cerr << ex.what() << ", resuming..." << '\n';
		}

	}

}

void Server::handleClient(std::shared_ptr<Socket> client)
{
	
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
				client->close();
				break;
			}

			if (cmd == "quit") {
				client->write("Bye!\n");
				break; // out of game loop, will end this thread and close connection
			}
			printIncomingMessage(client, cmd);

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
		std::shared_ptr<Socket> client{ command.get_client() };
		if (client) {
			try {
				// TODO handle command here
				if (mGame->getAmountOfConnectedPlayers() < 2){

					client->write("MACHIAVELLI-SERVER: Please wait for the other player to connect first!\nThank you!\n");

				}
				else{
					std::string message = command.get_cmd();



					mGame->consumeCommand(message, client);

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
		if (!mGame->connectedPlayers.empty()){
			//	throw std::logic_error("The method or operation is not implemented.");
			mGame->connectedPlayers.size();
			
			for (size_t i = 0; i < mGame->connectedPlayers.size(); i++){
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

void Server::printIncomingMessage(std::shared_ptr<Socket> client, std::string message)
{
	cerr << "client (" << client->get_dotted_ip() + ":" + std::to_string(client->get()) << ") said: " << message << '\n';
}

void Server::sendMessageToAllPlayers(std::string message)
{
	mGame->connectedPlayers.at(0)->getPlayerClient()->write(message);
	mGame->connectedPlayers.at(1)->getPlayerClient()->write(message);
}

void Server::notifyPlayersGameHasStarted()
{
	thread gameStartedMessagingHandler{ &Server::sendMessageToAllPlayers, "CLEARSCREEN\n" };
	gameStartedMessagingHandler.detach();
	mGame->run();
}

void Server::sendMessageToPlayer(std::string message, int playerNumber)
{
	mGame->connectedPlayers.at(playerNumber)->getPlayerClient()->write(message);
}



