#pragma once
#include <iostream>
#include "Geometry.h"

class Model {
public:
	Model(const char* _location, GeomUtils* _gUtil);
	~Model();

private:
	
	std::vector<Vertex> verticies;
	int numFaces;
};