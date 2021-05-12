#pragma once

#include "Common.h"
#include "geometry.h"
#include "Hittable.h"

struct HitRecord;

Vec3f Reflect(const Vec3f& _v, const Vec3f& _n) {
	return _v - 2 * _v.dotProduct(_n) * _n;
}

Vec3f Refract(const Vec3f& _uv, const Vec3f& n, double _etaiOverEtat) {
	auto cosTheta = fmin(-_uv.dotProduct(n), 1.0);
	Vec3f rOutPerp = _etaiOverEtat * (_uv + cosTheta * n);
	Vec3f rOutParallel = -sqrt(fabs(1.0 - rOutPerp.norm())) * n;
	return rOutPerp + rOutParallel;
}

class Material {
public:
	virtual bool Scatter(
		const Ray& _rIn, const HitRecord& _rec, Colour& _attenuation, Ray& _scattered
	) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Colour& _a) : albedo(_a) {}

	virtual bool Scatter(
		const Ray& _rIn, const HitRecord& _rec, Colour& _attenuation, Ray& _scattered
	) const override {
		auto scatterDirection = _rec.normal + Vec3f().RandomInUnitSphere();

		if (scatterDirection.NearZero()) {
			scatterDirection = _rec.normal;
		}
		_scattered = Ray(_rec.p, scatterDirection);
		_attenuation = albedo;
		return true;
	}

	Vec3f Reflect(const Vec3f& _v, const Vec3f& _n) {
		return _v - 2 * _v.dotProduct(_n) * _n;
	}

private:
	Colour albedo;
};

class Metal : public Material {
public:
	Metal(const Colour& _a, double _f) : albedo(_a), fuzz(_f < 1 ? _f : 1){}

	virtual bool Scatter(
		const Ray& _rIn, const HitRecord& _rec, Colour& _attenuation, Ray& _scattered
	) const override {
		Vec3f reflected = Reflect(_rIn.Direction().normalize(), _rec.normal);
		_scattered = Ray(_rec.p, reflected + fuzz * Vec3f().RandomInUnitSphere());
		_attenuation = albedo;
		return (_scattered.Direction().dotProduct(_rec.normal) > 0);
	}

private:
	Colour albedo;
	double fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double _indexOfRefraction) : ir(_indexOfRefraction) {}

	virtual bool Scatter(
		const Ray& _rIn, const HitRecord& _rec, Colour& _attenuation, Ray& _scattered
	) const override {
		_attenuation = Colour(1.0, 1.0, 1.0);
		double refractionRatio = _rec.frontFace ? (1.0 / ir) : ir;

		Vec3f unitDirection = _rIn.Direction().normalize();
		double cosTheta = fmin(-unitDirection.dotProduct(_rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3f direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble()) {
			direction = Reflect(unitDirection, _rec.normal);
		}
		else {
			direction = Refract(unitDirection, _rec.normal, refractionRatio);
		}

		_scattered = Ray(_rec.p, direction);
		return true;
	}
public:
	double ir;

private:
	static double Reflectance(double  _cosine, double _refIdx) {
		//Schlick's aproximation for reflectance
		auto r0 = (1 - _refIdx) / (1 + _refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - _cosine), 5);
	}
};