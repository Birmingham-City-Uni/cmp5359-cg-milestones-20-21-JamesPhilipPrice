#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

//usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//utility functions
inline double RandomDouble() {
	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double _min, double _max) {
	return _min + (_max - _min) * RandomDouble();
}

inline double DegreesToRadians(double _degrees) {
	return _degrees * pi / 180.0;
}

//common headers
#include "Ray.h"
#include "geometry.h"