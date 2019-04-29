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
	if (isRunning&&delay<300)
	{
		delay += deltaTime * 1;
		size += deltaTime * 0.05;
		this->translate(glm::vec3(5.0f, 8.f, 0.0f));
		this->setSize(size*2, -size*5);
	}
	if (delay>=300&&delay<750)
	{
		delay += deltaTime * 1;
	}
	if (delay >= 750&&isRunning==true)
	{
		delay = 0;
		this->loadText(" ");
		//size = 40;
		isRunning = false;
	}
}

void FloatText::render(glm::mat4 globalModelTransform)
{
	TextObject::render(globalModelTransform);
}

void FloatText::changeSize(float n)
{
	size = n;
}
