#pragma once
#include "Player.h"
#include <vector>
#include "CardStack.h"

class Game
{
private:
	std::vector<Player> players;
	CardStack<BuildingCard> buildings;
	CardStack<CharacterCard> characters;

public:
	Game();
	~Game();

	void initServer();
	void waitForClients();
	void run();
	void initGame();
	void dealCharacters();
	void discardCharacter();
	void showCharacter();
	void pickCharacter();
	void playRound();
	void countPlayerScores();
	void playCharacter();
};

