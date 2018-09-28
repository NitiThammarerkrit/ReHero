#include "stdafx.h"
#include "MojiMeshVbo.h"

string const MojiMeshVbo::MESH_NAME = "Moji";

void MojiMeshVbo::loadData()
{
	//VBO data
	GLfloat vertexData[] =
	{
		
		0.0f, -1.0f,
		1.5f, -1.5f,
		1.0f, -0.5f,
		2.0f, 0.5f,
		0.5f, 0.5f,
		0.0f, 1.5f,
		-0.5f, 0.5f,
		-2.0f, 0.5f,
		-1.0f, -0.5f,
		-1.5f, -1.5f,
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
	glBufferData(GL_ARRAY_BUFFER, 2 * 10 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &(this->texVboId));
	glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), texData, GL_STATIC_DRAW);


}

MojiMeshVbo::MojiMeshVbo()
{
}

string MojiMeshVbo::getMeshName()
{
	return MESH_NAME;
}

void MojiMeshVbo::render()
{
	if (this->posAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->posVboId);
		glVertexAttribPointer(this->posAttribId, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
	}
	if (this->texAttribId != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, this->texVboId);
		glVertexAttribPointer(this->texAttribId, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	}
	glLineWidth(5.0f);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
	glDrawArrays(GL_LINE_LOOP, 0, 10);
}