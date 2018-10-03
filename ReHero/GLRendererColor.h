#pragma once

#include "GLRenderer.h"
class GLRendererColor : public GLRenderer
{
	virtual bool initialize(string vertexShaderFile, string fragmentShaderFile);

public:
	GLRendererColor(int w, int h);
	~GLRendererColor();
	GLuint getColorUniformId();
	GLuint getModeUniformId();
};

