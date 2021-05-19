#pragma once

#include "Hittable.h"
#include "geometry.h"

class Triangle : public Hittable {
public:
	Triangle() {}
	Triangle(Point3f _vert0, Point3f _vert1, Point3f _vert2, shared_ptr<Material> _m) : 
	v0(_vert0), v1(_vert1), v2(_vert2), mat_ptr(_m){
		normal = (v1 - v0).crossProduct(v2 - v0);
	}
	Triangle(Point3f _vert0, Point3f _vert1, Point3f _vert2, Vec3f _vn, shared_ptr<Material> _m) : 
	v0(_vert0), v1(_vert1), v2(_vert2), normal(_vn), mat_ptr(_m) {}

	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const override;
	virtual bool bounding_box(AABB& _outputBox) const override;

public:
	Point3f v0, v1, v2;
	Vec3f normal;
	shared_ptr<Material> mat_ptr;
};

bool Triangle::Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const {
	float tHit, t, u, v;

	Vec3<float> v0v1 = v1 - v0;
	Vec3<float> v0v2 = v2 - v0;

	Vec3<float> pvec = _r.Direction().crossProduct(v0v2);

	float det = pvec.dotProduct(v0v1);
	float kEpsilon = 0.00001;

	//If determinant is negative, triangle is backfacing
	if (det < kEpsilon) {
		return false;
	}
	float invDet = 1 / det;

	Vec3<float> tvec = _r.Origin() - v0;
	u = tvec.dotProduct(pvec) * invDet;

	if (u < 0 || u > 1) {
		return false;
	}

	Vec3<float> qvec = tvec.crossProduct(v0v1);
	v = _r.Direction().dotProduct(qvec) * invDet;
	if (v < 0 || u + v > 1) {
		return false;
	}

	t = v0v2.dotProduct(qvec) * invDet;

	if (t < 0) {
		return false;
	}

	_rec.p = _r.At(t);
	_rec.t = t;

	_rec.normal = normal;
	_rec.matPtr = mat_ptr;

	return true;
}

inline bool Triangle::bounding_box(AABB& _outputBox) const {
	float min[3];
	float max[3];
	for (int i = 0; i < 3; i++) {
		min[i] = std::min(v0[i], std::min(v1[i], v2[i]));
		max[i] = std::max(v0[i], std::max(v1[i], v2[i]));
	}
	_outputBox = AABB(Vec3f(min[0], min[1], min[2]), Vec3f(max[0], max[1], max[2]));
	return true;
}