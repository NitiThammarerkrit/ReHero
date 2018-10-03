#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Card :public DrawableObject
{
	glm::vec3 color;
	string name;
	float cardAngle;

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
};

