#pragma once
#include "SpriteObject.h"

using namespace std;

class Transition :public SpriteObject
{
	

public:

	Transition(string name, int row, int column);
	~Transition();
	virtual void update(float deltaTime);
};



