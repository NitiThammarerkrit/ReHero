#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"

class Hero : public SpriteObject
{
	int maxHP;
	int HP;

public:
	Hero(int HP, string fileName, int row, int column);
	~Hero();

	virtual void update(float deltaTime);
	int getHP();
	int getMaxHP();
	void setHP(int amount);
	void setMaxHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die
	void heal(int amount);
	int state;
	int c;

};

