#pragma once

struct Vertex {
	float x, y, z;
	float u, v;

	Vertex(float _x, float _y, float _z, float _u, float _v) {
		x = _x;
		y = _y;
		z = _z;

		u = _u;
		v = _v;
	}

	Vertex() {
		x = y = z = 0.0f;
		u = v = 0.0f;
	}

	void operator *= (float _s) {
		x *= _s;
		y *= _s;
		z *= _s;
	}

	void operator += (float _v) {
		x += _v;
		y += _v;
		z += _v;
	}
};