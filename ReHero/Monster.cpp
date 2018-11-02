#include <iostream>
#include "stdafx.h"
#include "Monster.h"

using namespace std;

Monster::Monster(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->maxHP = HP;
	this->HP = HP;
}

Monster::~Monster() {

}

int Monster::getHP() {
	return this->HP;
}

int Monster::getMaxHP() {
	return this->maxHP;
}

void Monster::setHP(int amount) {
	this->HP = amount;
}

void Monster::setMaxHP(int amount) {
	this->maxHP = amount;
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

void Monster::heal(int amount) {
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
	}
	else
	{
		this->HP += amount;
	}
}