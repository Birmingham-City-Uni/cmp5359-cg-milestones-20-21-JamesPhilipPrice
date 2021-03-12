#pragma once
#include <iostream>
#include "Geometry.h"
#include "GeometryCore.h"

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

	//Accessors
	vec3f GetPosition() {
		return position;
	}

	vec3f GetRotation() {
		return rotation;
	}

	vec3f GetScale() {
		return scale;
	}

	//Mutators
	void SetPosition(vec3f _vec) {
		position = _vec;
	}

	void SetRotation(vec3f _vec) {
		rotation = _vec;
	}

	void SetScale(vec3f _vec) {
		scale = _vec;
	}

private:
	vec3f position, rotation, scale;

	GeomUtils* gUtil;
	std::vector<Vertex> verticies;
	int numFaces;
};