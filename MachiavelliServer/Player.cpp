#include "stdafx.h"
#include "Player.h"



Player::Player(Socket *client, string IPaddress)
{
	this->client = client;
	this->IPaddress = IPaddress;
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

void Player::addCharacter()
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
	return Table.size();
}

void Player::sendMessage(string message)
{
	client->write(message + "\r\n");
}

void Player::setPlayerID(int ID)
{
	mPlayerID = ID;
}

Socket* Player::getPlayerClient()
{
	return client;
}


