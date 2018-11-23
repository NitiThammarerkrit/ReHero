#include <iostream>
#include "stdafx.h"
#include "Monster.h"
#include "Game.h"

#define POISON_DMG 2

using namespace std;

Monster::Monster(int HP, string name, int row, int column) : SpriteObject(name + ".png", row, column) {
	srand(time(NULL));
	
	this->maxHP = HP;
	this->HP = HP;
	this->name = name;
	defArmor = 0;
	isPoisoned = false;

	state = 0;
	c = 0;

	skillData = new ifstream(name + ".txt");

	randomUseSkill(nullptr, nullptr);

}

Monster::~Monster() {
	skillData->close();
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
					Game::getInstance()->state = 3;
				}
	}
}

int Monster::getHP() {
	return this->HP;
}

int Monster::getMaxHP() {
	return this->maxHP;
}

int Monster::getArmor() {
	return this->defArmor;
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

void Monster::getHeal(int amount) {
	cout << name << " heal " << amount << " to himself" << endl;
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
	}
	else
	{
		this->HP += amount;
	}
	
}

void Monster::gainArmor(int amount) {
	defArmor += amount;
	cout << name << "gain " << amount << " armor" << endl;
}

void Monster::takePoison() {
	isPoisoned = true;
}

void Monster::startTurn() {
	defArmor = 0;
	if (isPoisoned)
	{
		cout << name << " take " << POISON_DMG << " damage from Poison."<< endl;
		HP -= POISON_DMG;
	}
}

void Monster::randomUseSkill(Hero * enemyTarget, Monster * friendTarget) {
	if (skillData == nullptr)
	{
		cout << "data file lost" << endl;
		return;
	}
	
	//this monster has how many skill?
	int howManySkill;
	*skillData >> howManySkill;

	//random select one
	int selectedSkill = rand() % howManySkill;
	string skill;
	getline(*skillData, skill, '\n');

	//loop for every skills
	for (int i = 0; i < howManySkill; i++)
	{
		getline(*skillData, skill, '\n');
		if (i == selectedSkill)
		{
			//get selected skill's name first
			stringstream usedSkill(skill);
			string effect;
			int value;
			getline(usedSkill, effect, '\t');
			cout << name << " use " << effect << endl;
			while (1)
			{
				//read data of the selected skill (each action)
				usedSkill >> effect >> value;

				//perform a skill
				if (effect == "damage") doDamage(enemyTarget, value);
				else if (effect == "heal") heal(friendTarget, value);
				else if (effect == "poison") usePoison(enemyTarget);
				else if (effect == "defend") gainArmor(value);
				else break;
			}

			break;
		}

	}
}

void Monster::doDamage(Hero * target, int damage) {
	if (target != nullptr)
	{
		target->takeDamage(damage);
		cout << name << " deal " << damage << " damage to Hero" << endl;
	}
}

void Monster::heal(Monster * target, int amount) {
	if (target != nullptr)
	{
		target->getHeal(amount);
		cout << "heal " << amount << " for " << target->name << endl;
	}
	else
	{
		this->getHeal(amount);
	}
	
}

void Monster::usePoison(Hero * target) {
	if (target != nullptr)
	{
		target->takePoison();
		cout << name << " use poison to Hero" << endl;
	}
}