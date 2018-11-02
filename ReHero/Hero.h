#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"

class Hero : public SpriteObject
{
	int HP;

public:
	Hero(int HP, string fileName, int row, int column);
	~Hero();

	int getHP();
	void setHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die

};

