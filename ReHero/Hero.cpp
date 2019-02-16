#include <iostream>
#include "stdafx.h"
#include "Hero.h"
#include "Game.h"
#include "SquareMeshVbo.h"

#define POISON_DMG 2

using namespace std;

Hero::Hero(int HP, string fileName, int row, int column) : SpriteObject(fileName, row, column) {
	this->HP = HP;
	this->maxHP = HP;
	state = 0;
	c = 0;
	damage = 0;
	getAttack = false;
	isHeal = false;
	isAttack = false;
	isGetAttack = false;
	delayDie = 0;

}

Hero::~Hero() {

}

void Hero::update(float deltaTime)
{
	//float y = deltaTime * 1.0f;
	//Game::getInstance()->damageText->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	timeCount += deltaTime;
	if (timeCount > animationTime / loopMax)
	{
		this->nextAnimation();
		this->genUV();
		timeCount = 0;
	}

	if (getAttack&&isAlive()) //get attacked
	{
		setAttack(true);   		 
		HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		//cout << "\nMonster HP:" << this->getHP();
		//cout << endl << "damage is" << damage;
		HPBar->translate(glm::vec3((-damage / 2.0f / (float)this->getMaxHP()) * 250.0f, 0.0f, 0.0f));
		cout << "HP = "<<(float)this->getHP()<<endl;
		this->setAnimationLoop(3, 1, 5, 100);
		damage = 0;
		Game::getInstance()->DamageAmount = 0;
		getAttack = false;
	}
	if (isAlive()==false)
	{
		if(delayDie==0)
		this->setAnimationLoop(4, 1, 5, 600);
		delayDie += 1 * deltaTime;
		if (delayDie > 590)
		{
			this->setActive(false);
		}
	}
	if (isGetAttack&&isAlive())
	{
		delay += 1 * deltaTime;
		if (delay > 110)
		{
			delay = 0;
			this->setAnimationLoop(1, 1, 7, 1500);
			setAttack(false);
		}
	}
	if (Game::getInstance()->state == 1 && isHeal == false && isAlive())  //attack!
	{
		if (state == 0)
		{
			//this->translate(glm::vec3(550, 0, 0));
			this->setAnimationLoop(2, 1, 8, 700);
			state = 1;
		}
		else
		if (state == 1)
		{
			c += 1*deltaTime;
			if (c > 500)
			{
				state = 2;
				c = 0;
			}
		}
		else
		if (state == 2)
		{
			//this->translate(glm::vec3(-550, 0, 0));
			this->setAnimationLoop(1, 1, 7, 1500);
			//this->setTexture(temptexture);
			state = 0;
			isAttack = false;
			Game::getInstance()->state = 0;
		}
	}
	else
		if (Game::getInstance()->state == 1 && isHeal == true && isAlive()) //get Heal
		{
			Game::getInstance()->state = 0;
			isHeal = false;
		}

}

int Hero::getHP() {
	return this->HP;
}

int Hero::getMaxHP() {
	return this->maxHP;
}

int Hero::getArmor() {
	return this->defArmor;
}

void Hero::setHP(int amount) {
	this->HP = amount;
}

void Hero::setMaxHP(int amount) {
	this->maxHP = amount;
}

bool Hero::takeDamage(int damage) {
	this->damage = damage;
	Game::getInstance()->DamageAmount = damage;
	getAttack = true;
	if (this->HP - damage > 0)
	{
		//survive the damage
		this->HP -= damage;
		return true;
	}
	else
	{
		//die
		this->HP = 0;
		return false;
	}


}

bool Hero::isAlive() {
	if (HP > 0) return true;
	else return false;
}

void Hero::getHeal(int amount) {
	isHeal = true;
	if (this->HP + amount > this->maxHP)
	{
		this->HP = this->maxHP;
	}
	else
	{
		this->HP += amount;
	}
}

void Hero::gainArmor(int amount) {
	isHeal = true;
	defArmor += amount;
	cout << "gain " << amount << " armor" << endl;
}

void Hero::takePoison() {
	getAttack = true;
	isPoisoned = true;

}

void Hero::render(glm::mat4 globalModelTransform)
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
		if (isGetAttack)
		{
			glUniform1i(modeId, 2);
		}
		else
		{
			glUniform1i(modeId, 1);
		}
		squareMesh->adjustTexCoord(uv);
		squareMesh->render();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Hero::setAttack(bool A)
{
	isGetAttack = A;
}


void Hero::startTurn() {
	defArmor = 0;
	if (isPoisoned)
	{
		cout << "Hero take " << POISON_DMG << " damage from Poison." << endl;
		HP -= POISON_DMG;
	}
}

