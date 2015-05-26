#pragma once
#include <string>
#include <vector>
#include <memory>
#include "BuildingCard.h"
#include "CharacterCard.h"
#include "Socket.h"
#include <memory>
#include "CardStack.h"

class Player
{
private:
	std::string IPaddress;
	std::shared_ptr<Socket> client;
	int gold = 0;
	int mPlayerID;
	//

int playerNumber;
	std::vector<CharacterCard> characterCardsInHand;
	std::vector<BuildingCard> buildingCardsInHand;

	
	std::shared_ptr<CharacterCard> currentCharacter;
	std::vector<BuildingCard> buildingCardsBoughtAndOnTable;

public:
	Player();
	Player(std::shared_ptr<Socket> socket);
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

	std::shared_ptr<Socket> getSocket();
	void setPlayerID(int ID);
	std::shared_ptr<Socket> getPlayerClient();
	int getPlayerNumber();
	void setPlayerNumber(int number);
	void setCurrentCharacter(std::shared_ptr<CharacterCard> character);
	std::shared_ptr<CharacterCard>  getCurrentCharacter();
	int getCurrentAmountOfGold();

	std::vector<CharacterCard> getAllCharacterCards();
	std::vector<BuildingCard> getAllBuildingCards();

};

