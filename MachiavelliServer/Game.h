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

	CardStack<BuildingCard> discardedBuildingCards;
	CardStack<CharacterCard> discardedCharacterCards;

	int clientNumber;
	bool topCardDiscarded = false;
	string serverName = "MACHIAVELLI-SERVER: ";

	bool playerOneHasChosenACharacterCard = false;
	bool playerOneHasDiscardedACharacterCard = false;

	bool playerTwoHasChosenACharacterCard = false;
	bool playerTwoHasDiscardedACharacterCard = false;

public:
	  enum GameStates {
		NOT_SET_YET,
		KING_PEEKS_AT_TOP_CARD_AND_DISCARDS,
		PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN,
		PLAYER_TWO_CHARACTER_CARD_SELECTION_TURN,
		WATIING_FOR_PLAYER_TO_FINISH,
		WAITING_FOR_PLAYERS_TO_CONNECT,
		INITIAL_CARD_DEALING,
		PLAYER_ONE_TURN, PLAYER_TWO_TURN,
		KING_GOES_THROUGH_ALL_CHARACTER_CARDS
	};
	static GameStates currentGameState;
	string playerOneIdentityNumber = "";
	string playerTwoIdentityNumber = "";
	int playerOnePortNumber = 0;
	int playerTwoPortNumber = 999;
	vector<shared_ptr<Player>> connectedPlayers;

	static vector<string> characterCardsInOrder;
	int announcedCharacterCardCounter = 0;
	//static Game instance;

	bool playerOneHasPickedTwoBuildingCards;
	bool playerTwoHasPickedTwoBuildingCards;
	Game();
	~Game();
	void initServer();
	bool waitForClients();
	void run();
	void initGame();
	void dealCharacters();
	void discardCharacterCard(int cardNumber);
	CharacterCard peekCharacter();
	void pickCharacter(); // Do I need this method?
	void playRound();
	void countPlayerScores();
	void playCharacter(); // Do I need this method?
	void addPlayer(shared_ptr<Socket> client);
	bool readAndLoadBuildingCardsFromCSVFile();
	bool readAndLoadCharacterCardsFromCSVFile();
	void removeLastDisconnectedPlayer(shared_ptr<Socket> client);
	shared_ptr<Player> getPlayer(int ID);
	int getAmountOfConnectedPlayers();
	void sendUpdatedClientDashboard(int playerNumber);
	void setPlayerCharacterToKing(int playerNumber);
	void discardTopCharacterCard();
	void showRemainingCharactersCardsInDeckToClient(shared_ptr<Socket> currentPlayer);
	void pickCharacterCard(int cardNumber, shared_ptr<Socket> client, int playerNumber);
	void givePlayer_TwoBuildingCards(int currentClientNumber, shared_ptr<Socket> currentClient);

	void consumeCommand(string command, shared_ptr<Socket> currentClient);

	string isPlayerTheAnnouncedCharacter(CharacterCard announcedCharacter);
	void static StartAnnouncingCharacterCards();
	void resetRound();

	template<class T>
	bool loadCSV(T card);
	static void broadCastEverySecond(string message);
	static void broadCastMessage(string message);
};

template<class T>
bool Game::loadCSV(T card)
{
	bool isLoadingSuccesful = false;
	string filePath = "..\\Resources\\";
	string fileName = "";
	string fullFilePath ="";
	ifstream file;
	char seperatorSymbol = ';';

	string classType = typeid(card).name();
	if (classType == "class BuildingCard"){

		fileName = "Bouwkaarten.csv";
		fullFilePath = filePath + fileName;
		ifstream file(fullFilePath);

		string costString = "";
		BuildingCard buildingCard;

		if (file.good() == true){ /*To check if file is good/exists */
			isLoadingSuccesful = true;

			while (file.good() == true){

				getline(file, buildingCard.mName, seperatorSymbol)
					&& getline(file, costString, seperatorSymbol)
					&& getline(file, buildingCard.mColor, seperatorSymbol)
					&& getline(file, buildingCard.mDescription);

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
		ifstream file(fullFilePath);

		string stringID = "";
		CharacterCard characterCard;

		if (file.good() == true){ /*To check if file is good/exists */
			isLoadingSuccesful = true;

			while (file.good() == true){
				getline(file, stringID, seperatorSymbol)
					&& getline(file, characterCard.mName);

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
