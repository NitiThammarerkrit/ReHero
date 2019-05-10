#include "stdafx.h"
#include "Game.h"
#include "SpriteObject.h"
#include "SquareMeshVbo.h"
#include<SDL_image.h>				 

SpriteObject::SpriteObject(string fileName, int row, int column)
{
	active = true;
	glActiveTexture(GL_TEXTURE0);
	SDL_Surface *image = IMG_Load(fileName.c_str());
	if (image == NULL)
	{
		cerr << "IMG_Load: " << SDL_GetError() << endl;
		return;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int Mode = GL_RGB;
	if (image->format->BytesPerPixel == 4)
	{
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(image);

	this->rowMax = row;
	this->columnMax = column;
	this->row =1;
	this->column = 1;
	this->genUV();

	this->animRow = 1;
	this->animColumn = 1;
	this->loopMax = 1;
	this->loopCount = 0;
	this->animationTime = 0;
	this->timeCount = 0;
	temptexture = texture;
	hasAnim = true;
	state = 0;
	c = 0;
}

SpriteObject::~SpriteObject()
{
}

void SpriteObject::render(glm::mat4 globalModelTransform)
{
	if (!active)
	{
		return;
	}
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = Game::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint modeId = Game::getInstance()->getRenderer()->getModeUniformId();

	glBindTexture(GL_TEXTURE_2D, texture);
	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}

	glm::mat4 currentMatrix = this->getTransform();

	if (squareMesh != nullptr) {

		currentMatrix = globalModelTransform * currentMatrix;
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform1i(modeId, 1);
		squareMesh->adjustTexCoord(uv);
		squareMesh->render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SpriteObject::update(float deltaTime)
{
	
	if (Game::getInstance()->state == State::PLAYER_SHOW_RANDOM_MANA && tag == "randomMana")
	{
		//cout << "random0" << endl;
		if (state == 0)
		{
			//cout << "random1" << endl;
			Game::getInstance()->randomMana->setActive(true);
			Game::getInstance()->randomMana->setPlayAnim(true);
			Game::getInstance()->randomManaText->setActive(true);
			c += deltaTime;
			if (c > 500)
			{
				c = 0;
				state = 1;
			}
		}
		else
			if (state == 1)
			{
				//cout << "random2" << endl;
				Game::getInstance()->randomMana->setPlayAnim(false);
				Game::getInstance()->randomMana->setColumn(Game::getInstance()->showMana->getColumn());
				Game::getInstance()->randomMana->genUV();
				c += deltaTime;
				if (c > 1000)
				{
					c = 0;
					state = 2;
				}
			}
			else
				if (state == 2)
				{
					//cout << "random3" << endl;
					state = 0;
					Game::getInstance()->randomMana->setActive(false);
					Game::getInstance()->randomManaText->setActive(false);
					Game::getInstance()->state = State::PLAYER_DRAW;
				}
	}

	if (hasAnim==false)
	{
		return;
	}
	timeCount += deltaTime;
	if (timeCount > animationTime / loopMax)
	{
		this->nextAnimation();
		this->genUV();
		timeCount = 0;
	}
	
}

void SpriteObject::setRow(int r)
{
	this->row = r;
}

void SpriteObject::setColumn(int c)
{
	this->column = c;
}

int SpriteObject::getRow()
{
	return this->row;
}

int SpriteObject::getColumn()
{
	return this->column;
}

int SpriteObject::getMaxRow()
{
	return rowMax;
}

int SpriteObject::getMaxColumn()
{
	return columnMax;
}

void SpriteObject::genUV()
{
	
	uv[0] = (1.0f / columnMax) * (column - 1) ;
	uv[1] = (1.0f / rowMax) * (row - 1);

	uv[2] = (1.0f / columnMax) * (column);
	uv[3] = (1.0f / rowMax) * (row - 1);

	uv[4] = (1.0f / columnMax) * (column);
	uv[5] = (1.0f / rowMax) * (row);

	uv[6] = (1.0f / columnMax) * (column - 1);
	uv[7] = (1.0f / rowMax) * (row);
}

void SpriteObject::setAnimationLoop(int row, int column, int loop, int time)
{
	this->row = row;
	this->column = column;
	this->animRow = row;
	this->animColumn = column;
	this->loopMax = loop;
	this->loopCount = 0;
	this->animationTime = time;
}

void SpriteObject::nextAnimation()
{
	loopCount++;
	if (loopCount < loopMax)
	{
		if (column < columnMax)
		{
			column++;
		}
		else
		{
			column = 1;
			row++;
		}
	}
	else
	{
		row = animRow;
		column = animColumn;
		loopCount = 0;
	}
	
}

unsigned int SpriteObject::getTexture()
{
	return texture;
}

void SpriteObject::setTexture(unsigned int newTexture)
{
	this->texture = newTexture;
	
}

void SpriteObject::setActive(bool active)
{
	this->active = active;
}

void SpriteObject::setPlayAnim(bool set)
{
	hasAnim = set;
}

void SpriteObject::setTag(string tag)
{
	this->tag = tag;
}

string SpriteObject::getTag()
{
	return tag;
}

void SpriteObject::changeSprite(string fileName, int row, int column)
{
	glActiveTexture(GL_TEXTURE0);
	SDL_Surface *image = IMG_Load(fileName.c_str());
	if (image == NULL)
	{
		cerr << "IMG_Load: " << SDL_GetError() << endl;
		return;
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int Mode = GL_RGB;
	if (image->format->BytesPerPixel == 4)
	{
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, image->w, image->h, 0, Mode, GL_UNSIGNED_BYTE, image->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	SDL_FreeSurface(image);

	this->rowMax = row;
	this->columnMax = column;
	this->row = 1;
	this->column = 1;
	this->genUV();

	this->animRow = 1;
	this->animColumn = 1;
	this->loopMax = 1;
	this->loopCount = 0;
	this->animationTime = 0;
	this->timeCount = 0;
	temptexture = texture;
	hasAnim = true;
	state = 0;
	c = 0;
}




