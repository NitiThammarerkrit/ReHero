#include "stdafx.h"
#include "ClickableObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"
#include "SpriteObject.h"
#include<SDL_image.h>	


ClickableObject::ClickableObject()
{
	color = glm::vec3(0.0, 0.0, 0.0);
	active = true;
	ID = 0;
	int effectrow=1;
	int effectcolumn = 1;
	int normalrow = 1;
	int normalcolumn = 1;
}



ClickableObject::~ClickableObject()
{

}

void ClickableObject::setColor(float r, float g, float b)
{
	color = glm::vec3(r, g, b);
}

void ClickableObject::render(glm::mat4 globalModelTransform)
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

void ClickableObject::setTagAndName(string t, string n)
{
	tag = t;
	name = n;
}

string ClickableObject::getName()
{
	return name;
}

string ClickableObject::getTag()
{
	return tag;
}

bool ClickableObject::isClick(float x, float y)
{
	if (active == false)
	{
		return false;
	}
	/* ref
	glm::mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0));
	transform = glm::rotate(transform, -angle * 3.14f / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, glm::vec3(-pos.x, -pos.y, 0));
	*/

	float xCen = 0.0f;
	float yCen = -650.0f;

	float angleR = -angle * 3.14f / 180.0f;
	float newx = (cos(angleR) * (x - pos.x) - sin(angleR) * (y - pos.y)) + pos.x;
	float newy = (sin(angleR) * (x - pos.x) + cos(angleR) * (y - pos.y)) + pos.y;


	return (newy > pos.y - size.y / 2) && (newy < pos.y + size.y / 2)
		&& (newx > pos.x - size.x / 2) && (newx < pos.x + size.x / 2);
}

void ClickableObject::update(float deltaTime)
{

}

void ClickableObject::setRow(int r)
{
	this->row = r;
}

void ClickableObject::setColumn(int c)
{
	this->column = c;
}

int ClickableObject::getRow()
{
	return this->row;
}

int ClickableObject::getColumn()
{
	return this->column;
}

void ClickableObject::genUV()
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

void ClickableObject::setSpriteClickableObject(SpriteObject sprite, int row, int col)
{
	this->texture = sprite.getTexture();
	temptexture = texture;
	this->row = row;
	this->column = col;
	rowMax = sprite.getMaxRow();
	columnMax = sprite.getMaxColumn();
	this->normalrow = row;
	this->normalcolumn = col;

}

void ClickableObject::setId(int id)
{
	ID = id;
}

int ClickableObject::getId()
{
	return ID;
}

unsigned int ClickableObject::getTexture()
{
	return texture;
}

void ClickableObject::setTexture(unsigned int Texture)
{
	this->texture = Texture;
}

void ClickableObject::setEffect(SpriteObject sprite, int row, int col)
{
	EffectTexture = sprite.getTexture();
	this->effectrow = row;
	this->effectcolumn = col;
}

void ClickableObject::onClick(bool isClick)
{
	if (isClick==true)
	{
		this->setTexture(EffectTexture);
		this->setRow(effectrow);
		this->setColumn(effectcolumn);
		genUV();
	}
	if (isClick==false)
	{
		this->setTexture(temptexture);
		this->setRow(normalrow);
		this->setColumn(normalcolumn);
		genUV();
	}
}


