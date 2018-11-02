#include <iostream>
#include "stdafx.h"
#include "Monster.h"

using namespace std;

Monster::Monster(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->HP = HP;
}

Monster::~Monster() {

}

int Monster::getHP() {
	return this->HP;
}

void Monster::setHP(int amount) {
	this->HP = amount;
}

bool Monster::takeDamage(int damage) {
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