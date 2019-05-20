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
	poison = 0;
	damage = 0;
	getAttack = false;
	isHeal = false;	 
	isAttack = false;
	state = 0;
	c = 0;
	delay = 0;
	oneTime = true;

	loadSkillData("Data_" + name + ".txt");

	//randomUseSkill(nullptr, nullptr);
}

Monster::~Monster() {
	skillData.clear();
}

void Monster::changeMonster(string name, int HP) {
	this->name = name;
	this->changeSprite("Sprite/" + this->name + ".png", 4, 5);
	cout << "--------LOAD Data_" + this->name + ".txt" << endl;
	this->loadSkillData("Data_" + this->name + ".txt");
	this->setMaxHP(HP);
	this->setHP(HP);

	defArmor = 0;
	poison = 0;
	damage = 0;
	getAttack = false;
	isHeal = false;
	isAttack = false;
	state = 0;
	c = 0;
	delay = 0;
	oneTime = true;
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
	if (defArmor > 0 && Game::getInstance()->state != State::PLAYER_DIE && Game::getInstance()->state != State::ENEMY_DIE && Game::getInstance()->state != State::TRANSITION)
	{
		Game::getInstance()->drawEffectText(to_string(defArmor), 2, 4, true);
	}
	else
		Game::getInstance()->drawEffectText(to_string(defArmor), 2, 4, false);
	if (poison > 0 && Game::getInstance()->state != State::PLAYER_DIE && Game::getInstance()->state != State::ENEMY_DIE && Game::getInstance()->state != State::TRANSITION)
	{
		Game::getInstance()->drawEffectText(to_string(poison), 3, 3, true);
	}
	else
		Game::getInstance()->drawEffectText(to_string(poison), 3, 3, false);

	if (!effect.empty()&&isAlive())
	{
		if (effect.front() == "damage")
		{
			if (oneTime == true)
			{
				Game::getInstance()->effectOnPlayer->setAnimationLoop(10, 1, 7, 700);
				Game::getInstance()->effectOnPlayer->setActive(true);
				Game::getInstance()->state = State::ENEMY_ATTACK_ANIM;
				this->setAnimationLoop(2, 1, 5, 700);
				if (!monsterMakeDamage.empty())
				{
					doDamage(enemyTarget, monsterMakeDamage.front());
					if (monsterMakeDamage.front() > 0) Game::getInstance()->drawText(to_string(abs(monsterMakeDamage.front())), glm::vec3(-350.0f, 0.f, 0.f), abs((monsterMakeDamage.front()*2) + 15.f), 2);
					monsterMakeDamage.pop();
				}
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				Game::getInstance()->effectOnPlayer->setActive(false);
				delay = 0;
				oneTime = true;
				effect.pop();
				if (effect.empty()) endMyTurn();
			}
		}
		else if (effect.front() == "heal")
		{
			if (oneTime == true)
			{
				Game::getInstance()->state = State::ENEMY_HEAL_ANIM;

				this->setAnimationLoop(3, 1, 4, 600);
				if (!monsterMakeDamage.empty())
				{
					heal(friendTarget, monsterMakeDamage.front());
					Game::getInstance()->effectOnEnemy->setAnimationLoop(3, 1, 5, 600);
					Game::getInstance()->effectOnEnemy->setActive(true);
					Game::getInstance()->drawText(to_string(abs(monsterMakeDamage.front())), glm::vec3(350.0f, 0.f, 0.f), abs((monsterMakeDamage.front()*2)+15.f), 1);
					monsterMakeDamage.pop();
				}
				HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
				HPBar->setPosition(glm::vec3((float)350.0f -
					((float)this->getMaxHP() - (float)this->getHP())
					/ ((float)this->getMaxHP() / (float)(250.0f / 2.0f))
					, 200.0f, 0.0f));
				oneTime = false;
			}	
			delay += 1 * deltaTime;
			if (delay > 610)
			{
				Game::getInstance()->effectOnEnemy->setActive(false);
				delay = 0;
				oneTime = true;
				effect.pop();
				if (effect.empty()) endMyTurn();
			}
		}
		else if (effect.front() == "poison")
		{
			if (oneTime == true)
			{
				Game::getInstance()->effectOnPlayer->setAnimationLoop(11, 1, 7, 600);
				Game::getInstance()->effectOnPlayer->setActive(true);
				Game::getInstance()->state = State::ENEMY_SPELL_ANIM;
				this->setAnimationLoop(3, 1, 4, 600);
				if (!monsterMakeDamage.empty())
				{
					usePoison(enemyTarget, monsterMakeDamage.front());
					if (monsterMakeDamage.front() > 0) Game::getInstance()->drawText(to_string(abs(monsterMakeDamage.front())), glm::vec3(-350.0f, 0.f, 0.f), abs((monsterMakeDamage.front() * 2) + 15.f), 3);
					monsterMakeDamage.pop();
				}
			}
			oneTime = false;
			delay += 1 * deltaTime;
			if (delay > 610)
			{
				Game::getInstance()->effectOnPlayer->setActive(false);
				delay = 0;
				oneTime = true;
				effect.pop();
				//Game::getInstance()->drawText(to_string(abs(monsterMakeDamage.front())), glm::vec3(-350.0f, 0.f, 0.f), abs(monsterMakeDamage.front()) * 40.0f, 3);
				if (effect.empty()) endMyTurn();
			}
		}
		else if (effect.front() == "defend")
		{
			if (oneTime == true)
			{
				this->setAnimationLoop(3, 1, 4, 700);
				Game::getInstance()->state = State::ENEMY_DEFENSE_ANIM;
				if (!monsterMakeDamage.empty())
				{
					Game::getInstance()->effectOnEnemy->setAnimationLoop(9, 1, 9, 700);
					Game::getInstance()->effectOnEnemy->setActive(true);
					
					gainArmor(monsterMakeDamage.front());
					Game::getInstance()->drawText(to_string(abs(monsterMakeDamage.front())), glm::vec3(350.0f, 0.f, 0.f), abs((monsterMakeDamage.front() * 2) + 15.f), 3);
					monsterMakeDamage.pop();
				}

				oneTime = false;
			}				
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				Game::getInstance()->effectOnEnemy->setActive(false);
				delay = 0;
				oneTime = true;
				effect.pop();
				if (effect.empty()) endMyTurn();
			}
		}
	}

	if ((getAttack||isHeal)&&isAlive())
	{
		HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		HPBar->setPosition(glm::vec3((float)350.0f -
			((float)this->getMaxHP() - (float)this->getHP())
			/ ((float)this->getMaxHP() / (float)(250.0f / 2.0f))
			, 200.0f, 0.0f));
		//HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		////cout << "\nMonster HP:" << this->getHP();
		////cout << endl << "damage is" << damage;
		//HPBar->translate(glm::vec3(-damage / 2.0f / (float)this->getMaxHP()*250.0f, 0.0f, 0.0f));
		if (oneTime == true && getAttack == true)
		this->setAnimationLoop(4, 1, 1, 200);
		if (oneTime == true && isHeal == true)
		this->setAnimationLoop(3, 1, 4, 200);
		oneTime = false;
		damage = 0;
		delay += deltaTime;
		if (delay > 410)
		{
			delay = 0;
			this->setAnimationLoop(1, 1, 5, 400);
			if(getAttack)
			getAttack = false;
			if (isHeal)
				isHeal = false;
			oneTime = true;
		}
	}	
	if (isAlive() == false)
	{
		setAnimationLoop(5, 1, 2, 400);
		delay += deltaTime;
		if (delay > 410)
		{
			delay = 0;
			victory->setActive(true);
			Game::getInstance()->state = State::ENEMY_DIE;
		}
	}
	if (Game::getInstance()->state == State::TRANSITION)
	{
		HPBar->setActive(false);
	}
	if (Game::getInstance()->state == State::GAME_MAP)
	{
		HPBar->setActive(true);
		victory->setActive(false);
	}
}

