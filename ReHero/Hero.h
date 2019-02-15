#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"




class Hero : public SpriteObject
{
	int maxHP;
	int HP;
	int defArmor;
	bool isPoisoned;
	bool getAttack;
	bool isHeal;
	bool isAttack;
	int damage;
	bool isGetAttack;
	

public:
	Hero(int HP, string fileName, int row, int column);
	~Hero();

	virtual void update(float deltaTime);
	int getHP();
	int getMaxHP();
	int getArmor();
	void setHP(int amount);
	void setMaxHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die
	bool isAlive();
	void getHeal(int amount);
	void gainArmor(int amount);
	void takePoison();
	virtual void render(glm::mat4 globalModelTransform);
	void setAttack(bool A);
	void startTurn(); //resolve every special condition & remove armor from last turn
	
	int state;
	int c;
	int delay;
	DrawableObject* HPBar;
	//DrawableObject * DamageText;
};

