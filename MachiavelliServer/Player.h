#pragma once
#include <string>
#include <vector>
#include <memory>
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
	int mPlayerID;
	std::vector<BuildingCard> Hand;
	std::vector<BuildingCard> Table;
	std::vector<CharacterCard> Characters;
	int playerNumber;
	std::shared_ptr<CharacterCard> currentCharacter;

public:
	Player(Socket *client, string IPaddress);
	~Player();

	void addGold(int goldToAdd);
	void removeGold(int goldToRemove);
	void addHandCard();
	void addTableCard();
	void addCharacter();
	void buyBuilding();
	void removeCharacters(); 
	void removeHandCard();
	void removeTableCard();
	void clearAll();
	int getTableSize();
	void sendMessage(string message);

	Socket* getSocket();
	void setPlayerID(int ID);
	Socket* getPlayerClient();
	int getPlayerNumber();
	void setPlayerNumber(int number);
	void setCurrentCharacter(std::shared_ptr<CharacterCard> character);
	std::shared_ptr<CharacterCard>  getCurrentCharacter();
	int getCurrentAmountOfGold();
};

