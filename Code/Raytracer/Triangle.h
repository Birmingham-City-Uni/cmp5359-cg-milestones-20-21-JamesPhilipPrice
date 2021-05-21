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
		v0(_vert0), v1(_vert1), v2(_vert2), normal(_vn), mat_ptr(_m) {
	}

	Triangle(Point3f _vert0, Point3f _vert1, Point3f _vert2, Vec3f _vn0, Vec3f _vn1, Vec3f _vn2, Vec2f _vt0, Vec2f _vt1, Vec2f _vt2, shared_ptr<Material> _m) :
		v0(_vert0), v1(_vert1), v2(_vert2), vn0(_vn0), vn1(_vn1), vn2(_vn2), vt0(_vt0), vt1(_vt1), vt2(_vt2), mat_ptr(_m) {
		normal = (vn0 + vn1 + vn2) / 3.0;
		//normal = (v1 - v0).crossProduct(v2 - v0);
	}

	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const override;
	virtual bool bounding_box(AABB& _outputBox) const override;

	Vec3f CalculateBarycentrics(Point3f _p) const {
		float triArea = (v1 - v0).crossProduct(v2 - v0).length() / 2.0f;
		float alpha = (v1 - _p).crossProduct(v2 - _p).length() / 2.0f;
		float beta = (v0 - _p).crossProduct(v2 - _p).length() / 2.0f;
		float gamma = (v0 - _p).crossProduct(v1 - _p).length() / 2.0f;
		alpha /= triArea;
		beta /= triArea;
		gamma /= triArea;
		return Vec3f(alpha, beta, gamma);
	}

public:
	Point3f v0, v1, v2;
	Vec3f vn0, vn1, vn2;
	Vec2f vt0, vt1, vt2;
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

	//Set the uvs here vvv

	Vec3f bWeightings = CalculateBarycentrics(_rec.p);
	Vec2f UVs = vt0 * bWeightings[0] + vt1 * bWeightings[1] + vt2 * bWeightings[2];

	_rec.u = UVs.x;
	_rec.v = 1 - UVs.y;

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