#pragma once
#include <iostream>
#include "Geometry.h"

class Model {
public:
	Model(const char* _location, GeomUtils* _gUtil);
	~Model();

	Vertex GetVertex(int _index) {
		return verticies[_index];
	}

	unsigned int GetFaceCount() {
		return numFaces;
	}

private:
	GeomUtils* gUtil;
	std::vector<Vertex> verticies;
	int numFaces;
};