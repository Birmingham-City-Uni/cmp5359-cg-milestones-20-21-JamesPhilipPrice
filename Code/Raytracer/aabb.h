#pragma once
#include "Common.h"

class AABB {
public:
	AABB() {}
	AABB(const Point3f& mini, const Point3f& maxi) { minimum = mini; maximum = maxi; }

	Point3f min() const { return minimum; }
	Point3f max() const { return maximum; }

	bool hit(const Ray& _r, double _tMin, double _tMax) const {
		for (int a = 0; a < 3; a++) {
			auto t0 = fmin((minimum[a] - _r.Origin()[a]) / _r.Direction()[a],
				(maximum[a] - _r.Origin()[a]) / _r.Direction()[a]);
			auto t1 = fmax((minimum[a] - _r.Origin()[a]) / _r.Direction()[a],
				(maximum[a] - _r.Origin()[a]) / _r.Direction()[a]);
			_tMin = fmax(t0, _tMin);
			_tMax = fmin(t1, _tMax);
			if (_tMax <= _tMin) {
				return false;
			}
		}
		return true;
	}

	Point3f minimum;
	Point3f maximum;
};

AABB surrounding_box(AABB box0, AABB box1) {
	Point3f small(fmin(box0.min().x - 1e-3, box1.min().x - 1e-3),
		fmin(box0.min().y - 1e-3, box1.min().y - 1e-3),
		fmin(box0.min().z - 1e-3, box1.min().z - 1e-3));

	Point3f big(fmax(box0.max().x + 1e-3, box1.max().x + 1e-3),
		fmax(box0.max().y + 1e-3, box1.max().y + 1e-3),
		fmax(box0.max().z + 1e-3, box1.max().z + 1e-3));

	return AABB(small, big);
}