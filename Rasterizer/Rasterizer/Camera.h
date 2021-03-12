#pragma once
#include <math.h>
#include "GeometryCore.h"

class Camera {
public:
	Camera(vec3f _pos, float _pitch, float _yaw, float _hFOV, float _ar) : position(_pos), pitch(_pitch), yaw(_yaw), horizontalFOV(_hFOV), aspectRatio(_ar) {
		verticalFOV = horizontalFOV * aspectRatio;
	}
	~Camera() {}

	mat4 GetViewMatrix() {
		float cosPitch = cosf(pitch * PI / 180.0f);
		float sinPitch = sinf(pitch * PI / 180.0f);
		float cosYaw = cosf(pitch * PI / 180.0f);
		float sinYaw = sinf(pitch * PI / 180.0f);

		vec3f xAxis = vec3f(cosYaw, 0, -sinYaw);
		vec3f yAxis = vec3f(sinYaw * sinPitch, cosPitch, cosPitch * sinPitch);
		vec3f zAxis = vec3f(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

		mat4 tempMat = mat4(
			vec4f(xAxis.x, yAxis.x, zAxis.x, 0),
			vec4f(xAxis.y, yAxis.y, zAxis.y, 0),
			vec4f(xAxis.z, yAxis.z, zAxis.z, 0),
			vec4f(-(xAxis.Dot(&position)), -(yAxis.Dot(&position)), -(zAxis.Dot(&position)), 1.0f)
		);

		return tempMat;
	}

	//To do get projection matrix out

private:
	//World position and orientation
	float pitch, yaw;
	vec3f position;

	//Clipping plane and viewport information
	float nearPlane, farPlane;
	float verticalFOV, horizontalFOV;
	float aspectRatio; //image height/width
};