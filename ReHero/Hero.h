#pragma once

//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"
#include "SpriteObject.h"
//#include "Monster.h"
//#include "Deck.h"
#include <queue>
//#include "TextObject.h"




class Hero : public SpriteObject
{
	int maxHP;
	int HP;
	int defArmor;
	bool isPoisoned;
	bool getAttack;
	bool isHeal;
	bool isAttack;
	int damage;
	bool isGetAttack;
	

public:
	Hero(int HP, string fileName, int row, int column);
	~Hero();

	virtual void update(float deltaTime);
	int getHP();
	int getMaxHP();
	int getArmor();
	bool getPoisoned();
	void setHP(int amount);
	void setMaxHP(int amount);
	bool takeDamage(int damage); //true if alive, false if die
	bool isAlive();
	void getHeal(int amount);
	void gainArmor(int amount);
	void takePoison();
	void curePoison();
	virtual void render(glm::mat4 globalModelTransform);
	void startTurn(); //resolve every special condition & remove armor from last turn
	/*void SetAnim(int animRow, int loopNum, int time);
	void CheckState();	*/
	int state;
	int c;
	int delay;
	int delayDie;
	bool oneTime;
	DrawableObject* HPBar;

	queue<string> effect;
	queue<int> heroMakeDamage;
	void endAction();
};

