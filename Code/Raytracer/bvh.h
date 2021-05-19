#pragma once
#include "Common.h"
#include "Hittable.h"
#include "HittableList.h"
#include <algorithm>

class BVHNode : public Hittable {
public:
	BVHNode();
	BVHNode(const HittableList& _list) : BVHNode(_list.objects, 0, _list.objects.size()) {}
	BVHNode(const std::vector<shared_ptr<Hittable>>& _srcObjects, size_t _start, size_t _end);
	
	virtual bool Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const override;
	virtual bool bounding_box(AABB& _outputBox) const override;

public:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB box;
};

bool BVHNode::bounding_box(AABB& _outputBox) const {
	_outputBox = box;
	return true;
}

bool BVHNode::Hit(const Ray& _r, double _tMin, double _tMax, HitRecord& _rec) const {
	if (!box.hit(_r, _tMin, _tMax)) {
		return false;
	}
	bool hitLeft = left->Hit(_r, _tMin, _tMax, _rec);
	bool hitRight = right->Hit(_r, _tMin, _tMax, _rec);

	return hitLeft || hitRight;
}

//Helpers
inline int random_int(int _min, int _max) {
	return static_cast<int>(RandomDouble(_min, _max + 1));
}

inline bool box_compare(const shared_ptr<Hittable> _a, const shared_ptr<Hittable> _b, int _axis) {
	AABB boxA;
	AABB boxB;
	if (!_a->bounding_box(boxA) || !_b->bounding_box(boxB)) {
		std::cerr << "No bounding box in bvh node constructor.\n";
	}
	return boxA.min()[_axis] < boxB.min()[_axis];
}

bool box_x_compare(const shared_ptr<Hittable> _a, const shared_ptr<Hittable> _b) {
	return box_compare(_a, _b, 0);
}

bool box_y_compare(const shared_ptr<Hittable> _a, const shared_ptr<Hittable> _b) {
	return box_compare(_a, _b, 1);
}

bool box_z_compare(const shared_ptr<Hittable> _a, const shared_ptr<Hittable> _b) {
	return box_compare(_a, _b, 2);
}

BVHNode::BVHNode(const std::vector<shared_ptr<Hittable>>& _srcObjects, size_t _start, size_t _end) {
	auto objects = _srcObjects;

	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

	size_t object_span = _end - _start;

	if (object_span == 1) {
		left = right = objects[_start];
	}
	else if (object_span == 2) {
		if (comparator(objects[_start], objects[_start + 1])) {
			left = objects[_start];
			right = objects[_start + 1];
		}
		else {
			left = objects[_start + 1];
			right = objects[_start];
		}
	}
	else {
		std::sort(objects.begin() + _start, objects.begin() + _end, comparator);

		auto mid = _start + object_span / 2;
		left = make_shared<BVHNode>(objects, _start, mid);
		right = make_shared<BVHNode>(objects, mid, _end);
	}

	AABB boxLeft, boxRight;

	if (!left->bounding_box(boxLeft) || !right->bounding_box(boxRight)) {
		std::cerr << "No bounding box in bvh node constructor.\n";
	}
	box = surrounding_box(boxLeft, boxRight);
}