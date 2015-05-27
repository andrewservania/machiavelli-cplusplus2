#pragma once
#include <vector>
#include "BuildingCard.h"
#include <algorithm>

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
	T at(int index);
	void removeAt(int index);
	int getCardStackSize();
	void shuffle();
	bool loadCSV();
};

template <class T>
T CardStack<T>::at(int index)
{
	return cards.at(index);
}

template <class T>
int CardStack<T>::getCardStackSize()
{
	return cards.size();
}

template <class T>
void CardStack<T>::removeAt(int index)
{
	cards.erase(cards.begin() + index);
	cards.shrink_to_fit();
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
	std::random_shuffle(cards.begin(), cards.end());
}

template<class T>
bool CardStack<T>::loadCSV(){

}

