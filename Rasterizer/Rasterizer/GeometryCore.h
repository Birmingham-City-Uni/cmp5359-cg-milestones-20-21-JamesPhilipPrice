#pragma once
#define PI 3.1415

struct vec2f {
	float x, y;
	vec2f() {
		x = y = 0.0f;
	}

	vec2f(float _x, float _y) {
		x = _x;
		y = _y;
	}

	//Operators
	//Direct modifiers
	//Vector input
	void operator +=(vec2f _v) {
		x += _v.x;
		y += _v.y;
	}

	void operator -=(vec2f _v) {
		x -= _v.x;
		y -= _v.y;
	}

	void operator /=(vec2f _v) {
		x /= _v.x;
		y /= _v.y;
	}

	void operator *=(vec2f _v) {
		x *= _v.x;
		y *= _v.y;
	}

	//Non vector input
	void operator /=(float _v) {
		x /= _v;
		y /= _v;
	}

	void operator *=(float _v) {
		x *= _v;
		y *= _v;
	}

	//Indirect modifiers
	//Vector input
	vec2f operator +(vec2f _v) {
		return vec2f(x + _v.x, y + _v.y);
	}

	vec2f operator -(vec2f _v) {
		return vec2f(x - _v.x, y - _v.y);
	}

	vec2f operator /(vec2f _v) {
		return vec2f(x / _v.x, y / _v.y);
	}

	vec2f operator *(vec2f _v) {
		return vec2f(x * _v.x, y * _v.y);
	}

	//Non vector input
	vec2f operator /(float _v) {
		return vec2f(x / _v, y / _v);
	}

	vec2f operator *(float _v) {
		return vec2f(x * _v, y * _v);
	}

	float Dot(vec2f* _v) {
		return (x * _v->x + y * _v->y);
	}
};

struct vec3f {
	float x, y, z;
	vec3f() {
		x = y = z = 0.0f;
	}

	vec3f(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	//Operators
	//Direct modifiers
	//Vector input
	void operator +=(vec3f _v) {
		x += _v.x;
		y += _v.y;
		z += _v.z;
	}

	void operator -=(vec3f _v) {
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
	}

	void operator /=(vec3f _v) {
		x /= _v.x;
		y /= _v.y;
		z /= _v.z;
	}

	void operator *=(vec3f _v) {
		x *= _v.x;
		y *= _v.y;
		z *= _v.z;
	}

	//Non vector input
	void operator /=(float _v) {
		x /= _v;
		y /= _v;
		z /= _v;
	}

	void operator *=(float _v) {
		x *= _v;
		y *= _v;
		z *= _v;
	}

	//Indirect modifiers
	//Vector input
	vec3f operator +(vec3f _v) {
		return vec3f(x + _v.x, y + _v.y, z + _v.z);
	}

	vec3f operator -(vec3f _v) {
		return vec3f(x - _v.x, y - _v.y, z - _v.z);
	}

	vec3f operator /(vec3f _v) {
		return vec3f(x / _v.x, y / _v.y, z / _v.z);
	}

	vec3f operator *(vec3f _v) {
		return vec3f(x * _v.x, y * _v.y, z * _v.z);
	}

	//Non vector input
	vec3f operator /(float _v) {
		return vec3f(x / _v, y / _v, z / _v);
	}

	vec3f operator *(float _v) {
		return vec3f(x * _v, y * _v, z * _v);
	}

	float Dot(vec3f* _v) {
		return (x * _v->x + y * _v->y + z * _v->z);
	}
};

struct vec4f {
	float x, y, z, w;
	vec4f() {
		x = y = z = w = 0.0f;
	}

