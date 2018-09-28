#pragma once

#include "MeshVbo.h"

class MojiMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	MojiMeshVbo();
	virtual string getMeshName();
	virtual void render();
	virtual void loadData();
};