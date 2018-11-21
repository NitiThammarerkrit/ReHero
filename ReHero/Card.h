#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"
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
	float damage;
	int attack;
	int mana;
	


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
	float effect();
	void setMana(int mana);			//random amount of mana
	int getMana();
	bool active;

	void setEffectCard(SpriteObject, int row, int col);
	void setState(int state);


	int state;
	int c;
};

