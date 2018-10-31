#pragma once

#include <vector>
#include <stdlib.h>
#include <ctime>
#include "Card.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Deck
{
	static Deck* instance;

	int mana;
	vector<Card*> hand;
	vector<Card*> playerDeck;
	vector<Card*> discardPile;

public:
	Deck();
	~Deck();

	static Deck * getInstance();

	Card * handAt(int index);
	Card * playerDeckAt(int index);
	Card * discardPileAt(int index);

	int cardsOnHand();				//return number of cards in hand
	int cardsOnPlayerDeck();		//return number of cards in player deck
	int cardsOnDiscardPile();		//return number of cards in discard pile

	void addCardToDeck(Card * c);	//add a new card to player deck
	void drawToHand(int amount);	//draw 'amount' cards and add them to hand
	Card * drawACard();				//return the first card in player deck
	void shufflePlayerDeck();		//as the function name says
	void reshuffleDeckAndPile();	//add all cards in discard pile into player deck
	void discardHand();				//discard all cards in hand into discard pile
	void handToDiscardPile(int index);	//add used card to discard pile and remove it from hand

	void randomMana();			//random amount of mana
	int getMana();				//return current mana
	bool loseMana(int cost);	//allow to lose mana and return true if cost <= mana, if not, return false

};

