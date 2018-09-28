#include "stdafx.h"
#include "GameObject.h"
#include "Game.h"
#include "SquareMeshVbo.h"


Card::Card()
{
	color = glm::vec3(0.0, 0.0, 0.0);
	cardAngle = 0.0f;
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
	SquareMeshVbo *squareMesh = dynamic_cast<SquareMeshVbo *> (Game::getInstance()->getRenderer()->getMesh(SquareMeshVbo::MESH_NAME));

	GLuint modelMatixId = Game::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint colorId = Game::getInstance()->getRenderer()->getColorUniformId();


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
		if (cardAngle != 0)
		{
			currentMatrix = this->rotateAroundPoint(0, -650, cardAngle) *currentMatrix;
		}
		
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x, color.y, color.z);
		squareMesh->render();

		/*currentMatrix = glm::translate(currentMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
		glUniform3f(colorId, color.x-0.5f, color.y-0.5f, color.z-0.5f);
		squareMesh->render();*/

	}
}

void Card::setName(string n)
{
	name = n;
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

