#include "stdafx.h"
#include "TextObject.h"
#include "Game.h"
#include "TextMeshVbo.h"

TextObject::TextObject()
{
	middleText = false;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

TextObject::TextObject(bool mode)
{
	middleText = mode;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


TextObject::~TextObject()
{

}

void TextObject::render(glm::mat4 globalModelTransform)
{
	
	GLuint modelMatixId = Game::getInstance()->getRenderer()->getModelMatrixAttrId();
	GLuint modeId = Game::getInstance()->getRenderer()->getModeUniformId();

	glBindTexture(GL_TEXTURE_2D, texture);
	//glBindTexture(GL_TEXTURE_2D, texNum);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	//glBindTexture(GL_TEXTURE_2D, texture);
	//cout << "Render\n";


	if (modelMatixId == -1) {
		cout << "Error: Can't perform transformation " << endl;
		return;
	}

	glm::mat4 currentMatrix = this->getTransform();


		TextMeshVbo *textMesh = dynamic_cast<TextMeshVbo *> (Game::getInstance()->getRenderer()->getMesh(TextMeshVbo::MESH_NAME));
		if (textMesh != nullptr) {

			currentMatrix = globalModelTransform * currentMatrix;
			glUniformMatrix4fv(modelMatixId, 1, GL_FALSE, glm::value_ptr(currentMatrix));
			glUniform1i(modeId, 1);
			//squareMesh->resetTexcoord();
			textMesh->render();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	
}

void TextObject::update(float deltaTime)
{
}

void TextObject::loadText(string text, SDL_Color textColor, string fontname, int fontSize)
{
	glBindTexture( GL_TEXTURE_2D, texture);
	
	const char* fontchar = fontname.c_str();
	TTF_Font * font = TTF_OpenFont(fontchar, fontSize);
	if (font)
	{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, text.c_str() , textColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMessage->w, surfaceMessage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surfaceMessage->pixels);
		this->setSize(surfaceMessage->w, -surfaceMessage->h);
		SDL_FreeSurface(surfaceMessage);
		TTF_CloseFont(font);
	}
	else
	{
		cout << "Error: " << TTF_GetError() << endl;
		return;
	}
	
}

void TextObject::loadText(string text)
{
	loadText(text, textColor, fontName, fontSize);
}

void TextObject::setFullText(string text)
{
	fullText = text;
}

void TextObject::setTextColor(SDL_Color color)
{
	textColor = color;
}

void TextObject::setFontName(string font)
{
	fontName = font;
}

void TextObject::setFontSize(int size)
{
	fontSize = size;
}

string TextObject::GetFullText()
{
	return fullText;
}

void TextObject::SetIndex(int ind)
{
	index = ind;
}

bool TextObject::GetComplete()
{
	return complete;
}

void TextObject::SetComplete(bool comp)
{
	complete = comp;
}