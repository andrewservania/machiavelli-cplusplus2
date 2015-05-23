#include "stdafx.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

Game::Game()
{
	clientNumber = 0;
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
	peekCharacter();
	pickCharacter();
	//switch Active Player
	// Other player gets to pick one, and gets to discard one characterCard
	peekCharacter();
	discardCharacter();
	//switch Active Player
	// King gets to pick one, and gets to discard one characterCard
	peekCharacter();
	discardCharacter();
	//switch Active Player
	// Other player gets to pick one, and gets to discard one characterCard
	peekCharacter();
	discardCharacter();
}

void Game::discardCharacter()
{

}

CharacterCard Game::peekCharacter()
{
	return mCharacterCards.at(0);
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

int playerID = 1000;
int firstPlayerPositionCompensationValue = 0;
void Game::addPlayer(Socket *client, string ip)
{
	if (clientNumber < 2){

		auto player = make_shared<Player>(client, ip);
			
			playerID++;
			player->setPlayerID(playerID);

			player->setPlayerNumber(clientNumber);
			mPlayers.push_back(player);



		if (mPlayers.size() == 1){

			mPlayers[clientNumber]->sendMessage("You are Player " + std::to_string(clientNumber + 1) + ". The King!");
		}
		else if (mPlayers.size() == 2){
			mPlayers[clientNumber + firstPlayerPositionCompensationValue]->sendMessage("You are Player " + std::to_string(clientNumber + 1));
		}
		clientNumber++;

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
	std::printf("Status: Building Cards loaded successfully.\n");
	}
	else{
		std::cerr << "Status: Building Cards NOT loaded.\n";
	}

	mBuildingCards.shuffle();


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
		std::printf("Status: Character Cards loaded successfully.\n");
	}
	else{
		std::cerr << "Status: Character Cards NOT loaded.\n";
	}

	mCharacterCards.shuffle();
	return isLoadingSuccesful;
}

/*To remove players that have disconnected at any given point*/
void Game::removeLastDisconnectedPlayer(shared_ptr<Socket> client)
{
	string currentClientIP = client->get_dotted_ip();
	SOCKET currentClientSocket = client->get();

	for (std::vector<shared_ptr<Player>>::iterator it = mPlayers.begin(); it != mPlayers.end(); ++it) {
		/* std::cout << *it; ... */
		

		if (currentClientIP == (*it).get()->getPlayerClient()->get_dotted_ip()
			&& currentClientSocket == (*it).get()->getPlayerClient()->get()){
			//last disconnected player/client found!
			//Now delete him from the vector

			if ((*it).get()->getPlayerNumber() == 0){
				clientNumber = 0;
				firstPlayerPositionCompensationValue = 1;
			}
			else if((*it).get()->getPlayerNumber() == 1){
				firstPlayerPositionCompensationValue = 0;
				clientNumber = 1;
			}
			mPlayers.erase(it);
			mPlayers.shrink_to_fit();
			if (mPlayers.size() == 0){
				clientNumber = 0;
				firstPlayerPositionCompensationValue = 0;
			}
			//mPlayers.shrink_to_fit();
			break;
		}
		

	}

}

shared_ptr<Player> Game::getPlayer(int ID)
{
	shared_ptr<Player > spClient{ mPlayers.at(ID) };
	
	return spClient;
}

int Game::getAmountOfConnectedPlayers()
{
	return clientNumber;
}
