#include "stdafx.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

Game::Game()
{

	initServer();
}

Game::~Game(){

}


void Game::initServer()
{
	readAndLoadBuildingCardsFromCSVFile();
	readAndLoadCharacterCardsFromCSVFile();
}



bool Game::waitForClients()
{
	if (mPlayers.size() < 2){
		return true;
	}
	else {
		return false;
	}
}

void Game::run()
{

	//dealCharacters();

}

void Game::initGame()
{
	mPlayers[0]->addGold(2);
	mPlayers[1]->addGold(2);

	for (int i = 0; i < 4; i++){
		//AddHandCard
	}
	cout << "Game is initialized." << endl;
}

void Game::dealCharacters()
{
	// King gets to see upper characterCard, and gets to pick one characterCard
	showCharacter();
	pickCharacter();
	//switch Active Player
	// Other player gets to pick one, and gets to discard one characterCard
	showCharacter();
	discardCharacter();
	//switch Active Player
	// King gets to pick one, and gets to discard one characterCard
	showCharacter();
	discardCharacter();
	//switch Active Player
	// Other player gets to pick one, and gets to discard one characterCard
	showCharacter();
	discardCharacter();
}

void Game::discardCharacter()
{

}

void Game::showCharacter()
{

}

void Game::pickCharacter()
{

}

void Game::playRound()
{

}

void Game::countPlayerScores()
{

}

void Game::playCharacter()
{

}

void Game::addPlayer(Socket *client, string ip)
{
	auto player = make_shared<Player>(client, ip);
	mPlayers.push_back(player);
	if (mPlayers.size() == 1){
		mPlayers[0]->sendMessage("You are Player 1");
	}
	else if (mPlayers.size() == 2){
		mPlayers[1]->sendMessage("You are Player 2");
	}
	else {

	}
}

bool Game::readAndLoadBuildingCardsFromCSVFile(){

	bool isLoadingSuccesful = false;

	std::string filePath = "..\\Resources\\";
	std::string fileName = "Bouwkaarten.csv"; //karakterkaarten.csv
	std::string fullFilePath = filePath + fileName;
	std::ifstream file(fullFilePath);
	char seperatorSymbol = ';';
	std::string costString = "";
	
	BuildingCard buildingCard;
	

	if (file.good() == true){
		isLoadingSuccesful = true;

		while (file.good() == true){

				   std::getline(file, buildingCard.mName, seperatorSymbol)
				&& std::getline(file, costString, seperatorSymbol)
				&& std::getline(file, buildingCard.mColor, seperatorSymbol)
				&& std::getline(file, buildingCard.mDescription);

			buildingCard.mCost = atoi(costString.c_str());

			mBuildingCards.add(buildingCard);
		}
	// Remove last element to eliminate unnecessary and non existing element
	mBuildingCards.removeAt(mBuildingCards.getCardStackSize() - 1);
	printf("Status: Building Cards loaded successfully.\n");
	}
	else{
		std::cerr << "Status: Building Cards NOT loaded.\n";
	}



	return isLoadingSuccesful;

}

bool Game::readAndLoadCharacterCardsFromCSVFile(){

	bool isLoadingSuccesful = false;

	std::string filePath = "..\\Resources\\";
	std::string fileName = "karakterkaarten.csv";
	std::string fullFilePath = filePath + fileName;


	std::ifstream file(fullFilePath);
	char seperatorSymbol = ';';
	std::string stringID = "";

	CharacterCard characterCard;


	 if (file.good() == true){
		isLoadingSuccesful = true;

		while (file.good() == true){
			       std::getline(file, stringID, seperatorSymbol)
				&& std::getline(file, characterCard.mName);

			characterCard.mID = atoi(stringID.c_str());
			mCharacterCards.add(characterCard);
		}
		// Remove last element to eliminate unnecessary and non existing element
		mCharacterCards.removeAt(mCharacterCards.getCardStackSize() - 1);
		printf("Status: Character Cards loaded successfully.\n");
	}
	else{
		std::cerr << "Status: Character Cards NOT loaded.\n";
	}

	return isLoadingSuccesful;
}

