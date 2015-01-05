#pragma once
#include <vector>
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
};


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