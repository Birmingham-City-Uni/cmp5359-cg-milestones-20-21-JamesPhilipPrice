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
	
	Vertex operator + (Vertex _v) {
		return Vertex(
			x + _v.x,
			y + _v.y,
			z + _v.z,
			u, v
		);
	}

	Vertex operator - (Vertex _v) {
		return Vertex(
			x - _v.x,
			y - _v.y,
			z - _v.z,
			u, v
		);
	}

	Vertex operator * (float _v) {
		return Vertex(
			x * _v,
			y * _v,
			z * _v,
			u,
			v
		);
	}
};