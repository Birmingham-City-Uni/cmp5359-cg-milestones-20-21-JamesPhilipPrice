#pragma once
#include "Ray.h"
#include "Common.h"

class Material;

struct HitRecord {
	Point3f p;
	Vec3f normal;
	double t;
	bool frontFace;

	inline void SetFaceNormal(const Ray& _r, const Vec3f& _outwardNormal) {
		frontFace = (_r.Direction().dotProduct(_outwardNormal)) < 0;
		normal = frontFace ? _outwardNormal : -_outwardNormal;
	}
	shared_ptr<Material> matPtr;
};

class Hittable {
public: 
	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const = 0;
};