void Monster::endMyTurn() {
	effect = {};
	monsterMakeDamage = {};
	this->setAnimationLoop(1, 1, 5, 400);
	Game::getInstance()->state = State::PLAYER_RANDOM_MANA;
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

int Monster::getPoison() {
	return this->poison;
}

void Monster::setHP(int amount) {
	this->HP = amount;
}

void Monster::setMaxHP(int amount) {
	this->maxHP = amount;
}

bool Monster::takeDamage(int damage) {
	this->damage = damage;
	getAttack = true;

	int leftoverDMG = damage - this->defArmor;
	if (leftoverDMG <= 0)
	{
		this->defArmor -= damage;
		return true;
	}
	else
	{
		this->defArmor = 0;
	}

	if (this->HP - leftoverDMG > 0)
	{
		//survive the damage
		this->HP -= leftoverDMG;
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
	//getAttack = true;
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
	//isHeal = true;
	defArmor += amount;
	cout << name << "gain " << amount << " armor" << endl;
}

void Monster::takePoison(int amount) {
	getAttack = true;
	poison += amount;
	//Game::getInstance()->drawText(to_string(poison), glm::vec3(350.0f, 0.f, 0.f), abs((poison * 2) + 15.f), 2);

}

void Monster::startTurn() {
	Game::getInstance()->state = State::ENEMY_CONDITION;
	defArmor = 0;
	if (poison > 0)
	{
		cout << name << " take " << poison << " damage from Poison."<< endl;
		this->takeDamage(poison);
		Game::getInstance()->drawText(to_string(abs(poison)), glm::vec3(350.0f, 0.f, 0.f), abs((poison * 2) + 15.f), 2);
		poison -= 1;
	}
	/*if (isAlive()==false)
	{
		Game::getInstance()->state = State::ENEMY_DIE;
	}*/

}

void Monster::loadSkillData(string fileName) {
	//load a skill data file
	ifstream datafile(fileName);
	if (!datafile)
	{
		cout << "fail to load skill data for " << this->name << endl;
		return;
	}

	skillData.clear();

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

	this->enemyTarget = enemyTarget;
	this->friendTarget = friendTarget;

	//random skill
	int selectedSkill = rand() % skillData.size();

	//check if this is boss monster
	if (this->name == "BossSpider" || this->name == "BossDemon")
	{
		for (int i = 0; i < skillData.size(); i++)
		{
			if (i == selectedSkill)
			{
				stringstream selectedData(skillData[i]);
				string effect;
				int value1, value2, value3;
				getline(selectedData, effect, '\t'); //get skill name

				while (1)
				{
					//read data of the selected skill (each action)

					selectedData >> effect >> value1 >> value2 >> value3;

					if (effect == ";") break;

					this->effect.push(effect);
					if (this->HP >= 70)
					{
						this->monsterMakeDamage.push(value1);
						cout << "push " << effect << " " << value1 << endl;
					}
					else if (this->HP >= 30)
					{
						this->monsterMakeDamage.push(value2);
						cout << "push " << effect << " " << value2 << endl;
					}
					else 
					{
						this->monsterMakeDamage.push(value3);
						cout << "push " << effect << " " << value3 << endl;
					}
				}

				break;
			}
		}

		return;
	}
	
	//loop to find the selected skill
	for (int i = 0; i < skillData.size(); i++)
	{
		if (i == selectedSkill)
		{
			stringstream selectedData(skillData[i]);
			string effect;
		    int value;
			getline(selectedData, effect, '\t'); //get skill name
			
			while (1)
			{
				//read data of the selected skill (each action)

				selectedData >> effect >> value;

				if (effect == ";") break;

				this->effect.push(effect);
				this->monsterMakeDamage.push(value);

				cout << "push " << effect << " " << value << endl;
				
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

void Monster::usePoison(Hero * target, int amount) {
	isAttack = true;
	if (target != nullptr)
	{
		target->takePoison(amount);
		cout << name << " use poison " << amount << " to Hero" << endl;
	}
	else cout << name << " use poison " << amount << ", but no target!" << endl;
}

void Monster::curePoison() {
	this->poison = 0;
}

/*void Monster::SetAnim(int animRow,int loopNum, int time)
{
	cout << endl << "state is " << state << endl;
	while (state == 0)
	{
		this->setAnimationLoop(animRow, 1, loopNum, time);
		state = 1;
	}
	while (state == 1)
	{
		c += 1;
		if (c > time + 10)
		{
			state = 2;
			c = 0;
		}
	}
	while (state == 2)
	{
		this->setAnimationLoop(1, 1, 5, 400);
		state = 0;
		monsterMakeDamage = 0;
		Game::getInstance()->state = State::ENEMY_CONDITION;
	}
}
void Monster::CheckState()
{
	if (Game::getInstance()->state == State::ENEMY_ATTACK_ANIM)
	{
		SetAnim(2, 5, 600);
		//Game::getInstance()->drawText(to_string(monsterMakeDamage), glm::vec3(-350.0f, 0.f, 0.f), monsterMakeDamage*50.0f);
	}
	else
	if (Game::getInstance()->state == State::ENEMY_HEAL_ANIM)
	{
		//Game::getInstance()->drawText(to_string(monsterMakeDamage), glm::vec3(350.0f, 0.f, 0.f), monsterMakeDamage*50.0f);
		SetAnim(3, 4, 600);
	}
	else
	if (Game::getInstance()->state == State::ENEMY_DEFENSE_ANIM)
	{
		SetAnim(3, 4, 600);
	}
	else
	if (Game::getInstance()->state == State::ENEMY_SPELL_ANIM)
	{
		SetAnim(2, 5, 600);
	}
	else
	if (Game::getInstance()->state == State::ENEMY_PAY_HP_ANIM)
	{
		SetAnim(3, 4, 600);
	}
	else
	if (Game::getInstance()->state == State::ENEMY_DIE)
	{
		active = false;
		//SetAnim(3, 4, 600, deltaTime);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_ATTACK_ANIM)
	{
		SetAnim(4, 1, 300);
	}		
}		   */