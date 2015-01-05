#pragma once
#include <string>
#include <vector>
#include "BuildingCard.h"
#include "CharacterCard.h"
class Player
{
private:
	std::string IPaddress;
	int gold;
	std::vector<BuildingCard> Hand;
	std::vector<BuildingCard> Table;
	std::vector<CharacterCard> Characters;

public:
	Player();
	~Player();

	void addGold();
	void removeGold();
	void addHandCard();
	void addTableCard();
	void addCharacter();
	void buyBuilding();
	void removeCharacters(); 
	void removeHandCard();
	void removeTableCard();
	void clearAll();
	void getTableSize();
};

