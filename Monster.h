#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"

class Monster : public DrawableObject
{
	int maxHP;				  
	int HP;
	SpriteObject * monsterSprite;
	SpriteObject * healthBarBackground;
	SpriteObject * healthBar;

public:
	Monster(int HP, string fileName, int row, int column);
	~Monster();

	int getHP();
	int getMaxHP();
	void setHP(int amount);
	void setMaxHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die
	void heal(int amount);
	void render(glm::mat4 globalModelTransform);

};