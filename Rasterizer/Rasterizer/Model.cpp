#include "Model.h"

Model::Model(const char* _location, GeomUtils* _gUtil)
{
	if (verticies.size() % 3 != 0) {
		std::cout << "Warning. There may be non manifold geometry or rogue verticies in this model. Artifacts are to be expected" << std::endl;
	}
	numFaces = (int)(verticies.size() / 3);
}

Model::~Model()
{
}
