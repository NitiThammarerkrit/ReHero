#pragma once

#include "MeshVbo.h"

class SquareMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	SquareMeshVbo();
	virtual string getMeshName();
	virtual void render();
	virtual void loadData();
};