// A practical implementation of the ray tracing algorithm.

#include "Common.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "SDL.h" 
#include <fstream>
#include <chrono>

#define M_PI 3.14159265359


SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* screen;

void init() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Software Ray Tracer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0
    );

    screen = SDL_GetWindowSurface(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16*)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32*)p = pixel;
        break;
    }
}

/*struct Sphere {
    Point3f c;
    double r;
    Sphere(const Point3f& c, double r) : c(c), r(r) {}
    Vec3f getNormal(const Vec3f& pi) const { return (pi - c) / r; }

    // Solve t^2*d.d+2*t*(o-p).d+(o-p).(o-p)-R^2=0​
    bool intersect(const Ray& ray, double& t) const {
        const Point3f o = ray.Origin();
        const Vec3f d = ray.Direction();
        const Vec3f oc = o - c;
        const double b = 2 * oc.dotProduct(d);
        const double c = oc.dotProduct(oc) - r * r;
        double disc = b * b - 4 * c; // a=1 as ray is normalised​
        if (disc < 1e-4) return false; // ray misses sphere​
        disc = sqrt(disc);
        const double t0 = -b - disc;
        const double t1 = -b + disc;
        t = (t0 < t1) ? t0 : t1; // two intersections on sphere, set t to shortest​
        return true;
    }
};*/

// method to ensure colours don’t go out of 8 bit range in RGB​
void clamp255(Vec3f& col) {
    col.x = (col.x > 255) ? 255 : (col.x < 0) ? 0 : col.x;
    col.y = (col.y > 255) ? 255 : (col.y < 0) ? 0 : col.y;
    col.z = (col.z > 255) ? 255 : (col.z < 0) ? 0 : col.z;
}

double HitSphere(const Point3f& _center, double _radius, const Ray& _r) {
    Vec3f oc = _r.Origin() - _center;
    auto a = _r.Direction().dotProduct(_r.Direction());
    auto b = 2.0 * oc.dotProduct(_r.Direction());
    auto c = oc.dotProduct(oc) - _radius * _radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

Colour RayColour(const Ray& _r, const Hittable& _world, int _depth) {
    HitRecord rec;
    if (_depth <= 0) return Colour(0, 0, 0);
    if (_world.Hit(_r, 0.001, infinity, rec)) {
        Ray scattered;
        Colour attenuation;
        if (rec.matPtr->Scatter(_r, rec, attenuation, scattered)) {
            return attenuation * RayColour(scattered, _world, _depth - 1);
        }
        return Colour(0, 0, 0);
    }
    Vec3f unitDirection = _r.Direction().normalize();
    auto t = 0.5 * (unitDirection.y + 1.0);
    return (1.0 - t) * Colour(1.0, 1.0, 1.0) + t * Colour(0.5, 0.7, 1.0) * 255;
}

HittableList RandomizeWorld() {
    HittableList world;
    auto groundMaterial = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3f(0, -1000, 0), 1000, groundMaterial));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = RandomDouble();
            Point3f centre(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
            if ((centre - Point3f(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphereMaterial;
                if (chooseMat < 0.8) {
                    //Difuse
                    auto albedo = Colour::Random() * Colour::Random();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                }
                else if (chooseMat < 0.95) {
                    //Metal
                    auto albedo = Colour::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                }
                else {
                    //Glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(centre, 0.2, sphereMaterial));
                }
            }
        }
    }
    auto material1 = make_shared<Dielectric>(1.5);
    world.Add(make_shared<Sphere>(Point3f(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3f(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3f(4, 1, 0), 1.0, material3));
    return world;
}

HittableList TestModel() {
    HittableList world;

    Vec3f transform(0, 1, 0);
    Model* testModel = new Model("monka.obj");
    auto metal = make_shared<Metal>(Colour(0.7, 0.5, 0.1), 0.0);
    auto ground = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3f(0, -1000, 0), 1000, metal));
    std::cout << testModel->nVerts() << std::endl;

    for (int i = 0; i < testModel->nFaces(); i++) {
        const Vec3f v0 = testModel->Vert(testModel->Face(i)[0]);
        const Vec3f v1 = testModel->Vert(testModel->Face(i)[1]);
        const Vec3f v2 = testModel->Vert(testModel->Face(i)[2]);

        world.Add(make_shared<Triangle>(v0 + transform, v1 + transform, v2 + transform, metal));
    }
    return world;
}

int main(int argc, char **argv)
{
    // initialise SDL2
    init();

    //Image values
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = screen->w;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int spp = 50;
    const float scale = 1.f / spp;
    const int lightBounces = 50;

    //Camera values
    Point3f lookFrom(0, 5, -5);
    Point3f lookAt(0, 0, 0);
    Vec3f vUp(0, 1, 0);
    auto distToFocus = 7;
    auto aperture = 0.15;
    Camera cam(lookFrom, lookAt, vUp, 20, aspectRatio, aperture, distToFocus);

    //World setup
    auto world = TestModel();

    const Colour white(255, 255, 255);
    const Colour black(0, 0, 0);
    const Colour red(255, 0, 0);

    double t;
    Colour pix_col(black);

    SDL_Event e;
    bool running = true;
    while (running) {

        auto t_start = std::chrono::high_resolution_clock::now();

        // clear back buffer, pixel data on surface and depth buffer (as movement)
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_RenderClear(renderer);


        Colour pixelColour = black;
        for (int y = 0; y < screen->h; ++y) {
            std::cout << "Scanlines left: " << screen->h - y << std::endl;
            for (int x = 0; x < screen->w; ++x) {
                pixelColour = black;
                for (int s = 0; s < spp; s++) {
                    auto u = double(x + RandomDouble()) / (imageWidth - 1);
                    auto v = double(y + RandomDouble()) / (imageHeight - 1);
                    Ray ray = cam.GetRay(u, v);
                    pixelColour = pixelColour + RayColour(ray, world, lightBounces);
                }
                pixelColour /= 255.f * spp;
                pixelColour.x = sqrt(pixelColour.x);
                pixelColour.y = sqrt(pixelColour.y);
                pixelColour.z = sqrt(pixelColour.z);
                pixelColour *= 255;
                Uint32 colour = SDL_MapRGB(screen->format, pixelColour.x, pixelColour.y, pixelColour.z);
                putpixel(screen, x, y, colour);
            }
        }

        auto t_end = std::chrono::high_resolution_clock::now();
        auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        std::cerr << "Frame render time:  " << passedTime << " ms" << std::endl;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
        if (texture == NULL) {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(screen);

        SDL_RenderCopyEx(renderer, texture, nullptr, nullptr, 0, 0, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);

        if (SDL_PollEvent(&e))
        {
            switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
                break;
            }
        }
    }

    return 0;
}
