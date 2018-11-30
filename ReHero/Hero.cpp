#include <iostream>
#include "stdafx.h"
#include "Hero.h"
#include "Game.h"

#define POISON_DMG 2

using namespace std;

Hero::Hero(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->HP = HP;
	this->maxHP = HP;
	state = 0;
	c = 0;
}

Hero::~Hero() {

}

void Hero::update(float deltaTime)
{
	timeCount += deltaTime;
	if (timeCount > animationTime / loopMax)
	{
		this->nextAnimation();
		this->genUV();
		timeCount = 0;
	}
	if (Game::getInstance()->state == 1)                          
	{
		if (state == 0)
		{
			this->translate(glm::vec3(550, 0, 0));
			this->setAnimationLoop(2, 1, 4, 700);
			state = 1;
		}
		else
		if (state == 1)
		{
			c += 1*deltaTime;
			if (c > 500)
			{
				state = 2;
				c = 0;
			}
		}
		else
		if (state == 2)
		{
			this->translate(glm::vec3(-550, 0, 0));
			this->setAnimationLoop(1, 1, 6, 800);
			//this->setTexture(temptexture);
			state = 0;
			Game::getInstance()->state = 0;
		}
	}

}

int Hero::getHP() {
	return this->HP;
}

int Hero::getMaxHP() {
	return this->maxHP;
}

int Hero::getArmor() {
	return this->defArmor;
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

bool Hero::isAlive() {
	if (HP > 0) return true;
	else return false;
}

void Hero::getHeal(int amount) {
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
	}
	else
	{
		this->HP += amount;
	}
}

void Hero::gainArmor(int amount) {
	defArmor += amount;
	cout << "gain " << amount << " armor" << endl;
}

void Hero::takePoison() {
	isPoisoned = true;
}

void Hero::startTurn() {
	defArmor = 0;
	if (isPoisoned)
	{
		cout << "Hero take " << POISON_DMG << " damage from Poison." << endl;
		HP -= POISON_DMG;
	}
}