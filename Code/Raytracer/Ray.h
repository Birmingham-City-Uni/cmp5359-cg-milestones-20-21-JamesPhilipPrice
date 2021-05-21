#pragma once
/* Sourced from module tutorials */
#include "geometry.h"

class Ray {
public:
	Ray(){}
	Ray(const Point3f& _origin, const Vec3f& _direction) : origin(_origin), direction(_direction) {}

	Point3f Origin() const { return origin; }
	Vec3f Direction() const { return direction; }

	Point3f At(double _t)const {
		return origin + _t * direction;
	}

public:
	Point3f origin;
	Vec3f direction;
};
/* Code source stops */