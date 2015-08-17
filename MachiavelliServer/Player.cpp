#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::Player(std::shared_ptr<Socket> socket)
{
	client = socket;
	IPaddress = socket->get_dotted_ip();
	gold = 2;
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

void Player::addBuildingCardOnTable(BuildingCard boughtBuildingCard)
{
	buildingCardsBoughtAndOnTable.push_back(boughtBuildingCard);
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
	buildingCardsInHand.push_back(buildingCard);
}

void Player::addCharacterCard(CharacterCard characterCard)
{
	characterCardsInHand.push_back(characterCard);
}

std::shared_ptr<Socket> Player::getSocket()
{
	return client;
}

std::vector<CharacterCard> Player::getAllCharacterCards()
{
	return characterCardsInHand;
}

std::vector<BuildingCard> Player::getBoughtBuildingCards()
{
	return buildingCardsBoughtAndOnTable;
}

std::vector<BuildingCard> Player::getBuildingCardsInHand()
{
	return buildingCardsInHand;
}

std::vector<CharacterCard> Player::getCharacterCardsInHand()
{
	return characterCardsInHand;
}

bool Player::playerHasCollectedGold()
{
	return playerHasCollectedGoldInCurrentTurn;
}

void Player::setPlayerGoldCollectionStatus(bool playerhasCollectedGold)
{
	playerHasCollectedGoldInCurrentTurn = playerhasCollectedGold;
}


