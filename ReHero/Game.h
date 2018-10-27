#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRendererColor.h"

using namespace std;
class Game
{
	static Game* instance;
	int winWidth, winHeight;
	float gameWidth, gameHeight;
	vector<DrawableObject*> objects;
	GLRenderer *renderer;
	bool isMouseDown;
	class Card* clickObject;
	Game();
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

};