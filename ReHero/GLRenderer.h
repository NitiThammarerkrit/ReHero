#pragma once

#include <iostream>
#include <GL\glew.h>
#include "Shader.h"
#include <map>
#include <vector>
#include <string>
#include "MeshVbo.h"
#include "glm\glm.hpp"
#include "DrawableObject.h"

using namespace std;

class GLRenderer
{
protected:
	int winWidth;
	int winHeight;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	GLuint matrixId = -1;
	GLuint pMatrixId = -1;
	GLuint colorUniformId = -1;

	GLuint gProgramId;
	int gPos2DLocation;
	void printProgramLog(GLuint program);
	virtual bool initialize(string vertexShaderFile, string fragmentShaderFile);
	Shader *vertexShader;
	Shader *fragmentShader;
	map <string, MeshVbo*> shapes;
	virtual void setMeshAttribId(MeshVbo * shape);

public:
	virtual void render(vector <DrawableObject*> & objList);
	GLRenderer(int w, int h);
	bool initGL(string vertexShaderFile, string fragmentShaderFile);

	void addMesh(string name, MeshVbo* shape);
	MeshVbo * getMesh(string name);

	~GLRenderer();

	void setOrthoProjection(float left, float right, float bottom, float top);
	void setViewPort(int x, int y, int w, int h);
	void setClearColor(float r, float g, float b);

	GLuint getModelMatrixAttrId();
	GLuint getProjectionMatrixAttrId();
	virtual GLuint getColorUniformId() = 0;

};
