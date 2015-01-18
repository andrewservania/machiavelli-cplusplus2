#pragma once
#include <string>
class CharacterCard
{
private:


public:
	int mID;
	std::string mName;

	CharacterCard();
	~CharacterCard();
	int getID();
	std::string getName();
	
};

