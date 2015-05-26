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
	int command = -1;

public:
	Server();
	~Server();

	void runServer();
	void stopServer();
	void listenForClients();
	void handleClient(std::shared_ptr<Socket> socket);
	void printIncomingMessage(std::shared_ptr<Socket> client, std::string message);
	void consumeCommand();
	void pingPlayers();
	static void sendMessageToAllPlayers(std::string message);
	static void sendMessageToPlayer(std::string message, int playerNumber);
	void notifyPlayersGameHasStarted();
};

