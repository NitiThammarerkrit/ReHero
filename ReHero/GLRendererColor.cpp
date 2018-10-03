#include "stdafx.h"
#include "GLRendererColor.h"


GLRendererColor::GLRendererColor(int w, int h) :GLRenderer(w, h)
{
}


bool GLRendererColor::initialize(string vertexShaderFile, string fragmentShaderFile)
{
	bool loadShader = GLRenderer::initialize(vertexShaderFile, fragmentShaderFile);
	if (!loadShader) {
		return false;
	}

	//Setting color uniform id

	colorUniformId = glGetUniformLocation(gProgramId, "color");
	if (colorUniformId == -1)
	{
		cout << "Color is not a valid glsl uniform variable" << endl;
		return false;
	}

	modeUniformId = glGetUniformLocation(gProgramId, "mode");
	if (modeUniformId == -1)
	{
		cout << "Mode is not a valid glsl uniform variable" << endl;
		return false;
	}

	return true;
}


GLRendererColor::~GLRendererColor()
{
}

GLuint GLRendererColor::getColorUniformId()
{
	return this->colorUniformId;
}

GLuint GLRendererColor::getModeUniformId()
{
	return this->modeUniformId;
}