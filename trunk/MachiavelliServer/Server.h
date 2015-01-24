#pragma once
#include <string>
#include <thread>
#include <iostream>
#include <exception>
#include <memory>
#include "Game.h"
#include "Player.h"
#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"

using namespace std;

class Server
{
private:
	const int tcp_port{ 1080 };
	const string prompt{ "> " };
	unique_ptr<Game> mGame;
	std::vector<Socket*> connectedClients;

public:
	Server();
	~Server();

	void runServer();
	void stopServer();
	void listenForClients();
	void handleClient(Socket* socket);
	void consumeCommand();
	void sendMessageToClient(Socket* client, string message);
	void pingPlayers();
};

