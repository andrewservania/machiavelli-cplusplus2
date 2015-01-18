#include "stdafx.h"
#include "BuildingCard.h"


BuildingCard::BuildingCard(){
	mName = "empty";
	mCost = 0;
	mColor = "empty";
	mDescription = "empty";
}


BuildingCard::~BuildingCard()
{
}

std::string BuildingCard::getName()
{
	return mName;
}

int BuildingCard::getCost()
{
	return mCost;
}

std::string BuildingCard::getColor()
{
	return mColor;
}

std::string BuildingCard::getDescription()
{
	return mDescription;
}


