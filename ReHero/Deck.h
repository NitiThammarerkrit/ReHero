#pragma once

#include <vector>
#include <stdlib.h>
#include <ctime>
#include "Card.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Deck
{
	vector<Card*> hand;
	vector<Card*> playerDeck;
	vector<Card*> discardPile;

public:
	Deck();
	~Deck();

	void addCardToDeck(Card * c);	//add a new card to player deck
	void drawToHand(int amount);	//draw 'amount' cards and add them to hand
	Card * drawACard();				//return the first card in player deck
	void shufflePlayerDeck();		//as the function name says
	void reshuffleDeckAndPile();	//add all cards in discard pile into player deck
	void discardHand();				//discard all cards in hand into discard pile

};

