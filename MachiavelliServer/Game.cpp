#include "stdafx.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip> 
#include "Server.h"

using namespace std;

int playerID = 1000;
int firstPlayerPositionCompensationValue = 0;


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
	dealCharacters();

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



	mPlayers.at(0)->getPlayerClient()->write("All players have connected.\n");
	mPlayers.at(1)->getPlayerClient()->write("All players have connected.\n");

	Sleep(2000);
	mPlayers.at(0)->getPlayerClient()->write("CLEARSCREEN\n");
	mPlayers.at(1)->getPlayerClient()->write("CLEARSCREEN\n");

	mPlayers.at(0)->getPlayerClient()->write("Please take a peek at the top card in the character deck.\n");
	mPlayers.at(0)->getPlayerClient()->write("[0] Take a peek at the top card\n");
	mPlayers.at(0)->getPlayerClient()->write("[1] Discard the top card. In order to continue...\n");

	mPlayers.at(1)->getPlayerClient()->write("Please wait while the king takes a peek at the top card in the character deck.\n"
		"The king will also pick 1 card to keep for himself...\n");

	// King gets to see upper characterCard, and gets to pick one characterCard
	//peekCharacter();
	//pickCharacter();
	////switch Active Player
	//// Other player gets to pick one, and gets to discard one characterCard
	//peekCharacter();
	//discardCharacter();
	////switch Active Player
	//// King gets to pick one, and gets to discard one characterCard
	//peekCharacter();
	//discardCharacter();
	////switch Active Player
	//// Other player gets to pick one, and gets to discard one characterCard
	//peekCharacter();
	//discardCharacter();
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
			setPlayerCharacterToKing(clientNumber);
		}
		else if (mPlayers.size() == 2){
			mPlayers[clientNumber + firstPlayerPositionCompensationValue]->sendMessage("You are Player " + std::to_string(clientNumber + 1));
			std::shared_ptr<CharacterCard> characterCard = std::make_shared<CharacterCard>();
			characterCard->mName = "-";
			characterCard->mID = 0;
			mPlayers[clientNumber]->setCurrentCharacter(characterCard);
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
	std::string fileName = "karakterkaartenNEW.csv";
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

void Game::sendUpdatedClientDashboard()
{
	std::string line1 = "Je bent nu de: " + mPlayers.at(0)->getCurrentCharacter()->getName() +"\n";
	std::string line2 = "Goud: " + std::to_string(mPlayers.at(0)->getCurrentAmountOfGold())+"\n";
	std::string emptyLine1 = "\n";
	std::string line3 = "Gebouwen:\n";
	std::string line4 = " 1. Geen\n";
	std::string line5 = " 2. Geen\n";
	std::string line6 = " 3. Geen\n";
	std::string emptyLine2 = "\n";
	std::string line7 = "Handkaarten:\n";
	std::string line8 = "{GeenKaartNaam},{GeenKleur},{GeenBeschrijving}\n";
	std::string emptyLine3 = "\n";
	std::string line9 = "Maak een keuze:\n";
	std::string line10 = "[0] Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)\n";
	std::string line11 = "[1] Neem 2 goedstukken\n";
	std::string line12 = "[2] Neem 2 bouwkaarten en leg er 1 af\n";
	std::string line13 = "[3] Maak gebruik van de karakter eigenschap van de Magier\n";
	std::string emptyLine4 = "\n";
	std::thread clientsHandler{ &Server::sendMessageToAllPlayers, 
		line1+line2+line3+emptyLine1+line4+line5+line6+emptyLine2+line7+line8+emptyLine3+line9+line10+line11+line12+line13+emptyLine4+"\n" };
	clientsHandler.detach();
}

void Game::setPlayerCharacterToKing(int playerNumber)
{
	std::shared_ptr<CharacterCard> kingCharacterCard = std::make_shared<CharacterCard>();
	kingCharacterCard->mName = "Koning";
	kingCharacterCard->mID = 999;
	mPlayers.at(playerNumber)->setCurrentCharacter(kingCharacterCard);
	
	
}

void Game::discardTopCharacterCard()
{
	if (topCardDiscarded == false)
	{
		mCharacterCardDiscards.add(mCharacterCards.at(0));
		mCharacterCards.removeAt(0);
		topCardDiscarded = true;
	}
}

void Game::showRemainingCharactersCardsInDeck(std::shared_ptr<Socket> currentPlayer)
{
	for (int i = 0; i < mCharacterCards.getCardStackSize(); i++)
	{
		currentPlayer->write("[" + std::to_string(i) + "] Card: " + mCharacterCards.at(i).getName() + " Card ID: " + std::to_string(mCharacterCards.at(i).getID()) + "\n");
	}
}

void Game::pickCharacterCardAndDiscard(int cardNumber, std::shared_ptr<Socket> client, int playerNumber)
{
	mPlayers.at(playerNumber)->addCharacterCard(mCharacterCards.at(cardNumber));
	mCharacterCards.removeAt(cardNumber);
	client->write("Card number " + std::to_string(cardNumber) + " selected.\n");
}
