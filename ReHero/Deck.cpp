#include "stdafx.h"
#include "Deck.h"

Deck::Deck() {
	srand(time(NULL));

	//code for system testing
	//			|
	//			|
	//			V

	/*
	Card * c1 = new Card();
	c1->setName("Prame");
	playerDeck.push_back(c1);

	Card * c2 = new Card();
	c2->setName("Leo");
	playerDeck.push_back(c2);

	Card * c3 = new Card();
	c3->setName("Tem");
	playerDeck.push_back(c3);

	Card * c4 = new Card();
	c4->setName("Act");
	playerDeck.push_back(c4);

	Card * c5 = new Card();
	c5->setName("d1");
	discardPile.push_back(c5);

	Card * c6 = new Card();
	c6->setName("d2");
	discardPile.push_back(c6);

	Card * c7 = new Card();
	c7->setName("d3");
	discardPile.push_back(c7);

	cout << "default deck" << endl;
	for (int i = 0; i < playerDeck.size(); i++)
	{
		cout << playerDeck[i]->getName() << endl;
	}
	cout << "default discard pile == " << endl;
	for (int i = 0; i < discardPile.size(); i++)
	{
		cout << discardPile[i]->getName() << endl;
	}
	shufflePlayerDeck();
	cout << "after shuffle" << endl;
	for (int i = 0; i < playerDeck.size(); i++)
	{
		cout << playerDeck[i]->getName() << endl;
	}
	cout << "ore no turn... 5 DRAW!!" << endl;
	drawToHand(5);
	for (int i = 0; i < hand.size(); i++)
	{
		cout << hand[i]->getName() << endl;
	}
	cout << "Deck == " << endl;
	for (int i = 0; i < playerDeck.size(); i++)
	{
		cout << playerDeck[i]->getName() << endl;
	}
	cout << "I discard my hand!!" << endl;
	discardHand();
	cout << "hand == " << endl;
	for (int i = 0; i < hand.size(); i++)
	{
		cout << hand[i]->getName() << endl;
	}
	cout << "discard pile == " << endl;
	for (int i = 0; i < discardPile.size(); i++)
	{
		cout << discardPile[i]->getName() << endl;
	}
	reshuffleDeckAndPile();
	cout << "reshuffle!! Deck == " << endl;
	for (int i = 0; i < playerDeck.size(); i++)
	{
		cout << playerDeck[i]->getName() << endl;
	}
	cout << "discard pile == " << endl;
	for (int i = 0; i < discardPile.size(); i++)
	{
		cout << discardPile[i]->getName() << endl;
	}
	*/

}

Deck::~Deck() {
	//clear hand
	int count = hand.size();
	for (int i = 0; i < count; i++) delete hand[i];
	hand.clear();
	//clear player deck
	count = playerDeck.size();
	for (int i = 0; i < count; i++) delete playerDeck[i];
	playerDeck.clear();
	//clear discard pile
	count = discardPile.size();
	for (int i = 0; i < count; i++) delete discardPile[i];
	discardPile.clear();
}

void Deck::addCardToDeck(Card * c) {
	playerDeck.push_back(c);
}

void Deck::drawToHand(int amount) {
	//loop function drawACard = 'amount' times
	for (int i = 0; i < amount; i++)
	{
		hand.push_back(drawACard());
	}
}

Card * Deck::drawACard() {
	if (playerDeck.size() > 0)
	{
		//if cards in player deck are more than 0
		//draw normally
		Card * temp = playerDeck[0];
		playerDeck[0] = nullptr;
		playerDeck.erase(playerDeck.begin() + 0);
		return temp;
	}
	else
	{
		//if card in player deck is 0
		//swap player deck with discard pile first
		reshuffleDeckAndPile();

		//then draw
		Card * temp = playerDeck[0];
		playerDeck[0] = nullptr;
		playerDeck.erase(playerDeck.begin() + 0);
		return temp;
	}
}

void Deck::shufflePlayerDeck() {
	//create and copy player deck to temp
	vector<Card*> temp;
	int count = playerDeck.size();
	for (int i = 0; i < count; i++)
	{
		temp.push_back(playerDeck[i]);
	}

	//clear player deck
	playerDeck.clear();

	//randomly put card from temp into player deck
	count = temp.size();
	for (int i = 0; i < count; i++)
	{
		int ran = rand() % temp.size();
		playerDeck.push_back(temp[ran]);
		temp[ran] = nullptr;
		temp.erase(temp.begin() + ran);
	}
}

void Deck::reshuffleDeckAndPile() {
	//add all cards in discard pile into player deck
	int count = discardPile.size();
	for (int i = 0; i < count; i++)
	{
		playerDeck.push_back(discardPile[i]);
	}

	//clear discard pile & shuffle player deck
	discardPile.clear();
	shufflePlayerDeck();
}

void Deck::discardHand() {
	//add all cards in hand into discard pile
	int count = hand.size();
	for (int i = 0; i < count; i++)
	{
		discardPile.push_back(hand[i]);
	}

	//clear hand
	hand.clear();
}