#pragma once
#include "Common.h"

class Camera {
public:
	Camera(Point3f _lookFrom, Point3f _lookAt, Vec3f _vUp, double _vFOV, double _aspectRatio, double _aperture, double _focusDist) {
        //Camera values
        auto theta = DegreesToRadians(_vFOV);
        auto h = tan(theta / 2);
        auto viewportHeight = 2.0 * h;
        auto viewportWidth = _aspectRatio * viewportHeight;
        w = (_lookFrom - _lookAt).normalize();
        u = (_vUp.crossProduct(w)).normalize();
        v = w.crossProduct(u);
        origin = _lookFrom;
        horizontal = _focusDist * viewportWidth * u;
        vertical = _focusDist * viewportHeight * v;
        lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - _focusDist * w;
        lensRadius = _aperture / 2;
	}

    Ray GetRay(double _s, double _t) const {
        Vec3f rd = lensRadius * Vec3f().RandomInUnitDisk();
        Vec3f offset = u * rd.x + v * rd.y;
        return Ray(
            origin + offset,
            lowerLeftCorner + _s * horizontal + _t * vertical - origin - offset
        );
    }

private:
    Point3f origin;
    Point3f lowerLeftCorner;
    Vec3f horizontal;
    Vec3f vertical;
    Vec3f u, v, w;
    double lensRadius;
};