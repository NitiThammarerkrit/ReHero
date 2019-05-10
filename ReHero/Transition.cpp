#include "stdafx.h"
#include "Transition.h"
#include <iostream>
#include "Game.h"


Transition::Transition(string name, int row, int column) : SpriteObject("Sprite/" + name + ".png", row, column)
{
	onetime = true;
}

Transition::~Transition()
{

}

void Transition::update(float deltaTime)
{
	if (delay >= 0)
	{
		this->translate(glm::vec3(60.0f, 0.0f, 0.0f));
		delay -= deltaTime * 1;
	}
	if (delay < 2300 && delay >= 0 && onetime == true)
	{
		Game::getInstance()->restartGame();
		Game::getInstance()->state = Game::getInstance()->nextState;
		onetime = false;
	}
	else
	if(delay<0)
	{
		this->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
		onetime = true;
	}
}