	vec4f(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	//Operators
	//Direct modifiers
	//Vector input
	void operator +=(vec4f _v) {
		x += _v.x;
		y += _v.y;
		z += _v.z;
		w += _v.w;
	}

	void operator -=(vec4f _v) {
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
		w -= _v.w;
	}

	void operator /=(vec4f _v) {
		x /= _v.x;
		y /= _v.y;
		z /= _v.z;
		w /= _v.w;
	}

	void operator *=(vec4f _v) {
		x *= _v.x;
		y *= _v.y;
		z *= _v.z;
		w *= _v.w;
	}

	//Non vector input
	void operator /=(float _v) {
		x /= _v;
		y /= _v;
		z /= _v;
		w /= _v;
	}

	void operator *=(float _v) {
		x *= _v;
		y *= _v;
		z *= _v; 
		w *= _v;
	}

	//Indirect modifiers
	//Vector input
	vec4f operator +(vec4f _v) {
		return vec4f(x + _v.x, y + _v.y, z + _v.z, w + _v.w);
	}

	vec4f operator -(vec4f _v) {
		return vec4f(x - _v.x, y - _v.y, z - _v.z, w - _v.w);
	}

	vec4f operator /(vec4f _v) {
		return vec4f(x / _v.x, y / _v.y, z / _v.z, w / _v.w);
	}

	vec4f operator *(vec4f _v) {
		return vec4f(x * _v.x, y * _v.y, z * _v.z, w * _v.w);
	}

	//Non vector input
	vec4f operator /(float _v) {
		return vec4f(x / _v, y / _v, z / _v, w / _v);
	}

	vec4f operator *(float _v) {
		return vec4f(x * _v, y * _v, z * _v, w * _v);
	}

	float Dot(vec4f* _v) {
		return (x * _v->x + y * _v->y + z * _v->z + w * _v->w);
	}
};

struct mat4 {
	//Each vec4f holds a horizontal slice of the matrix
	vec4f rowOne, rowTwo, rowThree, rowFour;

	mat4() {
		rowOne = vec4f();
		rowTwo = vec4f();
		rowThree = vec4f();
		rowFour = vec4f();
	}

	mat4(vec4f _one, vec4f _two, vec4f _three, vec4f _four) {
		rowOne = _one;
		rowTwo = _two;
		rowThree = _three;
		rowFour = _four;
	}

	void MatrixMultiply(mat4 _mat) {
		//Build temporary vertical slices of second matrix
		vec4f vOne = vec4f(_mat.rowOne.x, _mat.rowTwo.x, _mat.rowThree.x, _mat.rowFour.x);
		vec4f vTwo = vec4f(_mat.rowOne.y, _mat.rowTwo.y, _mat.rowThree.y, _mat.rowFour.y);
		vec4f vThree = vec4f(_mat.rowOne.z, _mat.rowTwo.z, _mat.rowThree.z, _mat.rowFour.z);
		vec4f vFour = vec4f(_mat.rowOne.w, _mat.rowTwo.w, _mat.rowThree.w, _mat.rowFour.w);

		rowOne *= vOne;
		rowTwo *= vTwo;
		rowThree *= vThree;
		rowFour *= vFour;
	}

	mat4 GetMatrixMultiply(mat4 _mat) {
		//Build temporary vertical slices of second matrix
		vec4f vOne = vec4f(_mat.rowOne.x, _mat.rowTwo.x, _mat.rowThree.x, _mat.rowFour.x);
		vec4f vTwo = vec4f(_mat.rowOne.y, _mat.rowTwo.y, _mat.rowThree.y, _mat.rowFour.y);
		vec4f vThree = vec4f(_mat.rowOne.z, _mat.rowTwo.z, _mat.rowThree.z, _mat.rowFour.z);
		vec4f vFour = vec4f(_mat.rowOne.w, _mat.rowTwo.w, _mat.rowThree.w, _mat.rowFour.w);

		return mat4(
			rowOne * vOne,
			rowTwo * vTwo,
			rowThree * vThree,
			rowFour * vFour
		);
	}

	vec4f VectorMultiply(vec4f _vec) {
		return vec4f(
			rowOne.Dot(&_vec),
			rowTwo.Dot(&_vec),
			rowThree.Dot(&_vec),
			rowFour.Dot(&_vec)
		);
	}

	void PrintDebugInfo() {
		std::cout << "__________Matrix debug Info__________" << std::endl;
		std::cout << rowOne.x << " " << rowOne.y << " " << rowOne.z << " " << rowOne.w << std::endl;
		std::cout << rowTwo.x << " " << rowTwo.y << " " << rowTwo.z << " " << rowTwo.w << std::endl;
		std::cout << rowThree.x << " " << rowThree.y << " " << rowThree.z << " " << rowThree.w << std::endl;
		std::cout << rowFour.x << " " << rowFour.y << " " << rowFour.z << " " << rowFour.w << std::endl;
	}
};