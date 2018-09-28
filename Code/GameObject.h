#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class GameObject :public DrawableObject
{
	glm::vec3 color;

public:
	GameObject();
	~GameObject();
	void setColor(float r, float g, float b);
	void render(glm::mat4 globalModelTransform);
};

