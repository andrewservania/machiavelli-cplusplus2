#pragma once
#include <string>

class BuildingCard
{
private:
	std::string name;
	std::string description;
	int cost;

public:
	BuildingCard();
	~BuildingCard();
	std::string getName();
	std::string getDescription();
	int getCost();
};

