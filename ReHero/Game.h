#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRendererColor.h"
#include "Deck.h"

using namespace std;
class Game
{
	int cardIndex;
	Deck * deck;
	static Game* instance;
	int winWidth, winHeight;
	float gameWidth, gameHeight;
	vector<DrawableObject*> objects;
	vector<DrawableObject*> hp;
	GLRenderer *renderer;
	bool isMouseDown;
	class Card* clickObject;
	Game();
	SpriteObject * previewCard;
	
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

};