#pragma once
#include <string>
#include <vector>
#include <memory>
#include "BuildingCard.h"
#include "CharacterCard.h"
#include "Socket.h"
#include <memory>


class Player
{
private:
	std::string IPaddress;
	Socket* client;
	int gold = 0;
	int mPlayerID;
	//std::vector<BuildingCard> Hand;
	//std::vector<BuildingCard> Table;


	std::vector<CharacterCard> CharacterCards;
	std::vector<BuildingCard> BuildingsCards;
	int playerNumber;
	std::shared_ptr<CharacterCard> currentCharacter;

public:
	Player(Socket* socket, std::string ip);
	~Player();

	void addGold(int goldToAdd);
	void removeGold(int goldToRemove);

	void addHandCard();
	void addTableCard();

	void addCharacterCard(CharacterCard characterCard);
	void addBuildingCard(BuildingCard buildingCard);

	void buyBuilding();
	void removeCharacters(); 

	void removeHandCard();
	void removeTableCard();

	void clearAll();
	int getTableSize();
	void sendMessage(std::string message);

	Socket* getSocket();
	void setPlayerID(int ID);
	Socket* getPlayerClient();
	int getPlayerNumber();
	void setPlayerNumber(int number);
	void setCurrentCharacter(std::shared_ptr<CharacterCard> character);
	std::shared_ptr<CharacterCard>  getCurrentCharacter();
	int getCurrentAmountOfGold();

	std::vector<CharacterCard> getAllCharacterCards();
	std::vector<BuildingCard> getAllBuildingCards();

};

