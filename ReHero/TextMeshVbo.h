#pragma once

#include "MeshVbo.h"

class TextMeshVbo : public MeshVbo {
protected:

public:
	static string const MESH_NAME;
	TextMeshVbo();
	virtual string getMeshName();
	virtual void render();
	virtual void loadData();
};