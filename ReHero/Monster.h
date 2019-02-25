#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Hero.h"

class Monster : public SpriteObject
{
	int maxHP;
	int HP;
	int defArmor;
	bool isPoisoned;
	int damage;
	bool getAttack;
	bool isHeal;
	bool isAttack;

	string name;
	vector<string> skillData;
	vector<string> itemDrop;

public:
	Monster(int HP, string name, int row, int column);
	~Monster();

	virtual void update(float deltaTime);
	void loadSkillData(string fileName);

	int getHP();
	int getMaxHP();
	int getArmor();
	void setHP(int amount);
	void setMaxHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die
	void getHeal(int amount);
	void gainArmor(int amount);
	void takePoison();
	void curePoison();
	
	void startTurn(); //resolve every special condition & remove armor from last turn

	//monster skills
	void randomUseSkill(Hero * enemyTarget, Monster * friendTarget);
	void doDamage(Hero * target, int damage);
	void heal(Monster * target, int amount);
	void usePoison(Hero * target);
	bool isAlive();
	int state;
	int c;
	DrawableObject* HPBar;

};