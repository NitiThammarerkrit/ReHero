#include <iostream>
#include "stdafx.h"
#include "Hero.h"

using namespace std;

Hero::Hero(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->HP = HP;
	this->maxHP = HP;

}

Hero::~Hero() {

}

int Hero::getHP() {
	return this->HP;
}

int Hero::getMaxHP() {
	return this->maxHP;
}

void Hero::setHP(int amount) {
	this->HP = amount;
}

void Hero::setMaxHP(int amount) {
	this->maxHP = amount;
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

void Hero::heal(int amount) {
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
	}
	else
	{
		this->HP += amount;
	}
}