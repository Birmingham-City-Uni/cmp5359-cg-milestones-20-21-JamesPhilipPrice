#pragma once
#include <SDL.h>
#include <SDL_image.h>

struct RGB {
    float r, g, b;
    RGB() : r(0), g(0), b(0){}
    RGB(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
};

class Texture {
public:
	Texture(const char* filename) {
		SDL_Surface* tempSurface = IMG_Load(filename);
		width = tempSurface->w;
		height = tempSurface->h;
		format = tempSurface->format;
		pixels = new RGB[width * height];
		for (int y = 0; y < width; y++) {
			for (int x = 0; x < height; x++) {
                Uint32 pixCol =  GetPixelFromSurface(tempSurface, x, y);
                SDL_Color rgb;
                SDL_GetRGB(pixCol, format, &rgb.r, &rgb.g, &rgb.b);
                pixels[y * (width - 1) + x] = RGB(rgb.r, rgb.g, rgb.b);
			}
		}
        SDL_FreeSurface(tempSurface);
        std::cout << "New Texture. W: " << width << ", H: " << height << std::endl;
	}

    ~Texture() {}

    RGB GetPixelFromUV(float u, float v) {
        try {
            int x = (int)(u * (width - 1));
            int y = (int)(v * (height - 1));
            if (u >= 0 && u <= 1 && v >= 0 && v <= 1) {
                
                return pixels[y * (width - 1) + x];
            }
            else {
                throw((y * (width - 1) + x));
            }
        }
        catch (int attempedIndex) {
            std::cout << "Trying the write from pixel: " << attempedIndex << " where max index is: " << width * height << std::endl;
            std::cout << "Using U: " << u << " and V: " << v << std::endl;
        }
        
    }

public:
	SDL_PixelFormat* format;
	int width, height;

private:
    /* Source from https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface */
    Uint32 GetPixelFromSurface(SDL_Surface* surface, int x, int y)
    {
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

        switch (bpp)
        {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16*)p;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32*)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
        }
    }
    /* Source ends here */

private:
	RGB* pixels;
};