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



class Server
{
private:
	const int tcp_port{ 1080 };
	const std::string prompt ="> ";
	std::unique_ptr<Game> mGame;
	std::vector<Socket*> connectedClients;

	int command = -1;

public:
	Server();
	~Server();

	void runServer();
	void stopServer();
	void listenForClients();
	void handleClient(Socket* socket);
	void handleIncomingMessage(std::shared_ptr<Socket> client, std::string message);
	void consumeCommand();
	void sendMessageToClient(Socket* client, std::string message);
	void pingPlayers();
	void sendMessageToAllPlayers(std::string message);
};

