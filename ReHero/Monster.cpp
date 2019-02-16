#include <iostream>
#include "stdafx.h"
#include "Monster.h"
#include "Game.h"

#define POISON_DMG 2

using namespace std;

Monster::Monster(int HP, string name, int row, int column) : SpriteObject("Sprite/" + name + ".png", row, column) {
	srand(time(NULL));
	
	this->maxHP = HP;
	this->HP = HP;
	this->name = name;
	defArmor = 0;
	isPoisoned = false;
	damage = 0;
	getAttack = false;
	isHeal = false;	 
	isAttack = false;
	state = 0;
	c = 0;

	loadSkillData(name + ".txt");

	//randomUseSkill(nullptr, nullptr);
}

Monster::~Monster() {
	skillData.clear();
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
	if (getAttack)
	{
		HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		//cout << "\nMonster HP:" << this->getHP();
		cout << endl << "damage is" << damage;
		HPBar->translate(glm::vec3(-damage / 2.0f / 20.0f*250.0f, 0.0f, 0.0f));
		getAttack = false;
	}
	if (Game::getInstance()->state == 2 && isAttack == true)
	{

	//	cout << " SAS "<<endl;
		if (state == 0)
		{
			//this->translate(glm::vec3(-550, 0, 0)) ;
			//Game::getInstance()->damageText->translate(glm::vec3(0, 10, 0));
			this->setAnimationLoop(2, 1, 3, 700);
			state = 1;
		}
		else
			if (state == 1)
			{
				c += 1*deltaTime;
				
				if (c > 710)
				{
					state = 2;
					c = 0;
				}
			}
			else
				if (state = 2)
				{
					//this->translate(glm::vec3(550, 0, 0));
					this->setAnimationLoop(1, 1, 3, 400);
					state = 0;
					isAttack = false;
					Game::getInstance()->state = 3;
				}
	}
	else
	if (Game::getInstance()->state == 2 && isHeal==true)
	{
		Game::getInstance()->state = 3;
		isHeal = false;
	}



}

bool Monster::isAlive() {
	if (HP > 0) return true;
	else return false;
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
	this->damage = damage;
	Game::getInstance()->DamageAmount = damage;
	getAttack = true;
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

	this->damage = -amount;
	isHeal = true;
	getAttack = true;
	cout << name << " heal " << amount << " to himself" << endl;
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
		this->damage = 0;
	}
	else
	{
		this->HP += amount;
	}
	
}

void Monster::gainArmor(int amount) {  
	isHeal = true;
	defArmor += amount;
	cout << name << "gain " << amount << " armor" << endl;
}

void Monster::takePoison() {
	getAttack = true;
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

void Monster::loadSkillData(string fileName) {
	//load a skill data file
	ifstream datafile(fileName);
	if (!datafile)
	{
		cout << "fail to load skill data for " << this->name << endl;
		return;
	}

	//this monster has how many skill?
	int howManySkill;
	string skill;
	datafile >> howManySkill;
	getline(datafile, skill, '\n');

	//get all skill data
	for (int i = 0; i < howManySkill; i++)
	{
		getline(datafile, skill, '\n');
		skillData.push_back(skill);
	}

	datafile.close();
}

void Monster::randomUseSkill(Hero * enemyTarget, Monster * friendTarget) {

	//random skill
	int selectedSkill = rand() % skillData.size();

	//loop to find the selected skill
	for (int i = 0; i < skillData.size(); i++)
	{
		if (i == selectedSkill)
		{
			stringstream selectedData(skillData[i]);
			string effect;
		    int value;
			getline(selectedData, effect, '\t'); //get skill name
			cout << name << " use " << effect << endl;
			while (1)
			{
				//read data of the selected skill (each action)
				selectedData >> effect >> value;

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
	isAttack = true;
	if (target != nullptr)
	{
		target->takeDamage(damage);
		cout << name << " deal " << damage << " damage to Hero" << endl;
	}
	else cout << name << " deal " << damage << " damage, but no target!" << endl;
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
	isAttack = true;
	if (target != nullptr)
	{
		target->takePoison();
		cout << name << " use poison to Hero" << endl;
	}
	else cout << name << " use poison, but no target!" << endl;
}