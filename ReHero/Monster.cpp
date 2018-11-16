#include <iostream>
#include "stdafx.h"
#include "Monster.h"
#include "Game.h"

using namespace std;

Monster::Monster(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->maxHP = HP;
	this->HP = HP;
	state = 0;
	c = 0;
}

Monster::~Monster() {

}

void Monster::update(float deltaTime)
{
	timeCount += deltaTime;
	if (timeCount > animationTime / loopMax)
	{
		this->nextAnimation();
		this->genUV();
		timeCount = 0;
	}

	if (Game::getInstance()->state == 2)
	{
		if (state == 0)
		{
			this->translate(glm::vec3(-550, 0, 0));
			state = 1;
		}
		else
			if (state == 1)
			{
				c += deltaTime;
				if (c > 500)
				{
					state = 2;
					c = 0;
				}
			}
			else
				if (state = 2)
				{
					this->translate(glm::vec3(550, 0, 0));
					state = 0;
					Game::getInstance()->state = 0;
				}
	}
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