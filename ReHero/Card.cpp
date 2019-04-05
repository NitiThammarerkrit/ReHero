#include "stdafx.h"
#include "Card.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include <SDL_image.h>	


Card::Card()
{
	color = glm::vec3(0.0, 0.0, 0.0);
	cardAngle = 0.0f;
	positionDrag = glm::vec3(0, 0, 0);
	isInHand = true;
	active = true;
	ID = 0;
	state = 0;
	c = 0;
}



Card::~Card()
{

}

void Card::setColor(float r, float g, float b)
{
	color = glm::vec3(r, g, b);
}

void Card::render(glm::mat4 globalModelTransform)
{
	if (!active)
	{
		return;
	}
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint colorId = Game::getInstance()->getRenderer()->getColorUniformId();
	GLuint modelMatixId = Game::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint modeId = Game::getInstance()->getRenderer()->getModeUniformId();

	glBindTexture(GL_TEXTURE_2D, texture);
	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}
	if (colorId == -1) {
		cout << "Error: Can't set color " << endl;
		return;
	}	  
	vector <glm::mat4> matrixStack;

	glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		if (isInHand)
		{
			currentMatrix = this->rotateAroundPoint(0, -650, cardAngle) *currentMatrix;
			//currentMatrix = glm::translate(currentMatrix, glm::vec3(0, -20, 0));
		}
		else
		{
			//this->setPosition(positionDrag);
		}
		if (positionDrag != glm::vec3(0, 0, 0))
		{
			//currentMatrix = glm::translate(currentMatrix, positionDrag);
			//this->setPosition(positionDrag);
		}
		
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x, color.y, color.z);
		glUniform1i(modeId, 1);
		squareMesh->adjustTexCoord(uv);
		squareMesh->render();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		/*currentMatrix = glm::translate(currentMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x-0.5f, color.y-0.5f, color.z-0.5f);
		squareMesh->render();*/

	}
}

void Card::setName(string n)
{
	this->name = n;

	ifstream datafile("cardEffectData.txt");
	if (!datafile)
	{
		cout << "fail to load card effect data for " << this->name << endl;
		return;
	}

	int howMany;
	int mana;
	string text;

	datafile >> howMany;
	getline(datafile, text, '\n');

	//get all skill data
	for (int i = 0; i < howMany; i++)
	{
		//getline(datafile, text, '\t');
		datafile >> text;
		if (text == this->name)
		{
			datafile >> mana;
			this->setMana(mana);
			getline(datafile, text, '\n');
			effectData = text;

			break;
		}
		else
		{
			getline(datafile, text, '\n');
		}
	}

	datafile.close();
}

string Card::getName()
{
	return name;
}

bool Card::isClick(float x, float y)
{
	/* ref
	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::rotate(transform, -angle * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-pos.x, -pos.y, 0));
	*/
	if (!active)
	{
		return false;
	}
	float xCen = 0.0f;
	float yCen = -650.0f;

	float angleR = -angle * 3.14f / 180.0f;
	float newx = (cos(angleR) * (x - pos.x) - sin(angleR) * (y - pos.y)) + pos.x;
	float newy = (sin(angleR) * (x - pos.x) + cos(angleR) * (y - pos.y)) + pos.y;

	float cardAngleR = -cardAngle * 3.14f / 180.0f;
	float tempX = newx;
	float tempY = newy;
	newx = (cos(cardAngleR) * (tempX - xCen) - sin(cardAngleR) * (tempY - yCen)) + xCen;
	newy = (sin(cardAngleR) * (tempX - xCen) + cos(cardAngleR) * (tempY - yCen)) + yCen;
	
	return (newy > pos.y - size.y / 2) && (newy < pos.y + size.y / 2)
		&& (newx > pos.x - size.x / 2) && (newx < pos.x + size.x / 2);
}

float Card::getCardAngle()
{
	return this->cardAngle;
}

