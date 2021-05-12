#pragma once
#include <math.h>
#include "GeometryCore.h"

class Camera {
public:
	Camera(vec3f _pos, float _pitch, float _yaw, float _hFOV, float _width, float _height, float _near, float _far) {
		position = _pos;
		pitch = _pitch;
		yaw = _yaw;
		nearPlane = _near;
		farPlane = _far;

		verticalFOV = _hFOV;
		width = _width;
		height = _height;
		aspectRatio = width / height;

		top = tanf((verticalFOV / 2.0) * PI / 180.0f) * nearPlane;
		bottom = -top;
		left = -top * aspectRatio;
		right = top * aspectRatio;
	}
	~Camera() {}

	mat4 GetViewMatrix() {
		vec3f identity = vec3f();
		float cosPitch = cosf(pitch * PI / 180.0f);
		float sinPitch = sinf(pitch * PI / 180.0f);
		float cosYaw = cosf(yaw * PI / 180.0f);
		float sinYaw = sinf(yaw * PI / 180.0f);

		vec3f xAxis = vec3f(cosYaw, 0, -sinYaw);
		vec3f yAxis = vec3f(sinYaw * sinPitch, cosPitch, cosPitch * sinPitch);
		vec3f zAxis = vec3f(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

		mat4 tempMat = mat4(
			vec4f(xAxis.x, yAxis.x, zAxis.x, 0),
			vec4f(xAxis.y, yAxis.y, zAxis.y, 0),
			vec4f(xAxis.z, yAxis.z, zAxis.z, 0),
			vec4f(-(xAxis.Dot(&identity)), -(yAxis.Dot(&identity)), -(zAxis.Dot(&identity)), 1.0f)
		);

		return tempMat;
	}

	mat4 GetProjectionMatrix() {

		mat4 tempMat = mat4(
			vec4f((nearPlane / (width / 2.0f)), 0.0f, (left + right) / (width / 2.0f), 0.0f),
			vec4f(0.0f, nearPlane / (height / 2.0f), (top + bottom) / (height / 2.0f), 0.0f),
			vec4f(0.0f, 0.0f, -((farPlane + nearPlane) / (farPlane - nearPlane)), (2 * farPlane * nearPlane) / (farPlane - nearPlane)),
			vec4f(0.0f, 0.0f, -1.0f, 0.0f)
		);

		return tempMat;
	}

	vec3f GetPosition() { return position; }

private:
	//World position and orientation
	float pitch, yaw;
	vec3f position;

	//Clipping plane and viewport information
	float nearPlane, farPlane;
	float width, height;
	float top, bottom, left, right;
	float verticalFOV;
	float aspectRatio; //image height/width
};