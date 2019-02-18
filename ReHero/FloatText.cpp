#include "stdafx.h"
#include "FloatText.h"
#include <iostream>


FloatText::FloatText() : TextObject()
{
	delay = 0;
	isRunning = false;
}

FloatText::~FloatText()
{

}

void FloatText::update(float deltaTime)
{
	if (isRunning)
	{
		delay += deltaTime * 1;
		this->translate(glm::vec3(0.0f, 10.0f, 0.0f));
	}
	if (delay > 1000&&isRunning==true)
	{
		delay = 0;
		this->loadText(" ");
		isRunning = false;
	}
}

void FloatText::render(glm::mat4 globalModelTransform)
{
	TextObject::render(globalModelTransform);
}
