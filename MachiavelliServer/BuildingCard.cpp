#include "stdafx.h"
#include "BuildingCard.h"


BuildingCard::BuildingCard(){
	name = "new_building_name";
	description = "This is a funky building";
	cost = 7;

}


BuildingCard::~BuildingCard()
{
}

std::string BuildingCard::getName()
{
	return name;
}

std::string BuildingCard::getDescription()
{
	return description;
}

int BuildingCard::getCost()
{
	return cost;
}
