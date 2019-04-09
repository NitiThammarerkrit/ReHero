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
	if (isRunning&&delay<1000)
	{
		delay += deltaTime * 1;
		size += deltaTime * 0.1;
		this->translate(glm::vec3(0.0f, 4.f, 0.0f));
		this->setSize(size, -size*2);
	}
	if (delay>=1000&&delay<1500)
	{
		delay += deltaTime * 1;
	}
	if (delay >= 1500&&isRunning==true)
	{
		delay = 0;
		this->loadText(" ");
		size = 30;
		isRunning = false;
	}
}

void FloatText::render(glm::mat4 globalModelTransform)
{
	TextObject::render(globalModelTransform);
}
