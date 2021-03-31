#pragma once
#include <vector>
#include <iostream>
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

	float Dot(vec3f& _v) {
		return (x * _v.x + y * _v.y + z * _v.z);
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
		return ((x * _v->x) + (y * _v->y) + (z * _v->z) + (w * _v->w));
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

		rowOne = vec4f(rowOne.Dot(&vOne), rowOne.Dot(&vTwo), rowOne.Dot(&vThree), rowOne.Dot(&vFour));
		rowTwo = vec4f(rowTwo.Dot(&vOne), rowTwo.Dot(&vTwo), rowTwo.Dot(&vThree), rowTwo.Dot(&vFour));
		rowThree = vec4f(rowThree.Dot(&vOne), rowThree.Dot(&vTwo), rowThree.Dot(&vThree), rowThree.Dot(&vFour));
		rowFour = vec4f(rowFour.Dot(&vOne), rowFour.Dot(&vTwo), rowFour.Dot(&vThree), rowFour.Dot(&vFour));
	}

	mat4 GetMatrixMultiply(mat4 _mat) {
		//Build temporary vertical slices of second matrix
		vec4f vOne = vec4f(_mat.rowOne.x, _mat.rowTwo.x, _mat.rowThree.x, _mat.rowFour.x);
		vec4f vTwo = vec4f(_mat.rowOne.y, _mat.rowTwo.y, _mat.rowThree.y, _mat.rowFour.y);
		vec4f vThree = vec4f(_mat.rowOne.z, _mat.rowTwo.z, _mat.rowThree.z, _mat.rowFour.z);
		vec4f vFour = vec4f(_mat.rowOne.w, _mat.rowTwo.w, _mat.rowThree.w, _mat.rowFour.w);

		return mat4(
			vec4f(rowOne.Dot(&vOne), rowOne.Dot(&vTwo), rowOne.Dot(&vThree), rowOne.Dot(&vFour)),
			vec4f(rowTwo.Dot(&vOne), rowTwo.Dot(&vTwo), rowTwo.Dot(&vThree), rowTwo.Dot(&vFour)),
			vec4f(rowThree.Dot(&vOne), rowThree.Dot(&vTwo), rowThree.Dot(&vThree), rowThree.Dot(&vFour)),
			vec4f(rowFour.Dot(&vOne), rowFour.Dot(&vTwo), rowFour.Dot(&vThree), rowFour.Dot(&vFour))
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

	/*Source from Carlo Harvey (https://github.com/drcarlo/Software-Rasteriser-Starter) from geometry.h for finding inverse of 4x4 matrix*/

	// Compute the inverse of the matrix using the Gauss-Jordan (or reduced row) elimination method.
	// Don't worry at this point if you don't understand how this works. We need to be able to efficiently 
	// compute an inverse of a matric for CameraToWorld and WorldToCamera manipulation. This allows this.  
	mat4 Inverse() const
	{
		int i, j, k;
		std::vector<std::vector<float>> s = {
			std::vector<float> {1.0f, 0.0f, 0.0f, 0.0f},
			std::vector<float> {0.0f, 1.0f, 0.0f, 0.0f},
			std::vector<float> {0.0f, 0.0f, 1.0f, 0.0f},
			std::vector<float> {0.0f, 0.0f, 0.0f, 1.0f}
		};

		std::vector<std::vector<float>> t = {
			std::vector<float> {rowOne.x, rowOne.y, rowOne.z, rowOne.w},
			std::vector<float> {rowTwo.x, rowTwo.y, rowTwo.z, rowTwo.w},
			std::vector<float> {rowThree.x, rowThree.y, rowThree.z, rowThree.w},
			std::vector<float> {rowFour.x, rowFour.y, rowFour.z, rowFour.w}
		};

		// Forward elimination
		for (i = 0; i < 3; i++) {
			int pivot = i;

			float pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < 4; j++) {
				float tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				// Cannot invert singular matrix
				return mat4();
			}

			if (pivot != i) {
				for (j = 0; j < 4; j++) {
					float tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < 4; j++) {
				float f = t[j][i] / t[i][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = 3; i >= 0; --i) {
			float f;

			if ((f = t[i][i]) == 0) {
				// Cannot invert singular matrix
				return mat4();
			}

			for (j = 0; j < 4; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < 4; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		mat4 outputMat = mat4(
			vec4f(s[0][0], s[0][1], s[0][2], s[0][3]),
			vec4f(s[1][0], s[1][1], s[1][2], s[1][3]),
			vec4f(s[2][0], s[2][1], s[2][2], s[2][3]),
			vec4f(s[3][0], s[3][1], s[3][2], s[3][3])
		);

		return outputMat;
	}

	/*Source from Carlo Harvey ends*/

	void PrintDebugInfo() {
		std::cout << "__________Matrix debug Info__________" << std::endl;
		std::cout << rowOne.x << " " << rowOne.y << " " << rowOne.z << " " << rowOne.w << std::endl;
		std::cout << rowTwo.x << " " << rowTwo.y << " " << rowTwo.z << " " << rowTwo.w << std::endl;
		std::cout << rowThree.x << " " << rowThree.y << " " << rowThree.z << " " << rowThree.w << std::endl;
		std::cout << rowFour.x << " " << rowFour.y << " " << rowFour.z << " " << rowFour.w << std::endl;
	}
};