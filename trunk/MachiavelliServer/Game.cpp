#include "stdafx.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip> 

Game::Game()
{
	if (readAndLoadBuildingCardsFromCSVFile() == true){
		printf("Status: Building Cards loaded successfully.\n");
	}
	else{
		printf("Status: Building Cards NOT loaded.\n");
	}

	if (readAndLoadCharacterCardsFromCSVFile()){
		printf("Status: Character Cards loaded successfully.\n");
	}
	else{
		printf("Status: Character Cards NOT loaded.\n");
	}
	
	/* Works as well, this is a template-method approach:
	
	BuildingCard buildingCard;
	CharacterCard characterCard;


	if (loadCSV(buildingCard) == true){
		printf("Status: Building Cards loaded.");
	}
	else{
		printf("Status: Building Cards NOT loaded.");
	}

	if (loadCSV(characterCard) == true){
		printf("Status: Character Cards loaded.");
	}
	else{
		printf("Status: Character Cards NOT loaded.");
	}

	*/

	//Initialize game stuff here of course :)



}

Game::~Game()
{
}


void Game::initServer()
{

}

void Game::waitForClients()
{

}

void Game::run()
{

}

void Game::initGame()
{

}

void Game::dealCharacters()
{

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
	}

	// Remove last element to eliminate unnecessary and non existing element
	mBuildingCards.removeAt(mBuildingCards.getCardStackSize() - 1);

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
	}

	return isLoadingSuccesful;
}

