#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class SpriteObject :public DrawableObject
{
private:
	
	
	int rowMax;
	int columnMax;
	int row;
	int column;
	int animRow;
	int animColumn;
	bool hasAnim;
	string tag;
	

protected:
	bool active;
	int loopMax;
	int loopCount;
	int animationTime;
	int timeCount;
	unsigned int temptexture;
	float uv[8];

public:
	SpriteObject(string fileName, int row, int column);
	~SpriteObject();
	virtual void render(glm::mat4 globalModelTransform);
	void update(float deltaTime);
	void setRow(int r);
	void setColumn(int c);
	int getRow();
	int getColumn();
	int getMaxRow();
	int getMaxColumn();
	void genUV();
	void setAnimationLoop(int row, int column, int loop, int time); // 1000 = 1 second
	void nextAnimation();
	unsigned int getTexture();
	void setTexture(unsigned int newTexture);
	void setActive(bool active);
	void setPlayAnim(bool set);
	void setTag(string tag);
	string getTag();
	int state;
	int c; 
	void changeSprite(string fileName, int row, int column);
	unsigned int texture;
	float delay;
	bool onetime;
	float GetXPos()
	{
		return pos.x;
	}

};

