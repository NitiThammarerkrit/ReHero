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
	oneTime2 = true;
	poison = 0;
}

Hero::~Hero() {

}

void Hero::update(float deltaTime)
{	  
	if (defArmor > 0)
	{
		Game::getInstance()->drawEffectText(to_string(defArmor), 0, 3, true);
	}
	else
		Game::getInstance()->drawEffectText(to_string(defArmor), 0, 3, false);
	if (poison > 0)
	{
		Game::getInstance()->drawEffectText(to_string(poison), 1, 3, true);
	}
	else
		Game::getInstance()->drawEffectText(to_string(poison), 1, 3, false);

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
		if (effect.front() == "damage")
		{
			if (oneTime == true)
			{
				Game::getInstance()->state = State::PLAYER_ATTACK_ANIM;
				this->setAnimationLoop(2, 1, 8, 700);
				if (!heroMakeDamage.empty())
				{
					Game::getInstance()->doDamage(heroMakeDamage.front());
					Game::getInstance()->effectOnEnemy->setAnimationLoop(10, 1, 7, 700);
					Game::getInstance()->effectOnEnemy->setActive(true);
					if (heroMakeDamage.front() > 0) Game::getInstance()->drawText(to_string(abs(heroMakeDamage.front())), glm::vec3(350.0f, 0.f, 0.f), abs((heroMakeDamage.front() * 2) + 15.f), 2);
					heroMakeDamage.pop();
				}
				
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				effect.pop();
				Game::getInstance()->effectOnEnemy->setActive(false);
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		else if (effect.front() == "heal")
		{
			if (oneTime == true)
			{
				Game::getInstance()->state = State::PLAYER_HEAL_ANIM;
				 this->setAnimationLoop(5, 1, 4, 600);
				 if (!heroMakeDamage.empty())
				 {
					 Game::getInstance()->heal(heroMakeDamage.front());
					 HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
					 HPBar->setPosition(glm::vec3((float)-350.0f - 
						 ((float)this->getMaxHP() - (float)this->getHP()) 
						 / ((float)this->getMaxHP() / (float)(250.0f / 2.0f))
						 , 200.0f, 0.0f));
					 if (heroMakeDamage.front() > 0)
					 {
						 Game::getInstance()->effectOnPlayer->setAnimationLoop(3, 1, 5, 600);
						 Game::getInstance()->drawText(to_string(abs(heroMakeDamage.front())), glm::vec3(-350.0f, 0.f, 0.f), abs((heroMakeDamage.front() * 2) + 15.f), 1);
					 }
					 else if (heroMakeDamage.front() < 0)
						 {
							 Game::getInstance()->effectOnPlayer->setAnimationLoop(7, 1, 7, 600);
							 Game::getInstance()->drawText(to_string(abs(heroMakeDamage.front())), glm::vec3(-350.0f, 0.f, 0.f), abs((heroMakeDamage.front() * 2) + 15.f), 2);
						 }
					 Game::getInstance()->effectOnPlayer->setActive(true);
					 heroMakeDamage.pop();
				 }
				
				 oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 610)
			{
				effect.pop();
				Game::getInstance()->effectOnPlayer->setActive(false);
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		else if (effect.front() == "poison")
		{
			if (oneTime == true)
			{
				Game::getInstance()->effectOnEnemy->setAnimationLoop(11, 1, 7, 600);
				Game::getInstance()->effectOnEnemy->setActive(true);
				Game::getInstance()->state = State::PLAYER_SPELL_ANIM;
				Game::getInstance()->usePoison(heroMakeDamage.front());
				heroMakeDamage.pop();
				this->setAnimationLoop(5, 1, 4, 600);
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 610)
			{
				Game::getInstance()->effectOnEnemy->setActive(false);
				effect.pop();
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		else if (effect.front() == "defend")
		{
			if (oneTime == true)
			{
				this->setAnimationLoop(3, 1, 5, 700);
				Game::getInstance()->state = State::PLAYER_DEFENSE_ANIM;
				if (!heroMakeDamage.empty())
				{
					Game::getInstance()->effectOnPlayer->setAnimationLoop(9, 1, 9, 700);
					Game::getInstance()->effectOnPlayer->setActive(true);
					Game::getInstance()->gainArmor(heroMakeDamage.front());
					heroMakeDamage.pop();
				}
				oneTime = false;		
			}
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				Game::getInstance()->effectOnPlayer->setActive(false);
				effect.pop();
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		else if (effect.front() == "draw")
		{
			if (oneTime == true)
			{
				//Game::getInstance()->state = State::PLAYER_DRAW;
				Game::getInstance()->drawCard(heroMakeDamage.front());
				heroMakeDamage.pop();
				this->setAnimationLoop(5, 1, 4, 400);
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 410)
			{
				effect.pop();
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		/*else if (effect.front() == "dppc")
		{
			if (oneTime == true)
			{
				Game::getInstance()->state = State::PLAYER_ATTACK_ANIM;
				this->setAnimationLoop(2, 1, 8, 700);
				Game::getInstance()->doDamage();
				Game::getInstance()->drawText(to_string(abs(heroMakeDamage.front())), glm::vec3(350.0f, 0.f, 0.f), abs(heroMakeDamage.front()) * 40.0f, 2);

				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 710)
			{
				effect.pop();
				delay = 0;
				oneTime = true;
				if (effect.empty()) endAction();
			}
		}
		/*else if (effect.front() == "dppc")
		{
			if (oneTime == true)
			{
				Game::getInstance()->state = State::PLAYER_ATTACK_ANIM;
				this->setAnimationLoop(2, 1, 8, 1200);
				oneTime = false;
			}
			delay += 1 * deltaTime;
			if (delay > 1210)
			{
				effect.pop();
				delay = 0;
				this->setAnimationLoop(1, 1, 7, 1500);
				oneTime = true;
			}
		}*/
	}
	if (getAttack&&isAlive() || isHeal == true &&isAlive()) //get attacked
	{ 		 
		HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		HPBar->setPosition(glm::vec3((float)-350.0f -
			((float)this->getMaxHP() - (float)this->getHP())
			/ ((float)this->getMaxHP() / (float)(250.0f / 2.0f))
			, 200.0f, 0.0f));
		//HPBar->setSize(((float)this->getHP() / (float)this->getMaxHP()) * 250.0f, 20);
		////cout << "\nMonster HP:" << this->getHP();
		////cout << endl << "damage is" << damage;
		//HPBar->translate(glm::vec3((-damage / 2.0f / (float)this->getMaxHP()) * 250.0f, 0.0f, 0.0f));
		////cout << "HP = "<<(float)this->getHP()<<endl;
		if(oneTime2 == true&& getAttack)
		this->setAnimationLoop(3, 1, 5, 400);
		else
		if (oneTime2 == true && isHeal == true)
		this->setAnimationLoop(5, 1, 4, 400);
		oneTime2 = false;
		damage = 0;
		delay += deltaTime;
		if (delay > 410)
		{
			delay = 0;
			this->setAnimationLoop(1, 1, 7, 1500);
			if(getAttack==true)
			getAttack = false;
			if(isHeal==true)
			isHeal = false;
			oneTime2 = true;
		}	
	}
	else
	if (isAlive()==false)
	{
		if (delayDie == 0)
		{
			this->setAnimationLoop(6, 1, 5, 600);
			oneTime = false;
		}
		delayDie += 1 * deltaTime;
		if (delayDie > 590)
		{
			lose->setActive(true);
			//this->setActive(false);
			this->setAnimationLoop(6, 5, 1, 10000000);
			Game::getInstance()->ChangeState(1);   
			HP = 20;
			HPBar->setPosition(glm::vec3(-350.0f, 200.0f, 0.0f));
			delayDie = 0;
		}
	} 
	if (Game::getInstance()->state == State::TRANSITION)
	{
		if (oneTime == true)
		{
			this->setAnimationLoop(4, 1, 7, 600);
			HPBar->setActive(false);
			oneTime = false;
		}
		this->translate(glm::vec3(10.0f, 0.0f, 0.0f));
		delay += 1 * deltaTime;
		if (delay > 2500)
		{
			Game::getInstance()->TransitionPic->setPosition(glm::vec3(-2180.0f, 0.0f, 0.0f));
			Game::getInstance()->TransitionPic->onetime = true;
			Game::getInstance()->TransitionPic->delay = 3000;
			Game::getInstance()->nextState = State::GAME_MAP;
			//this->setPosition(glm::vec3(-350.0f, 80.0f, 0.0f));
			oneTime = true;
			delay = 0;
		}
	}
	if (Game::getInstance()->state == State::GAME_MAP)
	{
		HPBar->setActive(true);
		lose->setActive(false);
		this->setPosition(glm::vec3(-350.0f, 80.0f, 0.0f));
	}
}

void Hero::endAction() {
	this->setAnimationLoop(1, 1, 7, 1500);
	Game::getInstance()->state = State::PLAYER_PLAY;
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

int Hero::getPoison() {
	return this->poison;
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
}

void Hero::takePoison(int amount) {
	getAttack = true;
	poison += amount;
	//Game::getInstance()->drawText(to_string(poison), glm::vec3(-350.0f, 0.f, 0.f), abs((poison * 2) + 15.f), 3);
}

void Hero::curePoison() {
	this->poison = 0;
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
	if (poison > 0)
	{
		cout << "Hero take " << poison << " damage from Poison." << endl;
		this->takeDamage(poison);
		poison -= 1;
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

