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
	int rowMax;
	int columnMax;
	int row;
	int column;
	float uv[8];
	int ID;

	int attack;
	


public:
	Card();
	~Card();
	void setColor(float r, float g, float b);
	void render(glm::mat4 globalModelTransform);
	void setName(string n);
	void update(float deltaTime);
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

	void effect();

	bool active;
};

