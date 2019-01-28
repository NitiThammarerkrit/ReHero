#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"
#include "Hero.h"
#include "Monster.h"

class Card :public DrawableObject
{
	glm::vec3 color;
	string name;
	float cardAngle;
	glm::vec3 positionDrag;
	bool isInHand;
	unsigned int texture;
	unsigned int effectTexture;
	unsigned int tempTexture;
	int rowMax;
	int columnMax;
	int row;
	int column;
	float uv[8];
	int ID;
	//float damage;
	int attack;
	int mana;
	string effectData;
	


public:
	Card();
	~Card();
	void setColor(float r, float g, float b);
	void render(glm::mat4 globalModelTransform);
	void setName(string n);
	virtual void update(float deltaTime);
	string getName();
	bool isClick(float x, float y);
	float getCardAngle();
	void setCardAngle(float angle);
	void setPositionDrag(float x, float y);
	void setIsInHand(bool x);

	void setRow(int r);
	void setColumn(int c);
	int getRow();
	int getColumn();
	void genUV();
	void setSpriteCard(SpriteObject, int row, int col);
	void setId(int id);
	int getId();

	unsigned int getTexture();
	void effect(Hero * friendTarget, Monster * enemyTarget);
	void setMana(int mana);			
	int getMana();
	bool active;

	void setEffectCard(SpriteObject, int row, int col);
	void setState(int state);

	//card effect functions
	void doDamage(Monster * target, int damage);
	void heal(Hero * target, int amount);
	void usePoison(Monster * target);
	void gainArmor(Hero * target, int amount);
	void drawCard(int amount);

	int state;
	int c;
};

