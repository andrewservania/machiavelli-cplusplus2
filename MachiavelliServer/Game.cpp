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

Game::GameStates Game::currentGameState = NOT_SET_YET;
vector<string> Game::characterCardsInOrder;

//Game Game::instance;

Game::Game()
{
	clientNumber = 0;
	playerOneHasPickedTwoBuildingCards = false;
	initServer();
}

Game::~Game(){

}

void Game::initServer()
{
	readAndLoadBuildingCardsFromCSVFile();
	readAndLoadCharacterCardsFromCSVFile();

	characterCardsInOrder.push_back("Moordenaar");
	characterCardsInOrder.push_back("Dief");
	characterCardsInOrder.push_back("Magiër");
	characterCardsInOrder.push_back("Prediker");
	characterCardsInOrder.push_back("Koopman");
	characterCardsInOrder.push_back("Bouwmeester");
	characterCardsInOrder.push_back("Condottiere");

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

void Game::addPlayer(shared_ptr<Socket> client)
{
	if (clientNumber < 2){

		auto player = make_shared<Player>(client);

		playerID++;
		player->setPlayerID(playerID);
		player->setPlayerNumber(clientNumber);
		connectedPlayers.push_back(player);

		if (connectedPlayers.size() == 1){

			connectedPlayers[clientNumber]->sendMessage("You are Player " + to_string(clientNumber + 1) + ". The King!");
			setPlayerCharacterToKing(clientNumber);
		}
		else if (connectedPlayers.size() == 2){
			connectedPlayers[clientNumber + firstPlayerPositionCompensationValue]->sendMessage("You are Player " + to_string(clientNumber + 1));
			shared_ptr<CharacterCard> characterCard = make_shared<CharacterCard>();
			characterCard->mName = "-";
			characterCard->mID = 0;
			connectedPlayers[clientNumber]->setCurrentCharacter(characterCard);
		}
		clientNumber++;

	}
}

bool Game::readAndLoadBuildingCardsFromCSVFile(){

	bool isLoadingSuccesful = false;

	string filePath = "..\\Resources\\";
	string fileName = "Bouwkaarten.csv"; //karakterkaarten.csv
	string fullFilePath = filePath + fileName;
	ifstream file(fullFilePath);
	char seperatorSymbol = ';';
	string costString = "";

	BuildingCard buildingCard;


	if (file.good() == true){
		isLoadingSuccesful = true;

		while (file.good() == true){

			getline(file, buildingCard.mName, seperatorSymbol)
				&& getline(file, costString, seperatorSymbol)
				&& getline(file, buildingCard.mColor, seperatorSymbol)
				&& getline(file, buildingCard.mDescription);

			buildingCard.mCost = atoi(costString.c_str());

			mBuildingCards.add(buildingCard);
		}
		// Remove last element to eliminate unnecessary and non existing element
		mBuildingCards.removeAt(mBuildingCards.getCardStackSize() - 1);
		printf("Status: Building Cards loaded successfully.\n");
	}
	else{
		cerr << "Status: Building Cards NOT loaded.\n";
	}

	mBuildingCards.shuffle();


	return isLoadingSuccesful;

}

bool Game::readAndLoadCharacterCardsFromCSVFile(){

	bool isLoadingSuccesful = false;

	string filePath = "..\\Resources\\";
	string fileName = "karakterkaartenNEW.csv";
	string fullFilePath = filePath + fileName;


	ifstream file(fullFilePath);
	char seperatorSymbol = ';';
	string stringID = "";

	CharacterCard characterCard;


	if (file.good() == true){
		isLoadingSuccesful = true;

		while (file.good() == true){
			getline(file, stringID, seperatorSymbol)
				&& getline(file, characterCard.mName);

			characterCard.mID = atoi(stringID.c_str());
			mCharacterCards.add(characterCard);
		}
		// Remove last element to eliminate unnecessary and non existing element
		mCharacterCards.removeAt(mCharacterCards.getCardStackSize() - 1);
		printf("Status: Character Cards loaded successfully.\n");
	}
	else{
		cerr << "Status: Character Cards NOT loaded.\n";
	}

	mCharacterCards.shuffle();
	return isLoadingSuccesful;
}

void Game::removeLastDisconnectedPlayer(shared_ptr<Socket> client)
{
	string currentClientIP = client->get_dotted_ip();
	SOCKET currentClientSocket = client->get();

	for (vector<shared_ptr<Player>>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
		/* cout << *it; ... */


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
	string line1 = "Je bent nu de: " + connectedPlayers.at(playerNumber)->getCurrentCharacter()->getName() + "\n";
	string line2 = "Goud: " + to_string(connectedPlayers.at(playerNumber)->getCurrentAmountOfGold()) + "\n";
	string emptyLine1 = "\n";

	vector<BuildingCard> playerBuildingCards = connectedPlayers.at(playerNumber)->getBoughtBuildingCards();

	string line3 = "Gebouwen:\n";
	string line4 = "";


	// retrieve all building cards bought by the player and print them.
	if (playerBuildingCards.size() < 1)
	{
		line4 = "1. Geen\n2. Geen\n\3. Geen\n";
	}
	else
	{
		for each (BuildingCard var in playerBuildingCards)
		{
			string boughtbuildingCards = " -" + var.getName() + " (" + var.getColor() + ", " + to_string(var.getCost()) + ") " + var.getDescription() + "\n";
			line4 += boughtbuildingCards;
		}
	}
	string emptyLine2 = "\n";
	string line7 = "Handkaarten:\n";
	string line8;

	// retrieve all building cards from players hand and print them out
	for each (BuildingCard var in playerBuildingCards)
	{
		string buildingCard = " -"+var.getName() + " (" + var.getColor() +", " + to_string(var.getCost()) +") " + var.getDescription() + "\n";
		line8 += buildingCard;
	} 

	// retrieve all character cards from player's hand and print them out
	vector<CharacterCard> playerCharacterCards = connectedPlayers.at(playerNumber)->getCharacterCardsInHand();
	for each (CharacterCard var in playerCharacterCards)
	{
		string characterCardsInHand = " -" + var.getName() + +" (" + to_string(var.getID()) + ") \n";
		line8 += characterCardsInHand;
	}
	string emptyLine3 = "\n";
	string line9 = "Maak een keuze:\n";
	string line10 = "[0] Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)\n";
	string line11 = "[1] Neem 2 goedstukken\n";
	string line12 = "[2] Neem 2 bouwkaarten en leg er 1 af\n";
	string line13 = "[3] Maak gebruik van de karakter eigenschap van de Magier\n";
	string emptyLine4 = "\n";
	string line14 = "[4] Bekijk je kaarten\n";
	string line15 = "[5] Koop een gebouw\n";
	string line16 = "[9] End your turn (If applicable)";
	string emptyLine5 = "\n";
	thread clientsHandler{ &Server::sendMessageToPlayer,
		line1 + line2 + line3 + emptyLine1 + line4 + emptyLine2 + line7 + line8 + emptyLine3 + 
		line9 + line10 + line11 + line12 + line13 + emptyLine4 + 
		line14 + line15 + emptyLine5 + line16 +  "\n",playerNumber };
	clientsHandler.detach();

}

void Game::setPlayerCharacterToKing(int playerNumber)
{
	shared_ptr<CharacterCard> kingCharacterCard = make_shared<CharacterCard>();
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

void Game::showRemainingCharactersCardsInDeckToClient(shared_ptr<Socket> currentPlayer)
{
	for (int i = 0; i < mCharacterCards.getCardStackSize(); i++)
	{
		currentPlayer->write("[" + to_string(i) + "] Card: " + mCharacterCards.at(i).getName() + " Card ID: " + to_string(mCharacterCards.at(i).getID()) + "\n");
	}
}

void Game::pickCharacterCard(int cardNumber, shared_ptr<Socket> client, int playerNumber)
{
	connectedPlayers.at(playerNumber)->addCharacterCard(mCharacterCards.at(cardNumber));
	mCharacterCards.removeAt(cardNumber);
	client->write("Card number " + to_string(cardNumber) + " selected.\n");
}

// method that will be called when a player wants two building cards.
void Game::givePlayer_TwoBuildingCards(int currentClientNumber, shared_ptr<Socket> currentClient)
{
	switch (currentClientNumber)
	{
	case 0: 
		if (!playerOneHasPickedTwoBuildingCards)
		{
			//1. Give the player building cards from the top
			connectedPlayers.at(currentClientNumber)->addBuildingCard(mBuildingCards.at(0));
			connectedPlayers.at(currentClientNumber)->addBuildingCard(mBuildingCards.at(1));
			string message = "You now have the following building cards: \n + " + mBuildingCards.at(0).mName + "\n + " + mBuildingCards.at(1).mName;

			//2. Now remove the building cards from the building card stack
			mBuildingCards.removeAt(0);
			mBuildingCards.removeAt(1);

			currentClient->write(message + "\n"); // print the new building cards to the player
			sendUpdatedClientDashboard(currentClientNumber);
			playerOneHasPickedTwoBuildingCards = true;
			
		}
		break;
	case 1:

		if (!playerTwoHasPickedTwoBuildingCards)
		{
			//1. Give the player building cards from the top
			connectedPlayers.at(currentClientNumber)->addBuildingCard(mBuildingCards.at(0));
			connectedPlayers.at(currentClientNumber)->addBuildingCard(mBuildingCards.at(1));
			string message = "You now have the following building cards: \n + " + mBuildingCards.at(0).mName + "\n + " + mBuildingCards.at(1).mName;

			//2. Now remove the building cards from the building card stack
			mBuildingCards.removeAt(0);
			mBuildingCards.removeAt(1);

			currentClient->write(message + "\n"); // print the new building cards to the player
			sendUpdatedClientDashboard(currentClientNumber);
			playerTwoHasPickedTwoBuildingCards = true;

		}

		break;
	}


}


void Game::consumeCommand(string command, shared_ptr<Socket> currentClient)
{
	int commandNumber = atoi(command.c_str());	//Change the incoming message to an integer!
	string identityNumberOfCurrentClient = currentClient->get_dotted_ip() + to_string(currentClient->get());

#pragma  region HANDLE STATE: KING_PEEKS_AT_TOP_CARD_AND_DISCARDS

	if (currentGameState == KING_PEEKS_AT_TOP_CARD_AND_DISCARDS){
		
		if (identityNumberOfCurrentClient == playerOneIdentityNumber){
			switch (commandNumber)
			{
				case 0:
				{
					CharacterCard topCard = peekCharacter();
					currentClient->write(serverName + "Card name: " + topCard.getName() + " Card ID: " + to_string(topCard.getID()) + "\n");
					break;

				}
				case 1:
				{
					discardTopCharacterCard();
					currentClient->write("Top card discarded\n");
					Sleep(1000);
					currentClient->write("CLEARSCREEN\n");
					currentClient->write("Now choose one of the remaining character cards and discard an other one:\n");
					showRemainingCharactersCardsInDeckToClient(currentClient);
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

			currentClient->write(serverName+"Please wait for the king to finish. Thank you!\n");
		}


	}
#pragma endregion



#pragma region HANDLE STATE: PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN
	else if (currentGameState == PLAYER_ONE_CHARACTER_CARD_SELECTION_TURN){
		if (identityNumberOfCurrentClient == playerOneIdentityNumber){

				if (commandNumber >= 0 && commandNumber < mCharacterCards.getCardStackSize()){

						if (playerOneHasChosenACharacterCard == false){
							pickCharacterCard(commandNumber, currentClient, 0);
							playerOneHasChosenACharacterCard = true;
							currentClient->write(serverName + "Now pick a card to discard.\n");
							showRemainingCharactersCardsInDeckToClient(currentClient);
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

									thread broadcastHandler{ &Game::broadCastEverySecond, " King will starting going through character cards in " };
									broadcastHandler.detach();
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
			currentClient->write(serverName + "Please wait for the king to finish. Thank you!\n");;
		}

	}
#pragma endregion

#pragma region HANDLE STATE: PLAYER_TWO_CHARACTER_CARD_SELECTION_TURN
	else if (currentGameState == PLAYER_TWO_CHARACTER_CARD_SELECTION_TURN){
		if (identityNumberOfCurrentClient == playerTwoIdentityNumber){



				int commandNumber = atoi(command.c_str());

					if (commandNumber >= 0 && commandNumber < mCharacterCards.getCardStackSize()){


							if (playerTwoHasChosenACharacterCard == false){
								pickCharacterCard(commandNumber, currentClient, 1);
								playerTwoHasChosenACharacterCard = true;
								currentClient->write(serverName + "Now pick a card to discard.\n");
								showRemainingCharactersCardsInDeckToClient(currentClient);
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
			currentClient->write(serverName + "Please wait for other player to finish. Thank you!\n");
		}

	}
#pragma endregion



#pragma region HANDLE STATE: PLAYER_ONE_TURN

	else if (currentGameState == PLAYER_ONE_TURN){
		if (identityNumberOfCurrentClient == playerOneIdentityNumber) // To distinguish between connected client number one and connected client number two
		{
			switch (commandNumber)
			{
				case 0: // Command 0: Bekijk het goud en de  gebouwen van de tegenstander (en maak denn de keuze)
				{
						vector<BuildingCard> buildingsBoughtByOpponent = connectedPlayers.at(0)->getBoughtBuildingCards();
						string opponentsBuildings = "\n";
						if (buildingsBoughtByOpponent.size() == 0)
						{
							opponentsBuildings = "0 gebouwen";
						}
						else{
							for each (BuildingCard buildingCard in buildingsBoughtByOpponent)
							{
								opponentsBuildings += ("-"+buildingCard.getName() + " (" + buildingCard.getColor()+","+to_string(buildingCard.getCost())+ " )\n" + buildingCard.getDescription()+"\n");
							}
							
						}
						string opponentDetails = "Aantal gebouwen van tegenstander: " +opponentsBuildings + "\nAantal goud van tegenstander: " + to_string(connectedPlayers.at(1)->getCurrentAmountOfGold());
						currentClient->write(opponentDetails+"\n");		
						break;				
				}
				case 1: // Command 1: Neem 2 goed stukken
				{
					if (connectedPlayers.at(0)->playerHasCollectedGold() == false)
					{
						connectedPlayers.at(0)->addGold(2);
						connectedPlayers.at(0)->setPlayerGoldCollectionStatus(true);
					}
						// Neem 2 goudstukken
						currentClient->write("Aantal goud dat ik heb:" + to_string(connectedPlayers.at(0)->getCurrentAmountOfGold()) + "\n");
						break;		
				}
	
				case 2: // Command 2: Neem 2 bouwkaarten en leg er 1 af
				{		
					currentClient->write("CLEARSCREEN\n");
					givePlayer_TwoBuildingCards(0, currentClient);

					
						break;
				}
		
				case 3:// Command 3: Maak gebruik van de karaktereingenschap van de Magier
				{
						
						currentClient->write("Not implemented yet. \n");
						break;		
				}
		
				case 4: // Command 4: The player can use this command to take a look at his/her card
				{
					vector <BuildingCard> buildingCardsInHand = connectedPlayers.at(0)->getBuildingCardsInHand();
					vector<CharacterCard> characterCardsInHand = connectedPlayers.at(0)->getCharacterCardsInHand();

					string characterCardsDetails = "\n";
					for each (CharacterCard characterCard in characterCardsInHand)
					{
						characterCardsDetails += ("-" + characterCard.getName() + ", ID: " + to_string(characterCard.getID())+"\n");
					}
					string buildingCardsDetails = "\n";

					for each (BuildingCard buildingCard in buildingCardsInHand)
					{
						buildingCardsDetails += ("-" + buildingCard.getName() + " (" + buildingCard.getColor() + "," + to_string(buildingCard.getCost()) + " )\n" + buildingCard.getDescription() + "\n");
					}
						// Bekijk je handkaarten
						currentClient->write("Character Cards:\n"+characterCardsDetails + "\nBuilding Cards:" +buildingCardsDetails+"\n");
						break;
				}

				case 5: // Command 5: The player can buy a building if he/she has enough gold coins
				{
					currentClient->write("CLEARSCREEN\n");
					currentClient->write("Please select the building card you would like to buy\n");
					vector <BuildingCard> buildingCardsInHand = connectedPlayers.at(0)->getBuildingCardsInHand();
					string buildingCardsToBePrinted = "";
					int buildingCardCounter = 0;
					for each(BuildingCard buildingCard in buildingCardsInHand)
					{
						buildingCardsToBePrinted += ("[" + to_string(buildingCardCounter) + "]" + buildingCard.getName() + " (" + buildingCard.getColor() + "," + to_string(buildingCard.getCost()) + " )\n" + buildingCard.getDescription() + "\n");
						buildingCardCounter++;
					}
					
					currentClient->write(buildingCardsToBePrinted + "\n");
					currentGameState = PLAYER_ONE_BUYING_BUILDING_CARD;
					break;
				}

				case 9: // Command 9: End turn
				{
					resetRound();
					currentGameState = PLAYER_TWO_TURN;
					break;
				}
				default:
				{
					currentClient->write("Hey, you wrote: '");
					currentClient->write(command);
					currentClient->write("', but I'm not doing anything with it.\n");
					break;
				}

			}
		}
		else{
			currentClient->write(serverName + "Please wait for your turn. Thank you!\n");
		}
	}
#pragma endregion



		else if (currentGameState == PLAYER_ONE_BUYING_BUILDING_CARD)
		{

				vector <BuildingCard> buildingCardsInHand = connectedPlayers.at(0)->getBuildingCardsInHand();
				if (identityNumberOfCurrentClient == playerOneIdentityNumber){
					if (commandNumber < buildingCardsInHand.size()) // if the player has selected a card that is within the range of the building cards
					{
						if (connectedPlayers.at(0)->getCurrentAmountOfGold() >= buildingCardsInHand.at(commandNumber).getCost())
						{
							// then player can buy this card!
							// Add it to the building Cards on player's table
							// decrease the amount of gold coins spent from player
							connectedPlayers.at(0)->addBuildingCardOnTable(buildingCardsInHand.at(commandNumber));
							connectedPlayers.at(0)->removeGold(buildingCardsInHand.at(commandNumber).getCost());
							currentClient->write("You have bought the following card:\n - " + buildingCardsInHand.at(commandNumber).getName() + "\n");
					
							buildingCardsInHand.erase(buildingCardsInHand.begin(), buildingCardsInHand.end() + commandNumber); // erase the building that has been bought from hand
							sendUpdatedClientDashboard(0);
							currentGameState = PLAYER_ONE_TURN;
						} else
						{
							currentClient->write("HAH! Sorry, you do not have enough gold coins to buy this building.");
						}

					} else
					{
						currentClient->write("Hey, you wrote: '");
						currentClient->write(command);
						currentClient->write("', but I'm not doing anything with it.\n");
					}
				}
				else
				{
					currentClient->write(serverName + "Please wait for your turn. Thank you!\n");
				}
			
			
		}

	




#pragma region DEPRECATED: HANDLE STATE: KING_GOES_THROUGH_ALL_CHARACTER_CARDS
	else if (currentGameState == KING_GOES_THROUGH_ALL_CHARACTER_CARDS)
	{
		if (identityNumberOfCurrentClient == playerTwoIdentityNumber){
			switch (commandNumber)
			{
				case 0:
					// Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 1:
					// Neem 2 goudstukken
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 2:
					// Neem 2 bouwkaarten en leg er 1 af
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 3:
					// Maak gebruik van de karaktereingenschap van de Magier
					currentClient->write("Not implemented yet. :/\n");
					break;
				default:
					currentClient->write("Hey, you wrote: '");
					currentClient->write(command);
					currentClient->write("', but I'm not doing anything with it.\n");
					break;
			}
		}
		else
		{
			currentClient->write(serverName + "Please wait for your turn. Thank you!\n");
		}
	}
#pragma endregion	

#pragma region HANDLE STATE: <FILL IN> NEW STATE
	else if (currentGameState == PLAYER_TWO_TURN)
	{
		if (identityNumberOfCurrentClient == playerTwoIdentityNumber){
			switch (commandNumber)
			{
				case 0:
					// Bekijk het goed en de gebouwen van de tegenstander (en maak dan de keuze)
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 1:
					// Neem 2 goudstukken
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 2:
					// Neem 2 bouwkaarten en leg er 1 af
					currentClient->write("Not implemented yet. :/\n");
					break;
				case 3:
					// Maak gebruik van de karaktereingenschap van de Magier
					currentClient->write("Not implemented yet. :/\n");
					break;
				default:
					currentClient->write("Hey, you wrote: '");
					currentClient->write(command);
					currentClient->write("', but I'm not doing anything with it.\n");
					break;
			}
		}
		else
		{
			currentClient->write(serverName + "Please wait for your turn. Thank you!\n");
		}
	}
#pragma endregion	



}

// Method responsible for checking which player has the character card announced by the King.
// The method (ofcourse) requires the name of the characther card announced in order to do the checking
// returns a simple string representing the player that has the announced card.
// if the none of the players have the announced card, then a "NONE" string will be returned.
string Game::isPlayerTheAnnouncedCharacter(CharacterCard announcedCharacter)
{
	vector<CharacterCard> playerOneHandCards = connectedPlayers.at(0)->getCharacterCardsInHand();
	vector<CharacterCard> playerTwoHandCards = connectedPlayers.at(1)->getCharacterCardsInHand();
	//TODO: Issue, player two does not have any cards in hand after initial round.
	//Check all the character cards in the player one's hand
	for (vector<CharacterCard>::iterator it = playerOneHandCards.begin(); it != playerOneHandCards.end(); ++it)
	{
		// if player One has the announced character card
		if (it->getName() == announcedCharacter.getName()) 
		{
			return "PLAYER1";
		}

	}
	//Check all the character cards in the player two's hand
	for (vector<CharacterCard>::iterator it = playerOneHandCards.begin(); it != playerOneHandCards.end(); ++it)
	{
		// if player Two has the announced character card
		if (it->getName() == announcedCharacter.getName())
		{
			return "PLAYER2";
		}

	}
	
	//Otherwise return false when none of have the announced character card.
	return "NONE";
}

void Game::StartAnnouncingCharacterCards()
{
	broadCastMessage(characterCardsInOrder.at(Server::mGame->announcedCharacterCardCounter));
	Sleep(1000);
	broadCastMessage("Reveal yourself!");

	CharacterCard announcedCharacterCard;
	announcedCharacterCard.mName = characterCardsInOrder.at(Server::mGame->announcedCharacterCardCounter);
	Server::mGame->announcedCharacterCardCounter++;
	string result = Server::mGame->isPlayerTheAnnouncedCharacter(announcedCharacterCard);
	Sleep(1000);
	if (result == "PLAYER1")
	{
		broadCastMessage("Player one has :" + announcedCharacterCard.mName + " !");
		currentGameState = PLAYER_ONE_TURN;
		Server::mGame->sendUpdatedClientDashboard(0);
		Server::mGame->sendUpdatedClientDashboard(1);
		Sleep(1000);
		
	}
	else if (result == "PLAYER2")
	{
		broadCastMessage("Player one has :" + announcedCharacterCard.mName + " !");
		currentGameState = PLAYER_TWO_TURN;
		Server::mGame->sendUpdatedClientDashboard(0);
		Server::mGame->sendUpdatedClientDashboard(1);
		Sleep(1000);
		
	}
	else if (result == "NONE")
	{
		broadCastMessage("King: There are no more players with character the announced character card! I shall announce the next one..");
		Sleep(1000);
		if (Server::mGame->announcedCharacterCardCounter <=7)
		{
			StartAnnouncingCharacterCards();
		}
		
	}
	
}

// Method responsible for resetting all round-specific parameters such as, whether players have collected gold coins already or not and
// whether players have already picked two building cards or not and discard one of the cards.
void Game::resetRound()
{
	connectedPlayers.at(0)->setPlayerGoldCollectionStatus(false);
	connectedPlayers.at(1)->setPlayerGoldCollectionStatus(false);
	
}

//Method responsible for notifying players that the King will start announcing character cards soon.
//Specifically in 30 seconds after all players have gotten enough cards to start playing the game.
void Game::broadCastEverySecond(string message)
{
	int amountOfSeconds = 7;
	for (int i = amountOfSeconds; i >= 0; i--)
	{
		Sleep(1000);
		if (i == amountOfSeconds){
			thread broadcastHandler1{ &Server::sendMessageToAllPlayers, message + to_string(i) + " seconds \n" };
			broadcastHandler1.detach();
		}
		else if (i < 6 && i > 0){
			thread broadcastHandler2{ &Server::sendMessageToAllPlayers, message + to_string(i) + " seconds \n" };
			broadcastHandler2.detach();
		}
		else if (i == 0){
			thread broadcastHandler3{ &Server::sendMessageToAllPlayers, "CLEARSCREEN\n" };
			broadcastHandler3.detach();
		
			currentGameState = KING_GOES_THROUGH_ALL_CHARACTER_CARDS;
			StartAnnouncingCharacterCards();
		}
		
	}
}

void Game::broadCastMessage(string message)
{
	thread broadcastHandler1{ &Server::sendMessageToAllPlayers, message+ " \n" };
	broadcastHandler1.detach();
}
