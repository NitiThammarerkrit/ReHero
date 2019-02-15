#include "stdafx.h"
#include "TextMeshVbo.h"

string const TextMeshVbo::MESH_NAME = "text";

void TextMeshVbo::loadData()
{
	//VBO data
	GLfloat vertexData[] =
	{	  
	  -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  0.5f,  0.5f, 0.0f,
	  -0.5f,  0.5f, 0.0f
	  /*
	  0.0f, 0.0f,
	  1.0f, 0.0f,
	  1.0f, 1.0f,
	  0.0f, 1.0f*/
	};

	GLfloat texData[] =
	{
	  0.0f, 0.0f,
	  1.0f, 0.0f,
	  1.0f, 1.0f,
	  0.0f, 1.0f
	};

	//Create VBO
	glGenBuffers(1, &(this->posVboId));
	glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &(this->texVboId));
	glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texData, GL_STATIC_DRAW);


}

TextMeshVbo::TextMeshVbo()
{
}

string TextMeshVbo::getMeshName()
{
	return MESH_NAME;
}

void TextMeshVbo::render()
{
	if (this->posAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
		glVertexAttribPointer(this->posAttribId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	}
	if (this->texAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
		glVertexAttribPointer(this->texAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	}
	glDrawArrays(GL_POLYGON, 0, 4);
}