#pragma once
#include "Hittable.h"
#include "geometry.h"
#include "aabb.h"

class Sphere : public Hittable {
public:
	Sphere() {}
	Sphere(Point3f _centre, double _r, shared_ptr<Material> _m) : centre(_centre), radius(_r), materialPtr(_m) {};

	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const override;
	virtual bool bounding_box(AABB& _outputBox) const override;

public:
	Point3f centre;
	double radius;
	shared_ptr<Material> materialPtr;
};

bool Sphere::Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const {
	Vec3f oc = _r.Origin() - centre;
	auto a = _r.Direction().norm();
	auto halfB = oc.dotProduct(_r.Direction());
	auto c = oc.norm() - radius * radius;
	auto discriminant = halfB * halfB - a * c;
	if (discriminant < 0) {
		return false;
	}
	auto sqrtd = sqrt(discriminant);

	auto root = (-halfB - sqrtd) / a;
	if (root < _tMin || _tMax < root) {
		root = (-halfB + sqrtd) / a;
		if (root < _tMin || _tMax < root) {
			return false;
		}
	}
	_rec.t = root;
	_rec.p = _r.At(_rec.t);
	Vec3f outwardNormal = (_rec.p - centre) / radius;
	_rec.SetFaceNormal(_r, outwardNormal);
	_rec.matPtr = materialPtr;
	return true;
}

inline bool Sphere::bounding_box(AABB& _outputBox) const {
	_outputBox = AABB(centre - Vec3f(radius, radius, radius), centre + Vec3f(radius, radius, radius));
	return true;
}