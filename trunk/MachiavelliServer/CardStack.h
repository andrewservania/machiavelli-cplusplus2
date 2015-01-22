#pragma once
#include <vector>
#include "BuildingCard.h"

template <class T>
class CardStack
{
private:
	std::vector<T> cards;

public:
	CardStack();
	~CardStack();
	// add card to stack with this method
	void add(T card);
	void removeAt(int index);
	int getCardStackSize();
	void shuffle();
	bool loadCSV();
};

template <class T>
int CardStack<T>::getCardStackSize()
{
	return cards.size();
}

template <class T>
void CardStack<T>::removeAt(int index)
{
	cards.erase(cards.begin() + index);
}

template<class T>
CardStack<T>::CardStack()
{
	
}

template<class T>
CardStack<T>::~CardStack()
{
}

template<class T>
void CardStack<T>::add(T card){
	//TODO: not implemented yet: add card to stack here..



	cards.push_back(card);
}

template<class T>
void CardStack<T>::shuffle(){

}

template<class T>
bool CardStack<T>::loadCSV(){

}

