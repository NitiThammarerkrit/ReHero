#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRendererColor.h"
#include "Deck.h"
#include "Hero.h"
#include "Monster.h"

using namespace std;


enum class State
{
	PLAYER_CONDITION = 0,
	PLAYER_RANDOM_MANA = 1,
	PLAYER_DRAW = 2,
	PLAYER_PLAY = 3,
	PLAYER_ATTACK_ANIM = 4,
	PLAYER_HEAL_ANIM = 5,
	PLAYER_DEFENSE_ANIM = 6,
	PLAYER_SPELL_ANIM = 7,
	PLAYER_PAY_HP_ANIM = 8,
	PLAYER_END_TURN = 9,
	PLAYER_DIE = 20,
	
	ENEMY_CONDITION = 10,
	ENEMY_HEAL_ANIM = 11,
	ENEMY_DEFENSE_ANIM = 12,
	ENEMY_SPELL_ANIM = 13,
	ENEMY_PAY_HP_ANIM = 14,
	ENEMY_ATTACK_ANIM = 15,
	ENEMY_DIE = 21,

	GAME_MAINMENU = 16,
	GAME_QUIT = 17,
	GAME_CITY = 18,
	GAME_PAUSE = 19,
};

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
	State state;
	void restartGame();
	void setMonster(int HP, string name, int row, int column,int speed);
	SpriteObject * showMana;
	SpriteObject * randomMana;
	SpriteObject * randomManaText;
	SpriteObject * effectOnPlayer;
	SpriteObject * effectOnEnemy;

	DrawableObject * damageText;

	vector<DrawableObject*> Text;
	void drawText(string text, glm::vec3 pos, int fontSize);
	
	vector<DrawableObject*> tempText;
};