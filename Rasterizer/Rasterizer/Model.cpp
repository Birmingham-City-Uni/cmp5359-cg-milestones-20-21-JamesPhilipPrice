#include "Model.h"

Model::Model(const char* _location, GeomUtils* _gUtil)
{
	gUtil = _gUtil;
	gUtil->LoadOBJFile(_location, &verticies);

	if (verticies.size() > 0) {
		if (verticies.size() % 3 != 0) {
			std::cout << "Warning. There may be non manifold geometry or rogue verticies in this model. Artifacts are to be expected" << std::endl;
		}
		numFaces = (int)(verticies.size() / 3);
	}
	else {
		std::cout << "Warning there were 0 verticies loaded!" << std::endl;
	}
	
}

Model::~Model()
{
}
