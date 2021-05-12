#include "ImageUtil.h"

Image::Image(int _w, int _h) : width(_w), height(_h)
{
	pixels = new RGB[_w * _h];
	for (int i = 0; i < _w * _h; i++) {
		pixels[i] = RGB(0);
	}
	/*for (int i = 0; i < _w * _h; i++) {
		std::cout << pixels[i].r << std::endl;
		std::cout << pixels[i].g << std::endl;
		std::cout << pixels[i].b << std::endl;
	}*/
}

Image::~Image()
{
}

void Image::SetPixel(int _x, int _y, RGB _c)
{
	if (_x >= 0 && _x < width && _y >= 0 && _y < height) {
		pixels[(_y * width) + _x] = _c;
	}
}

void Image::FlipVertical()
{
	for (int vPos = 0; vPos < height / 2; vPos++) {
		for (int hPos = 0; hPos < width; hPos++) {
			int topIndex = ((height - (vPos + 1)) * width) + hPos;
			int bottomIndex = (vPos * width) + hPos;
			std::swap(pixels[topIndex], pixels[bottomIndex]);
		}
	}
}

void Image::WriteImage(const char* _filename)
{
	if (width == 0 || height == 0) {
		std::cerr << "Cannot write an empty image" << std::endl;
		return;
	}
	std::ofstream fileOut;
	try {
		fileOut.open(_filename, std::ios::binary);
		if (fileOut.fail()) {
			throw("Can't output the file");
		}
		fileOut << "P6\n" << width << " " << height << "\n255\n";
		unsigned char r, g, b;

		for (int i = 0; i < width * height; i++) {
			r = static_cast<unsigned char>(std::min(1.0f, pixels[i].r) * 255);
			g = static_cast<unsigned char>(std::min(1.0f, pixels[i].g) * 255);
			b = static_cast<unsigned char>(std::min(1.0f, pixels[i].b) * 255);
			fileOut << r << g << b;
		}
		fileOut.close();
	}
	catch(const char* _err){
		std::cerr << _err << std::endl;
	}
}