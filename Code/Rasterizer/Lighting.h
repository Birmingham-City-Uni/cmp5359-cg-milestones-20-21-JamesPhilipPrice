#pragma once
#include <vector>
#include "geometry.h"
#include "texture.h"

class DirectionalLight {
public:
	DirectionalLight(RGB _col, Vec3f _dir) : colour(_col), direction(_dir){}
	~DirectionalLight() {}

	RGB colour;
	Vec3f direction;
};

class LightSystem {
public:
	LightSystem() {}
	~LightSystem() {}

	void SetAmbientLight(RGB _col) {
		ambient = _col;
	}

	void AddDirLight(RGB _col, Vec3f _dir){
		dirLights.push_back(new DirectionalLight(_col, _dir));
	}

	RGB GetLightingFromScene(Vec3f _norm) {
		RGB lightCol(0.0f, 0.0f, 0.0f);
		lightCol += ambient;

		//Run through directional lighting
		for (DirectionalLight* l : dirLights) {
			Vec3f lDir = l->direction;
			float intensity = std::max(0.f, _norm.dotProduct(lDir));
			lightCol += (l->colour) * intensity;
		}

		lightCol = ClampLightingCol(lightCol);
		return lightCol;
	}

	RGB ClampLightingCol(RGB _col) {
		if (_col.r < 0) {
			_col.r = 0.0f;
		}
		else if (_col.r > 1) {
			_col.r = 1.0f;
		}
		if (_col.g < 0) {
			_col.g = 0.0f;
		}
		else if (_col.g > 1) {
			_col.g = 1.0f;
		}
		if (_col.b < 0) {
			_col.b = 0.0f;
		}
		else if (_col.b > 1) {
			_col.b = 1.0f;
		}
		return _col;
	}

private:
	RGB ambient;
	std::vector<DirectionalLight*> dirLights;
};