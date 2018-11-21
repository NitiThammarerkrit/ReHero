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
	static Game* instance;
	int winWidth, winHeight;
	float gameWidth, gameHeight;
	vector<DrawableObject*> objects;
	Hero * myHero;
	vector<Monster*> enemies;
	vector<DrawableObject*> monsterHp;
	vector<DrawableObject*> heroHp;
	GLRenderer *renderer;
	bool isMouseDown;
	class Card* clickObject;
	class ClickableObject* clickableObject;
	Game();
	SpriteObject * previewCard;
	vector<DrawableObject*> clickable;
	

	
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
	SpriteObject * showMana;
	SpriteObject * randomMana;
};