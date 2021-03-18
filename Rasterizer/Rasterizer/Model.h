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

	void SetVertex(int _index, Vertex _vert) {
		verticies[_index].x = _vert.x;
		verticies[_index].y = _vert.y;
		verticies[_index].z = _vert.z;
		verticies[_index].u = _vert.u;
		verticies[_index].v = _vert.v;
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

	mat4 CreateModelviewMatrix(mat4* _viewMat) {
		//Create the model matrix first
		//Scale and translate transforms
		mat4 modelMatrix = mat4(
			vec4f(scale.x, 0.0f, 0.0f, position.x),
			vec4f(0.0f, scale.y, 0.0f, position.y),
			vec4f(0.0f, 0.0f, scale.z, position.z),
			vec4f(0.0f, 0.0f, 0.0f, 1.0f)
		);

		//Concatonate rotation into the matrix if needed
		//About X axis
		if (rotation.x == 0) {
			modelMatrix.MatrixMultiply(
				mat4(
					vec4f(1.0f, 0.0f, 0.0f, 0.0f),
					vec4f(0.0f, cosf(rotation.x * PI / 180.0f), -sinf(rotation.x * PI / 180.0f), 0.0f),
					vec4f(0.0f, sinf(rotation.x * PI / 180.0f), cosf(rotation.x * PI / 180.0f), 0.0f),
					vec4f(0.0f, 0.0f, 0.0f, 1.0f)
				)
			);
		}

		//About Y axis
		if (rotation.y == 0) {
			modelMatrix.MatrixMultiply(
				mat4(
					vec4f(cosf(rotation.y * PI / 180.0f), 0.0f, sinf(rotation.y * PI / 180.0f), 0.0f),
					vec4f(0.0f, 1.0f, 0.0f, 0.0f),
					vec4f(-sinf(rotation.y * PI / 180.0f), 0.0f, cosf(rotation.y * PI / 180.0f), 0.0f),
					vec4f(0.0f, 0.0f, 0.0f, 1.0f)
				)
			);
		}

		//About Z axis
		if (rotation.z == 0) {
			modelMatrix.MatrixMultiply(
				mat4(
					vec4f(cosf(rotation.z * PI / 180.0f), -sinf(rotation.z * PI / 180.0f), 0.0f, 0.0f),
					vec4f(sinf(rotation.z * PI / 180.0f), cosf(rotation.z * PI / 180.0f), 0.0f, 0.0f),
					vec4f(0.0f, 0.0f, 1.0f, 0.0f),
					vec4f(0.0f, 0.0f, 0.0f, 1.0f)
				)
			);
		}
		modelMatrix.PrintDebugInfo();
		std::cout << "Model" << std::endl;
		//By this point there should be a fully fleshed out model matrix
		//Now just concatonate with the view matrix and return it
		return modelMatrix.GetMatrixMultiply(*_viewMat);
	}

private:
	vec3f position, rotation, scale;

	GeomUtils* gUtil;
	std::vector<Vertex> verticies;
	int numFaces;
};