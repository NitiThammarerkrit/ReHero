#pragma once

#include <GL\glew.h>
#include <string>

using namespace std;

class MeshVbo {
protected:
	GLuint posVboId = -1;
	GLuint texVboId = -1;
	GLuint posAttribId = -1;
	GLuint texAttribId = -1;

public:

	MeshVbo();
	virtual void loadData() = 0;
	virtual string getMeshName() = 0;
	GLuint getPosId();
	GLuint getTexId();
	virtual void render() = 0;
	void setAttribId(GLuint posId, GLuint texId);

};