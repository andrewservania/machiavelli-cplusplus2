#pragma once
#include <string>

class BuildingCard
{


private:



public:

	std::string mName;
	int mCost;
	std::string mColor;
	std::string mDescription;


	BuildingCard();
	~BuildingCard();

	std::string getName();
	int getCost();
	std::string getColor();
	std::string getDescription();
};

