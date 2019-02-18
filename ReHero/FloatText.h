#pragma once
#include "TextObject.h"
#include "Game.h"

using namespace std;

class FloatText:public TextObject
{
	float delay;
	

public:
	
	FloatText();
	~FloatText();
	virtual void update(float deltaTime);
	virtual void render(glm::mat4 globalModelTransform);
	bool isRunning;
};


