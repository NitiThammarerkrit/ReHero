#include "stdafx.h"
#include "GLRenderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include "DrawableObject.h"

using namespace std;

GLRenderer::GLRenderer(int w, int h)
{
	this->winWidth = w;
	this->winHeight = h;
	projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f);
	glViewport(0, 0, winWidth, winHeight);
}

bool GLRenderer::initGL(string vertexShaderFile, string fragmentShaderFile)
{
	// Initialize glew
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
		return false;
	}


	//Initialize OpenGL
	if (!initialize(vertexShaderFile, fragmentShaderFile))
	{
		cout << "Unable to initialize OpenGL! " << endl;
		return false;
	}
	return true;
}

bool GLRenderer::initialize(string vertexShaderFile, string fragmentShaderFile)
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramId = glCreateProgram();
	vertexShader = new Shader(vertexShaderFile, GL_VERTEX_SHADER);
	if (!vertexShader->loadSource()) {
		return false;
	}
	fragmentShader = new Shader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	if (!fragmentShader->loadSource()) {
		return false;
	}
	glAttachShader(gProgramId, vertexShader->getShaderId());
	glAttachShader(gProgramId, fragmentShader->getShaderId());


	//Link program
	glLinkProgram(gProgramId);

	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(gProgramId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		cout << "Error linking program " << gProgramId << endl;
		printProgramLog(gProgramId);
		return false;
	}

	//Get vertex attribute location
	gPos2DLocation = glGetAttribLocation(gProgramId, "pos2D");

	if (gPos2DLocation == -1)
	{
		cout << "pos2D is not a valid glsl program variable" << endl;
		return false;
	}

	gTexCoordLocation = glGetAttribLocation(gProgramId, "texCoord");

	if (gTexCoordLocation == -1)
	{
		cout << "texCoord is not a valid glsl program variable" << endl;
		return false;
	}

	//Set up uniform id attribute
	pMatrixId = glGetUniformLocation(gProgramId, "pMatrix");
	matrixId = glGetUniformLocation(gProgramId, "mMatrix");


	glEnableVertexAttribArray(gPos2DLocation);
	glEnableVertexAttribArray(gTexCoordLocation);

	//Initialize clear color
	glClearColor(1.0f, 1.0f, 1.0f, 1.f);

	return true;

}

void GLRenderer::render(vector <DrawableObject*> & objList)
{
	// Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update window with OpenGL rendering

	glUseProgram(gProgramId);
	//Set up matrix uniform

	if (pMatrixId != -1) {
		glUniformMatrix4fv(pMatrixId, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
	}

	glm::mat4 camera = glm::mat4(1.0);

	for (DrawableObject *obj : objList) {
		obj->render(camera);
	}

	//Unbind program
	glUseProgram(NULL);
}

void GLRenderer::setMeshAttribId(MeshVbo * shape)
{
	shape->setAttribId(gPos2DLocation, gTexCoordLocation);
}

void GLRenderer::addMesh(string name, MeshVbo * shape)
{
	setMeshAttribId(shape);
	shapes[name] = shape;
}

MeshVbo * GLRenderer::getMesh(string name)
{
	if (shapes.find(name) == shapes.end()) {
		return nullptr;
	}
	else {
		return shapes[name];
	}
}

void GLRenderer::printProgramLog(GLuint program)
{
	//Make sure name is shader
	if (glIsProgram(program))
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		//Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0)
		{
			//Print Log
			cout << infoLog << endl;

		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		printf("Name %d is not a program\n", program);
	}
}

GLRenderer::~GLRenderer()
{
	if (gPos2DLocation != -1) {
		glDisableVertexAttribArray(gPos2DLocation);
	}
}

void GLRenderer::setOrthoProjection(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top);
}

void GLRenderer::setViewPort(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
}

void GLRenderer::setClearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0);
}

GLuint GLRenderer::getModelMatrixAttrId()
{
	return this->matrixId;
}

GLuint GLRenderer::getProjectionMatrixAttrId()
{
	return this->pMatrixId;
}
