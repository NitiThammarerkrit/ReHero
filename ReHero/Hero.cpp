#include <iostream>
#include "stdafx.h"
#include "Hero.h"

using namespace std;

Hero::Hero(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->HP = HP;
}

Hero::~Hero() {

}

int Hero::getHP() {
	return this->HP;
}

void Hero::setHP(int amount) {
	this->HP = amount;
}

bool Hero::takeDamage(int damage) {
	if (this->HP - damage > 0)
	{
		//survive the damage
		this->HP -= damage;
		return true;
	}
	else
	{
		//die
		this->HP = 0;
		return false;
	}
}