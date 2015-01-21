#pragma once
#include <string>
#include <vector>
#include "BuildingCard.h"
#include "CharacterCard.h"
#include "Socket.h"

using namespace std;

class Player
{
private:
	std::string IPaddress;
	Socket *client;
	int gold = 0;

public:
	Player(Socket *client, string IPaddress);
	~Player();

	void addGold(int goldToAdd);
	void removeGold(int goldToRemove);
	int getTableSize();

	void sendMessage(string message);
};

