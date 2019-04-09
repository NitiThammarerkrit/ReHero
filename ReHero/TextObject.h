#pragma once

#include "DrawableObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <SDL_ttf.h>

class TextObject :public DrawableObject
{
private:
	
	int fontSize;
	SDL_Color textColor;
	string fontName;
	int countUpdateFrame = 0;
public:
	unsigned int texture = -1;
	TextObject();
	~TextObject();
	void setTextColor(SDL_Color color);
	void setFontName(string font);
	void setFontSize(int size);
	int getFontSize();
	virtual void render(glm::mat4 globalModelTransform);
	void update(float deltaTime);
	virtual void loadText(string text, SDL_Color textColor, string fontname, int fontSize);
	virtual void loadText(string text);
	
};