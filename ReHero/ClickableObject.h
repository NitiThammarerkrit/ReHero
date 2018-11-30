#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"
class ClickableObject :public DrawableObject
{
	glm::vec3 color;
	string name;
	unsigned int texture;
	unsigned int temptexture;
	int rowMax;
	int columnMax;
	int row;
	int column;
	float uv[8];
	int ID;



public:
	ClickableObject();
	~ClickableObject();
	void setColor(float r, float g, float b);
	void render(glm::mat4 globalModelTransform);
	void setName(string n);
	void update(float deltaTime);
	string getName();
	bool isClick(float x, float y);

	void setRow(int r);
	void setColumn(int c);
	int getRow();
	int getColumn();
	void genUV();
	void setSpriteClickableObject(SpriteObject, int row, int col);
	void setId(int id);
	int getId();

	unsigned int getTexture();
	void setTexture(unsigned int Texture);
	bool active;
};