void Card::setCardAngle(float angle)
{
	this->cardAngle = angle;
}
void Card::setPositionDrag(float x, float y)
{
	positionDrag = glm::vec3(x, y, 0);
}
void Card::setIsInHand(bool x)
{
	isInHand = x;
}
void Card::update(float deltaTime)
{
	if (Game::getInstance()->state == State::GAME_MAINMENU || Game::getInstance()->state == State::GAME_CITY|| Game::getInstance()->state == State::GAME_PAUSE)
	{
		active = false;
	}
	else
	{
		active = true;
	}
	if (state == 10)
	{
		//this-> translate(glm::vec3(0,50,0));
		texture = effectTexture;
		genUV();
	}
	else
	{
		texture = tempTexture;
		genUV();
	}

}

void Card::setRow(int r)
{
	this->row = r;
}

void Card::setColumn(int c)
{
	this->column = c;
}

int Card::getRow()
{
	return this->row;
}

int Card::getColumn()
{
	return this->column;
}

void Card::genUV()
{
	uv[0] = (1.0f / columnMax) * (column - 1);
	uv[1] = (1.0f / rowMax) * (row - 1);

	uv[2] = (1.0f / columnMax) * (column);
	uv[3] = (1.0f / rowMax) * (row - 1);

	uv[4] = (1.0f / columnMax) * (column);
	uv[5] = (1.0f / rowMax) * (row);

	uv[6] = (1.0f / columnMax) * (column - 1);
	uv[7] = (1.0f / rowMax) * (row);
}

void Card::setSpriteCard(SpriteObject * sprite, int row, int col)
{	   
	texture = sprite->getTexture();
	tempTexture = texture;
	rowMax = row;
	columnMax = col;
}

void Card::setId(int id)
{
	ID = id;
	/*for (int i = 0; i < 12; i++)
	{
		damage = 2 + i;
	}	  */

}

int Card::getId()
{
	return ID;
}

unsigned int Card::getTexture()
{
	return texture;
}

void Card::effect(Hero * friendTarget, Monster * enemyTarget)
{
	stringstream data(effectData);
	string effect;
	int value;

	while (1)
	{
		//read data (each action)
		/*if (Game::getInstance()->state != State::PLAYER_PLAY)
		{
			//friendTarget->CheckState();
			enemyTarget->CheckState();
			continue;
		}	  */

		data >> effect >> value;
		

		//perform a skill
		if (effect == "damage")
		{
			friendTarget->effect.push(effect);
			friendTarget->heroMakeDamage.push(value);
			//doDamage(enemyTarget, value);
		}
		else if (effect == "heal") 
		{ 
			friendTarget->effect.push(effect);
			friendTarget->heroMakeDamage.push(value);
			//heal(friendTarget, value);
		}
		else if (effect == "poison")
		{ 
			friendTarget->effect.push(effect);
			friendTarget->heroMakeDamage.push(value);
			//usePoison(enemyTarget);
		}
		else if (effect == "defend")
		{ 
			friendTarget->effect.push(effect);
			friendTarget->heroMakeDamage.push(value);
			//gainArmor(friendTarget, value);
		}
		else if (effect == "draw") 
		{
			friendTarget->effect.push(effect);
			friendTarget->heroMakeDamage.push(value);
			//drawCard(value);
		}
		else if (effect == "dppc")
		{
			Deck * d = Deck::getInstance();
			friendTarget->effect.push("damage");
			friendTarget->heroMakeDamage.push(value * d->getCardPlayedThisTurn());
			cout << "Played card count = " << d->getCardPlayedThisTurn() << endl;
			//doDamage(enemyTarget, value * d->getCardPlayedThisTurn());
		}
		else
		{
			break;
		}
			
	}
}

void Card::setMana(int mana)
{
	this->mana = mana;
}

int Card::getMana()
{
	return mana;
}

void Card::setEffectCard(SpriteObject * sprite, int row, int col)
{
	effectTexture = sprite->getTexture();
	rowMax = row;
	columnMax = col;
}

void Card::setState(int state)
{
	this->state = state;
}






