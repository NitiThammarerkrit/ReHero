#pragma once

#include <vector>
#include "DrawableObject.h"
#include "GLRenderer.h"

using namespace std;
class Game
{
	static Game* instance;
	int winWidth, winHeight;
	vector<DrawableObject*> objects;
	GLRenderer *renderer;
	Game();
public:
	static Game* getInstance();
	GLRenderer * getRenderer();
	void handleMouse(int x, int y);
	void handleKey(char ch);
	void init(int width, int height);
	void render();

};