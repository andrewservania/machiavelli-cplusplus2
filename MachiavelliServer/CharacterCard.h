#pragma once
#include <string>
class CharacterCard
{
private:
	std::string name;
	std::string description;

public:
	CharacterCard();
	~CharacterCard();
	std::string getName();
	std::string getDescription();
};

