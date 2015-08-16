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
	const string prompt ="> ";
	int command = -1;

public:
	static unique_ptr<Game> mGame;
	Server();
	~Server();

	void runServer();
	void stopServer();
	void listenForClients();
	void handleClient(shared_ptr<Socket> socket);
	void printIncomingMessage(shared_ptr<Socket> client, string message);
	void consumeCommand();
	void pingPlayers();
	static void sendMessageToAllPlayers(string message);
	static void sendMessageToPlayer(string message, int playerNumber);
	void notifyPlayersGameHasStarted();
	
};

