#include "pch.h"
#include "MeshVbo.h"


MeshVbo::MeshVbo()
{
}

GLuint MeshVbo::getPosId()
{
	return this->posVboId;
}

GLuint MeshVbo::getTexId()
{
	return this->texVboId;
}

void MeshVbo::setAttribId(GLuint posId, GLuint texId)
{
	this->posAttribId = posId;
	this->texAttribId = texId;
}
