#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>

class Image {
public:

	struct RGB {
		float r, g, b;
		RGB() : r(0), g(0), b(0) {}
		RGB(float _c) : r(_c), g(_c), b(_c) {}
		RGB(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	};


	Image(int _w, int _h);
	~Image();

	void WriteImage(const char* _filename);

private:
	int width;
	int height;
	RGB* pixels;
};