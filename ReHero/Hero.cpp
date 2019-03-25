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
	delay = 0;
	oneTime = true;
}

Hero::~Hero() {

}

void Hero::update(float deltaTime)
{	  
	//cout << endl << cd << endl;
	//float y = deltaTime * 1.0f;
	//Game::getInstance()->damageText->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	timeCount += deltaTime;
	if (timeCount > animationTime / loopMax)
	{
		this->nextAnimation();
		this->genUV();
		timeCount = 0;
	}
	if (!effect.empty())
	{
		if (effect[0] == "damage")
		{
			if (oneTime == true)
			{
				this->setAnimationLoop(2, 1, 8, 700);
				if (!heroMakeDamage.empty())
				{
					Game::getInstance()->drawText(to_string(heroMakeDamage[0]), glm::vec3(350.0f, 0.f, 0.f), heroMakeDamage[0] * 40.0f, 3);
					heroMakeDamage.pop_back();
				}
				
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
		else if (effect[0] == "heal")
		{
			if (oneTime == true)
			{
				 this->setAnimationLoop(3, 1, 4, 400);
				 if (!heroMakeDamage.empty())
				 {
					 Game::getInstance()->drawText(to_string(heroMakeDamage[0]), glm::vec3(-350.0f, 0.f, 0.f), heroMakeDamage[0] * 40.0f, 2);
					 heroMakeDamage.pop_back();
				 }
				
				 oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 410)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
		else if (effect[0] == "poison")
		{
			if (oneTime == true)
			this->setAnimationLoop(2, 1, 8, 700);
			oneTime = false;
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
		else if (effect[0] == "defend")
		{
			if (oneTime == true)
			this->setAnimationLoop(3, 1, 4, 400);
			oneTime = false;
			delay += 1 * deltaTime;
			if (delay > 410)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
		else if (effect[0] == "draw")
		{
			if (oneTime == true)
				this->setAnimationLoop(3, 1, 4, 400);
			oneTime = false;
			delay += 1 * deltaTime;
			if (delay > 410)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
		else if (effect[0] == "dppc")
		{
			if (oneTime == true)
			this->setAnimationLoop(2, 1, 8, 1200);
			oneTime = false;
			delay += 1 * deltaTime;
			if (delay > 1210)
			{
				effect.pop_back();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}
	}
	if (getAttack&&isAlive()) //get attacked
	{ 		 
		HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		//cout << "\nMonster HP:" << this->getHP();
		//cout << endl << "damage is" << damage;
		HPBar->translate(glm::vec3((-damage / 2.0f / (float)this->getMaxHP()) * 250.0f, 0.0f, 0.0f));
		//cout << "HP = "<<(float)this->getHP()<<endl;
		if(oneTime == true)
		this->setAnimationLoop(3, 1, 4, 400);
		oneTime = false;
		damage = 0;
		delay += deltaTime;
		if (delay > 410)
		{
			delay = 0;
			this->setAnimationLoop(1, 1, 7, 1500);
			getAttack = false;
			oneTime = true;
		}	
	}
	else
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
	getAttack = true;

	int leftoverDMG = damage - this->defArmor;
	if (leftoverDMG <= 0)
	{
		this->defArmor -= damage;
		return true;
	}
	else
	{
		this->defArmor = 0;
	}

	if (this->HP - leftoverDMG > 0)
	{
		//survive the damage
		this->HP -= leftoverDMG;
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

void Hero::curePoison() {
	this->isPoisoned = false;
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
		if (getAttack)
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



void Hero::startTurn() {
	Game::getInstance()->state = State::PLAYER_CONDITION;
	defArmor = 0;
	if (isPoisoned)
	{
		cout << "Hero take " << POISON_DMG << " damage from Poison." << endl;
		this->takeDamage(POISON_DMG);

		/*this->damage = POISON_DMG;
		getAttack = true;	 */
	}
	if (isAlive()==false)
	{
		Game::getInstance()->state = State::PLAYER_DIE;
		//CheckState();
	}
	else
	{
		Game::getInstance()->state = State::PLAYER_PLAY;
	}
}

/*void Hero::SetAnim(int animRow, int loopNum, int time)
{
	while (state == 0)
	{
		cout << endl << "change anim" << endl;
		this->setAnimationLoop(animRow, 1, loopNum, time);
		state = 1;
	}
	while (state == 1)
	{
		c += 1;
		cout << endl << c << endl;
		if (c > (time + 10))
		{
			cout << endl << "change state" << endl;
			state = 2;
			c = 0;
		}
	}
	
	while (state == 2)
	{
		this->setAnimationLoop(1, 1, 7, 1500);
		state = 0;
		heroMakeDamage = 0;
		isAttack = false;
		Game::getInstance()->state = State::PLAYER_PLAY;
	}
}
void Hero::CheckState()
{
	if (Game::getInstance()->state == State::PLAYER_ATTACK_ANIM)
	{
		cout << endl << "attack" << endl;
		SetAnim(2, 8, 20000);
		//Game::getInstance()->drawText(to_string(heroMakeDamage), glm::vec3(-350.0f, 0.f, 0.f), heroMakeDamage*50.0f);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_HEAL_ANIM)
	{
		SetAnim(3, 4, 20000);
		//Game::getInstance()->drawText(to_string(heroMakeDamage), glm::vec3(350.0f, 0.f, 0.f), heroMakeDamage*50.0f);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_DEFENSE_ANIM)
	{
		SetAnim(3, 4, 20000);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_SPELL_ANIM)
	{
		SetAnim(2, 8, 20000);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_PAY_HP_ANIM)
	{
		SetAnim(3, 4, 20000);
	}
	else
	if (Game::getInstance()->state == State::PLAYER_DIE)
	{
		active = false;
	}
	else
	if (Game::getInstance()->state == State::ENEMY_ATTACK_ANIM)
	{
		cout << endl << "get hit" << endl;
		SetAnim(3, 4, 200);
	}
}	   */

