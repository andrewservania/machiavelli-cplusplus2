#pragma once
#include "Player.h"
#include <vector>
#include "CardStack.h"
#include <typeinfo>
#include <memory>

using namespace std;

class Game
{
private:
	

	CardStack<BuildingCard> mBuildingCards;
	CardStack<CharacterCard> mCharacterCards;
	CardStack<BuildingCard> mBuildingCardDiscards;
	CardStack<CharacterCard> mCharacterCardDiscards;

	int clientNumber;

public:
	Game();
	~Game();

	std::vector<shared_ptr<Player > > mPlayers;

	void initServer();
	bool waitForClients();
	void run();
	void initGame();
	void dealCharacters();
	void discardCharacter();
	CharacterCard peekCharacter();
	void pickCharacter();
	void playRound();
	void countPlayerScores();
	void playCharacter();
	void addPlayer(Socket *client, string IPaddress);
	bool readAndLoadBuildingCardsFromCSVFile();
	bool readAndLoadCharacterCardsFromCSVFile();
	void removeLastDisconnectedPlayer(shared_ptr<Socket> client);
	shared_ptr<Player> getPlayer(int ID);
	int getAmountOfConnectedPlayers();


	template<class T>
	bool loadCSV(T card);

};

template<class T>
bool Game::loadCSV(T card)
{
	bool isLoadingSuccesful = false;
	std::string filePath = "..\\Resources\\";
	std::string fileName = "";
	std::string fullFilePath ="";
	std::ifstream file;
	char seperatorSymbol = ';';

	std::string classType = typeid(card).name();
	if (classType == "class BuildingCard"){

		fileName = "Bouwkaarten.csv";
		fullFilePath = filePath + fileName;
		std::ifstream file(fullFilePath);

		std::string costString = "";
		BuildingCard buildingCard;

		if (file.good() == true){ /*To check if file is good/exists */
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
	else if (classType == "class CharacterCard"){

		fileName = "karakterkaarten.csv";
		fullFilePath = filePath + fileName;
		std::ifstream file(fullFilePath);

		std::string stringID = "";
		CharacterCard characterCard;

		if (file.good() == true){ /*To check if file is good/exists */
			isLoadingSuccesful = true;

			while (file.good() == true){
				std::getline(file, stringID, seperatorSymbol)
					&& std::getline(file, characterCard.mName);

				characterCard.mID = atoi(stringID.c_str());
				mCharacterCards.add(characterCard);
			}
		}
		// Remove last element to eliminate unnecessary and non existing element
		mCharacterCards.removeAt(mCharacterCards.getCardStackSize() - 1);

		return isLoadingSuccesful;

	}
	else {
		isLoadingSuccesful = false;
		return isLoadingSuccesful;
	}

}
