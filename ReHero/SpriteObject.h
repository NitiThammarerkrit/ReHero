#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class SpriteObject :public DrawableObject
{
private:
	unsigned int texture;
	int rowMax;
	int columnMax;
	int row;
	int column;
	float uv[8];
	int animRow;
	int animColumn;
	int loopMax;
	int loopCount;
	int animationTime;
	int timeCount;
	bool active;

public:
	SpriteObject(string fileName, int row, int column);
	~SpriteObject();
	void render(glm::mat4 globalModelTransform);
	void update(float deltaTime);
	void setRow(int r);
	void setColumn(int c);
	int getRow();
	int getColumn();
	void genUV();
	void setAnimationLoop(int row, int column, int loop, int time); // 1000 = 1 second
	void nextAnimation();
	unsigned int getTexture();
	void setTexture(unsigned int newTexture);
	void setActive(bool active);
};

