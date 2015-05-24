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
	if (connectedPlayers.size() < 2){
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
	connectedPlayers.at(0)->addGold(2);
	connectedPlayers.at(1)->addGold(2);

	for (int i = 0; i < 4; i++){
		//AddHandCard
		int randomCardNumber = rand() % mBuildingCards.getCardStackSize();
		connectedPlayers.at(0)->addBuildingCard(mBuildingCards.at(randomCardNumber));
		mBuildingCards.removeAt(randomCardNumber);
	}

	for (int i = 0; i < 4; i++){
		//AddHandCard
		int randomCardNumber = rand() % mBuildingCards.getCardStackSize();
		connectedPlayers.at(1)->addBuildingCard(mBuildingCards.at(randomCardNumber));
		mBuildingCards.removeAt(randomCardNumber);
	}
	cout << "Game is initialized." << endl;
}

void Game::dealCharacters()
{
	currentGameState = KING_PEEKS_AT_TOP_CARD_AND_DISCARDS;


	connectedPlayers.at(0)->getPlayerClient()->write("All players have connected.\n");
	connectedPlayers.at(1)->getPlayerClient()->write("All players have connected.\n");

	Sleep(2000);
	connectedPlayers.at(0)->getPlayerClient()->write("CLEARSCREEN\n");
	connectedPlayers.at(1)->getPlayerClient()->write("CLEARSCREEN\n");

	connectedPlayers.at(0)->getPlayerClient()->write("Please take a peek at the top card in the character deck.\n");
	connectedPlayers.at(0)->getPlayerClient()->write("[0] Take a peek at the top card\n");
	connectedPlayers.at(0)->getPlayerClient()->write("[1] Discard the top card. In order to continue...\n");

	connectedPlayers.at(1)->getPlayerClient()->write("Please wait while the king takes a peek at the top card in the character deck.\n"
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

void Game::discardCharacterCard(int cardNumber)
{
	discardedCharacterCards.add(mCharacterCards.at(cardNumber));
	mCharacterCards.removeAt(cardNumber);
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

void Game::addPlayer(Socket* client, string ip)
{
	if (clientNumber < 2){

		auto player = make_shared<Player>(client, ip);

		playerID++;
		player->setPlayerID(playerID);

		player->setPlayerNumber(clientNumber);
		connectedPlayers.push_back(player);



		if (connectedPlayers.size() == 1){

			connectedPlayers[clientNumber]->sendMessage("You are Player " + std::to_string(clientNumber + 1) + ". The King!");
			setPlayerCharacterToKing(clientNumber);
		}
		else if (connectedPlayers.size() == 2){
			connectedPlayers[clientNumber + firstPlayerPositionCompensationValue]->sendMessage("You are Player " + std::to_string(clientNumber + 1));
			std::shared_ptr<CharacterCard> characterCard = std::make_shared<CharacterCard>();
			characterCard->mName = "-";
			characterCard->mID = 0;
			connectedPlayers[clientNumber]->setCurrentCharacter(characterCard);
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

	for (std::vector<shared_ptr<Player>>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
		/* std::cout << *it; ... */


		if (currentClientIP == (*it).get()->getPlayerClient()->get_dotted_ip()
			&& currentClientSocket == (*it).get()->getPlayerClient()->get()){
			//last disconnected player/client found!
			//Now delete him from the vector

			if ((*it).get()->getPlayerNumber() == 0){
				clientNumber = 0;
				firstPlayerPositionCompensationValue = 1;
			}
			else if ((*it).get()->getPlayerNumber() == 1){
				firstPlayerPositionCompensationValue = 0;
				clientNumber = 1;
			}
			connectedPlayers.erase(it);
			connectedPlayers.shrink_to_fit();
			if (connectedPlayers.size() == 0){
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
	shared_ptr<Player > spClient{ connectedPlayers.at(ID) };

	return spClient;
}

int Game::getAmountOfConnectedPlayers()
{
	return clientNumber;
}

void Game::sendUpdatedClientDashboard(int playerNumber)
{
	std::string line1 = "Je bent nu de: " + connectedPlayers.at(playerNumber)->getCurrentCharacter()->getName() + "\n";
	std::string line2 = "Goud: " + std::to_string(connectedPlayers.at(playerNumber)->getCurrentAmountOfGold()) + "\n";
	std::string emptyLine1 = "\n";

	std::vector<BuildingCard> playerBuildingCards = connectedPlayers.at(playerNumber)->getAllBuildingCards();

	std::string line3 = "Gebouwen:\n";
	std::string line4 = " 1. Geen\n";
	std::string line5 = " 2. Geen\n";
	std::string line6 = " 3. Geen\n";
	std::string emptyLine2 = "\n";
	std::string line7 = "Handkaarten:\n";
	std::string line8;

	for each (BuildingCard var in playerBuildingCards)
	{
		std::string buildingCard = var.getName() + " (" + var.getColor() +", " + std::to_string(var.getCost()) +" )" + var.getDescription() + "\n";
		line8 += buildingCard;
	} 

	std::string emptyLine3 = "\n";
	std::string line9 = "Maak een keuze:\n";
	std::string line10 = "[0] Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)\n";
	std::string line11 = "[1] Neem 2 goedstukken\n";
	std::string line12 = "[2] Neem 2 bouwkaarten en leg er 1 af\n";
	std::string line13 = "[3] Maak gebruik van de karakter eigenschap van de Magier\n";
	std::string emptyLine4 = "\n";
	std::thread clientsHandler{ &Server::sendMessageToPlayer,
		line1 + line2 + line3 + emptyLine1 + line4 + line5 + line6 + emptyLine2 + line7 + line8 + emptyLine3 + line9 + line10 + line11 + line12 + line13 + emptyLine4 + "\n",playerNumber };
	clientsHandler.detach();
}

void Game::setPlayerCharacterToKing(int playerNumber)
{
	std::shared_ptr<CharacterCard> kingCharacterCard = std::make_shared<CharacterCard>();
	kingCharacterCard->mName = "Koning";
	kingCharacterCard->mID = 999;
	connectedPlayers.at(playerNumber)->setCurrentCharacter(kingCharacterCard);


}

void Game::discardTopCharacterCard()
{
	if (topCardDiscarded == false)
	{
		discardedCharacterCards.add(mCharacterCards.at(0));
		mCharacterCards.removeAt(0);
		topCardDiscarded = true;
	}
}

void Game::showRemainingCharactersCardsInDeckToClient(Socket* currentPlayer)
{
	for (int i = 0; i < mCharacterCards.getCardStackSize(); i++)
	{
		currentPlayer->write("[" + std::to_string(i) + "] Card: " + mCharacterCards.at(i).getName() + " Card ID: " + std::to_string(mCharacterCards.at(i).getID()) + "\n");
	}
}

void Game::pickCharacterCard(int cardNumber, std::shared_ptr<Socket> client, int playerNumber)
{
	connectedPlayers.at(playerNumber)->addCharacterCard(mCharacterCards.at(cardNumber));
	mCharacterCards.removeAt(cardNumber);
	client->write("Card number " + std::to_string(cardNumber) + " selected.\n");
}

void Game::consumeCommand(std::string command, std::shared_ptr<Socket> currentClient)
{
	int commandNumber = std::atoi(command.c_str());	//Change the incoming message to an integer!

	if (currentGameState == KING_PEEKS_AT_TOP_CARD_AND_DISCARDS){
		std::string ipAddressOfCurrentClient = currentClient->get_dotted_ip();
		if (ipAddressOfCurrentClient == connectedPlayers.at(0)->getPlayerClient()->get_dotted_ip()){
			//TODO: Change this if-statement to switch-statement
			switch (commandNumber)
			{
				case 0:
				{
					CharacterCard topCard = peekCharacter();
					currentClient->write(serverName + "Card name: " + topCard.getName() + " Card ID: " + std::to_string(topCard.getID()) + "\n");
					break;

				}
				case 1:
				{
					discardTopCharacterCard();
					currentClient->write("Top card discarded\n");
					Sleep(1000);
					currentClient->write("CLEARSCREEN\n");
					currentClient->write("Now choose one of the remaining character cards and discard an other one:\n");
					showRemainingCharactersCardsInDeckToClient((currentClient).get());
					currentGameState = PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN;

					break;
				}
				default:
				{
					// All other commands are ignored and the current player is notified
					currentClient->write("Hey, you wrote: '");
					currentClient->write(command);
					currentClient->write("', but I'm not doing anything with it.\n");
					break;
				}

			}

		}
		else{

			currentClient->write("Please wait for the king to finish. Thank you!\n");




		}


	}



	else if (currentGameState == PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN){
		std::string ipAddressOfCurrentClient = currentClient->get_dotted_ip();
		if (ipAddressOfCurrentClient == connectedPlayers.at(0)->getPlayerClient()->get_dotted_ip()){

				if (commandNumber >= 0 && commandNumber < mCharacterCards.getCardStackSize()){

						if (playerOneHasChosenACharacterCard == false){
							pickCharacterCard(commandNumber, currentClient, 0);
							playerOneHasChosenACharacterCard = true;
							currentClient->write(serverName + "Now pick a card to discard.\n");
							showRemainingCharactersCardsInDeckToClient((currentClient).get());
						}
						else{
							if (playerOneHasDiscardedACharacterCard == false)
							{
								discardCharacterCard(commandNumber);

								if (mCharacterCards.getCardStackSize() == 0){

									thread messageHandler1{ &Server::sendMessageToAllPlayers, "CLEARSCREEN\n" };
									messageHandler1.detach();
									thread messageHandler2{ &Server::sendMessageToAllPlayers, "Let the first round begin!\n" };
									messageHandler2.detach();
									currentGameState = PLAYER_ONE_TURN;
									Sleep(1000);
									thread messageHandler3{ &Server::sendMessageToAllPlayers, "CLEARSCREEN\n" };
									messageHandler3.detach();
									thread messageHandler4{ &Server::sendMessageToAllPlayers, "Let the first round begin!\n" };
									messageHandler4.detach();
									initGame();
									sendUpdatedClientDashboard(0);
									sendUpdatedClientDashboard(1);
									// Start the first Machiavelli game turn! Finally!
									
								}
								else{
									connectedPlayers.at(1)->getPlayerClient()->write(serverName + "Please pick a character card and discard another one.\n");
									showRemainingCharactersCardsInDeckToClient(connectedPlayers.at(1)->getPlayerClient());
									playerOneHasDiscardedACharacterCard = true;
									currentGameState = PLAYER_TWO_CHARACTER_CARD_SELECTION_TURN;
									playerTwoHasChosenACharacterCard = false;
									playerTwoHasDiscardedACharacterCard = false;
									currentClient->write(serverName + "Please wait for the other player to pick and discard cards...\n");
								}

							}

						}
						

					
				}
				else{
					currentClient->write(serverName+"Hey, you wrote: '");
					currentClient->write(command);
					currentClient->write("', but I'm not doing anything with it.\n");
				}
		}
		else{
			currentClient->write(serverName + "Please wait for the other player to finish. Thank you!\n");
		}

	}


	else if (currentGameState == PLAYER_TWO_CHARACTER_CARD_SELECTION_TURN){
		std::string ipAddressOfCurrentClient = currentClient->get_dotted_ip();
		if (ipAddressOfCurrentClient == connectedPlayers.at(1)->getPlayerClient()->get_dotted_ip()){



				int commandNumber = std::atoi(command.c_str());

					if (commandNumber >= 0 && commandNumber < mCharacterCards.getCardStackSize()){


							if (playerTwoHasChosenACharacterCard == false){
								pickCharacterCard(commandNumber, currentClient, 0);
								playerTwoHasChosenACharacterCard = true;
								currentClient->write(serverName + "Now pick a card to discard.\n");
								showRemainingCharactersCardsInDeckToClient((currentClient).get());
							}
							else{
								if (playerTwoHasDiscardedACharacterCard == false)
								{
									discardCharacterCard(commandNumber);

									if (mCharacterCards.getCardStackSize() == 0){

									}
									else{
										currentGameState = PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN;
										connectedPlayers.at(0)->getPlayerClient()->write(serverName + "Please pick a character card and discard another one.\n");
										playerTwoHasDiscardedACharacterCard = true;
										showRemainingCharactersCardsInDeckToClient(connectedPlayers.at(0)->getPlayerClient());
										playerOneHasChosenACharacterCard = false;
										playerOneHasDiscardedACharacterCard = false;
										currentClient->write(serverName + "Please wait for the other player to pick and discard cards...\n");
									}

								}

							}

					
					}
					else{
						currentClient->write("Please pick a card to discard.\n");
					}
			

		}
		else{
			currentClient->write(serverName + "Please wait for the other player to finish. Thank you!\n");
		}

	}








	else if (currentGameState == PLAYER_ONE_TURN){
		
		//if (command == "0"){
		//	// Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)
		//}
		//else if (command == "1"){
		//	// Neem 2 goudstukken
		//}
		//else if (command == "2"){
		//	// Neem 2 bouwkaarten en leg er 1 af
		//}
		//else if (command == "3"){
		//	// Maak gebruik van de karaktereingenschap van de Magier
		//}
		//else{// All other commands are ignored and the current player is notified
		//	currentClient->write("Hey, you wrote: '");
		//	currentClient->write(command);
		//	currentClient->write("', but I'm not doing anything with it.\n");
		//}

	}

	else if (currentGameState == PLAYER_TWO_TURN)
	{
		//if (command == "0"){
		//	// Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)
		//}
		//else if (command == "1"){
		//	// Neem 2 goudstukken
		//}
		//else if (command == "2"){
		//	// Neem 2 bouwkaarten en leg er 1 af
		//}
		//else if (command == "3"){
		//	// Maak gebruik van de karaktereingenschap van de Magier
		//}
		//else{// All other commands are ignored and the current player is notified
		//	currentClient->write("Hey, you wrote: '");
		//	currentClient->write(command);
		//	currentClient->write("', but I'm not doing anything with it.\n");
		//}
	}



}
