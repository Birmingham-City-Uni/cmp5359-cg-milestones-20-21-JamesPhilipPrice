#pragma once
#include <SDL_image.h>
#include "ImageUtil.h"

class Material {
public:
	Material(const char* _difuse = nullptr, const char* _normal = nullptr, const char* _roughness = nullptr) {
		if (_difuse != nullptr) {
			SDL_Surface* temp = IMG_Load(_difuse);
			difuseWidth = temp->w;
			difuseHeight = temp->h;
			int bpp = temp->format->BytesPerPixel;
			for (int y = 0; y < temp->h; y++) {
				std::vector<RGB> row;
				for (int x = 0; x < temp->w; x++) {
					Uint32 pixCol = GetPixelOfSurface(temp, bpp, x, y);
					SDL_Color rgb;
					SDL_GetRGB(pixCol, temp->format, &rgb.r, &rgb.g, &rgb.b);
					row.push_back(RGB(rgb.r/255.0f, rgb.g/255.0f, rgb.b/255.0f));
				}
				difuseTexture.push_back(row);
			}
			SDL_FreeSurface(temp);
		}

		if (_normal != nullptr) {
			SDL_Surface* temp = IMG_Load(_normal);
			int bpp = temp->format->BytesPerPixel;
			for (int y = 0; y < temp->h; y++) {
				std::vector<RGB> row;
				for (int x = 0; x < temp->w; x++) {
					Uint32 pixCol = GetPixelOfSurface(temp, bpp, x, y);
					SDL_Color rgb;
					SDL_GetRGB(pixCol, temp->format, &rgb.r, &rgb.g, &rgb.b);
					row.push_back(RGB(rgb.r, rgb.g, rgb.b));
				}
				difuseTexture.push_back(row);
			}
			SDL_FreeSurface(temp);
		}

		if (_roughness != nullptr) {
			SDL_Surface* temp = IMG_Load(_roughness);
			int bpp = temp->format->BytesPerPixel;
			for (int y = 0; y < temp->h; y++) {
				std::vector<RGB> row;
				for (int x = 0; x < temp->w; x++) {
					Uint32 pixCol = GetPixelOfSurface(temp, bpp, x, y);
					SDL_Color rgb;
					SDL_GetRGB(pixCol, temp->format, &rgb.r, &rgb.g, &rgb.b);
					row.push_back(RGB(rgb.r, rgb.g, rgb.b));
				}
				difuseTexture.push_back(row);
			}
			SDL_FreeSurface(temp);
		}
	}
	~Material() {}

	Uint32 GetPixelOfSurface(SDL_Surface* _surface, int _bpp, int _x, int _y) {
		Uint8* pixel = (Uint8*)_surface->pixels + _y * _surface->pitch + _x * _bpp;
		switch (_bpp) {
		case 1:
			return *pixel;
			break;
		case 2:
			return *(Uint16*)pixel;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
			}
			else {
				return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
			}
			break;
		case 4:
			return *(Uint32*)pixel;
			break;
		default:
			return 0;
			break;
		}
	}



	RGB GetDifusePixel(int _u, int _v) {
		int x = _u * (difuseWidth-1);
		int y = _v * (difuseHeight-1);
		return difuseTexture[y][x];
	}

private:
	std::vector<std::vector<RGB>> difuseTexture;
	int difuseWidth, difuseHeight;
	RGB* normalTexture;
	RGB* roughnessTexture;
};