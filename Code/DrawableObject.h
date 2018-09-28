#pragma once
#include "glm\glm.hpp"
#include <vector>

using namespace std;

class DrawableObject
{
protected:
	glm::vec3 pos;
	glm::vec3 size;

public:
	glm::mat4 getTransform();

	DrawableObject();
	~DrawableObject();
	virtual void render(glm::mat4 globalModelTransform) = 0;
	void setSize(float sizeX, float sizeY);
	void setPosition(glm::vec3 newPosition);
	void translate(glm::vec3 moveDistance);

	//Rotate, Scale ???

};

