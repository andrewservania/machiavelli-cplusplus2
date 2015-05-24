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
	void handleClient(Socket* socket);
	void handleIncomingMessage(std::shared_ptr<Socket> client, std::string message);
	void consumeCommand();
	void sendMessageToClient(Socket* client, std::string message);
	void pingPlayers();
	static void sendMessageToAllPlayers(std::string message);
	void notifyPlayersGameHasStarted();
};

