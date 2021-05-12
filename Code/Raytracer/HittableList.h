#pragma once

#include "Hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> _object) {
		Add(_object);
	}

	void Clear() { objects.clear(); }
	void Add(shared_ptr<Hittable> _object) { objects.push_back(_object); }

	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const override;

private:
	std::vector<shared_ptr<Hittable>> objects;

};

bool HittableList::Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	auto closestSoFar = _tMax;
	for (const auto& object : objects) {
		if (object->Hit(_r, _tMin, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			_rec = tempRec;
		}
	}
	return hitAnything;
}