#include "stdafx.h"
#include "Player.h"



Player::Player(std::shared_ptr<Socket> socket, std::string ip)
{
	client = socket;
	IPaddress = ip;
	gold = 0;
}

Player::~Player()
{
	mPlayerID = 0;
}

void Player::addGold(int goldToAdd)
{
	gold = gold + goldToAdd;
}

void Player::removeGold(int goldToRemove)
{
	gold = gold - goldToRemove;
}

void Player::addHandCard()
{

}

void Player::addTableCard()
{

}

void Player::buyBuilding()
{

}

void Player::removeCharacters()
{

}

void Player::removeHandCard()
{

}

void Player::removeTableCard()
{

}

void Player::clearAll()
{

}

int Player::getTableSize()
{
	//return Table.size();
	return -1;
}

void Player::sendMessage(std::string message)
{
	client->write(message + "\r\n");
}

void Player::setPlayerID(int ID)
{
	mPlayerID = ID;
}

std::shared_ptr<Socket> Player::getPlayerClient()
{
	return client;
}

void Player::setPlayerNumber(int number)
{
	playerNumber = number;
}

int Player::getPlayerNumber()
{
	return playerNumber;
}

void Player::setCurrentCharacter(std::shared_ptr<CharacterCard> character)
{
	currentCharacter = character;
}

std::shared_ptr<CharacterCard> Player::getCurrentCharacter()
{
	return currentCharacter;
}

int Player::getCurrentAmountOfGold()
{
	return gold;
}

void Player::addBuildingCard(BuildingCard buildingCard)
{
	BuildingsCards.push_back(buildingCard);
}

void Player::addCharacterCard(CharacterCard characterCard)
{
	CharacterCards.push_back(characterCard);
}

std::shared_ptr<Socket> Player::getSocket()
{
	return client;
}

std::vector<CharacterCard> Player::getAllCharacterCards()
{
	return CharacterCards;
}

std::vector<BuildingCard> Player::getAllBuildingCards()
{
	return BuildingsCards;
}


