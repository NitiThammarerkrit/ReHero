#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRendererColor.h"
#include "Deck.h"
#include "Hero.h"
#include "Monster.h"





using namespace std;
class Game
{
	int cardIndex;
	Deck * deck;
	vector<string> PlayerDeck;
	static Game* instance;
	int winWidth, winHeight;
	float gameWidth, gameHeight;
	vector<DrawableObject*> objects;
	vector<DrawableObject*> Menu;
	vector<DrawableObject*> City;
	vector<DrawableObject*> Pause;
	Hero * myHero;
	vector<SpriteObject*> effect;
	vector<Monster*> enemies;
	vector<SpriteObject*> HPBG;
	vector<DrawableObject*> monsterHp;
	vector<DrawableObject*> heroHp;
	

	GLRenderer *renderer;
	bool isMouseDown;
	class Card* clickObject;
	class ClickableObject* clickableObject;
	Game();
	SpriteObject * previewCard;
	vector<ClickableObject*> clickable;
	//vector<ClickableObject*> Cityclickable;
	//vector<ClickableObject*> Menuclickable;
	//vector<ClickableObject*> Pauseclickable;

public:
	static Game* getInstance();
	GLRenderer * getRenderer();
	void handleMouseUp(int x, int y);
	void handleMouseDown(int x, int y);
	void handleMouseMotion(int x, int y);
	void handleKey(char ch);
	void init(int width, int height);
	void render();
	void update(float deltaTime);
	void resetHandPos();
	void endTurn();
	void monsterTurn();
	void getHit();
	int state;
	void restartGame();
	void setMonster(int HP, string name, int row, int column,int speed);
	SpriteObject * showMana;
	SpriteObject * randomMana;
	SpriteObject * randomManaText;
	SpriteObject * effectOnPlayer;
	SpriteObject * effectOnEnemy;

	int DamageAmount;
	bool isGetDamage = false;
	DrawableObject * damageText;

	vector<DrawableObject*> Text;
	void drawText(string text, glm::vec3 pos, int fontSize);

	vector<DrawableObject*> tempText;
};