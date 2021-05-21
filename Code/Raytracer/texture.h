/*Sourced from Peter Shirley's Ray tracing the next week: https://raytracing.github.io/books/RayTracingTheNextWeek.html */

#pragma once
#include "Common.h"
#include "rtw_stb_image.h"

class Texture {
public:
	virtual Colour value(double _u, double _v, const Point3f& _p) const = 0;
};

class SolidColour : public Texture {
public:
	SolidColour() {}
	SolidColour(Colour _c) : colourValue(_c) {}

	SolidColour(double _r, double _g, double _b) : SolidColour(Colour(_r, _g, _b)) {}

	virtual Colour value(double _u, double _v, const Point3f& _p) const override {
		return colourValue;
	}
private:
	Colour colourValue;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture() {}

	CheckerTexture(shared_ptr<Texture> _even, shared_ptr<Texture> _odd) :
		even(_even), odd(_odd) {}

	CheckerTexture(Colour _c1, Colour _c2) :
		even(make_shared<SolidColour>(_c1)), odd(make_shared<SolidColour>(_c2)) {}

	virtual Colour value(double _u, double _v, const Point3f& _p) const override {
		auto sines = sin(10 * _p.x) * sin(10 * _p.y) * sin(10 * _p.z);
		if (sines < 0) {
			return odd->value(_u, _v, _p);
		}
		else {
			return even->value(_u, _v, _p);
		}
	}

public:
	shared_ptr<Texture> even;
	shared_ptr<Texture> odd;
};

class ImageTexture : public Texture {
public:
	const static int bytes_per_pixel = 3;

	ImageTexture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

	ImageTexture(const char* _filename) {
		auto components_per_pixel = bytes_per_pixel;

		data = stbi_load(_filename, &width, &height, &components_per_pixel, components_per_pixel);

		if (!data) {
			std::cerr << "ERROR: Could not load texture image file: " << _filename << std::endl;
			width = height = 0;
		}
		bytes_per_scanline = bytes_per_pixel * width;
	}

	~ImageTexture() {
		delete data;
	}

	virtual Colour value(double _u, double _v, const Point3f& _p) const override {
		if (data == nullptr) {
			return Colour(0, 1, 1);
		}

		//Clamp uv values
		if (_u > 1.0) _u = 1.0;
		if (_u < 0.0) _u = 0.0;
		if (_v > 1.0) _v = 1.0;
		if (_v < 0.0) _v = 0.0;

		auto i = static_cast<int>(_u * width);
		auto j = static_cast<int>(_v * height);

		//Clamp int mapping
		if (i >= width) i = width - 1;
		if (j >= height) j = height - 1;

		const auto colourScale = 1.0 / 255.0;
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
		return Colour(colourScale * pixel[0], colourScale * pixel[1], colourScale * pixel[2]);
	}

private:
	unsigned char *data;
	int width, height;
	int bytes_per_scanline;
};
/* Code source stops*/