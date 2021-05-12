// A practical implementation of the rasterization algorithm.

#include "geometry.h"
#include "SDL.h" 
#include "model.h"
#include "texture.h"
#include <fstream>
#include <chrono>

#define M_PI 3.14159265359

static const float inchToMm = 25.4;
enum FitResolutionGate { kFill = 0, kOverscan };

// Compute screen coordinates based on a physically-based camera model
// http://www.scratchapixel.com/lessons/3d-basic-rendering/3d-viewing-pinhole-camera
void computeScreenCoordinates(
    const float &filmApertureWidth,
    const float &filmApertureHeight,
    const uint32_t &imageWidth,
    const uint32_t &imageHeight,
    const FitResolutionGate &fitFilm,
    const float &nearClippingPlane,
    const float &focalLength,
    float &top, float &bottom, float &left, float &right
)
{
    float filmAspectRatio = filmApertureWidth / filmApertureHeight;
    float deviceAspectRatio = imageWidth / (float)imageHeight;
    
    top = ((filmApertureHeight * inchToMm / 2) / focalLength) * nearClippingPlane;
    right = ((filmApertureWidth * inchToMm / 2) / focalLength) * nearClippingPlane;

    // field of view (horizontal)
    float fov = 2 * 180 / M_PI * atan((filmApertureWidth * inchToMm / 2) / focalLength);
    std::cerr << "Field of view " << fov << std::endl;
    
    float xscale = 1;
    float yscale = 1;
    
    switch (fitFilm) {
        default:
        case kFill:
            if (filmAspectRatio > deviceAspectRatio) {
                xscale = deviceAspectRatio / filmAspectRatio;
            }
            else {
                yscale = filmAspectRatio / deviceAspectRatio;
            }
            break;
        case kOverscan:
            if (filmAspectRatio > deviceAspectRatio) {
                yscale = filmAspectRatio / deviceAspectRatio;
            }
            else {
                xscale = deviceAspectRatio / filmAspectRatio;
            }
            break;
    }
    
    right *= xscale;
    top *= yscale;
    
    bottom = -top;
    left = -right;
}

// Compute vertex raster screen coordinates.
// Vertices are defined in world space. They are then converted to camera space,
// then to NDC space (in the range [-1,1]) and then to raster space.
// The z-coordinates of the vertex in raster space is set with the z-coordinate
// of the vertex in camera space.
void convertToRaster(
    const Vec3f &vertexWorld,
    const Matrix44f &worldToCamera,
    const float &l,
    const float &r,
    const float &t,
    const float &b,
    const float &near,
    const uint32_t &imageWidth,
    const uint32_t &imageHeight,
    Vec3f &vertexRaster
)
{
    Vec3f vertexCamera;

    worldToCamera.multVecMatrix(vertexWorld, vertexCamera);
    
    // convert to screen space
    Vec2f vertexScreen;
    vertexScreen.x = near * vertexCamera.x / -vertexCamera.z;
    vertexScreen.y = near * vertexCamera.y / -vertexCamera.z;
    
    // now convert point from screen space to NDC space (in range [-1,1])
    Vec2f vertexNDC;
    vertexNDC.x = 2 * vertexScreen.x / (r - l) - (r + l) / (r - l);
    vertexNDC.y = 2 * vertexScreen.y / (t - b) - (t + b) / (t - b);

    // convert to raster space
    vertexRaster.x = (vertexNDC.x + 1) / 2 * imageWidth;
    // in raster space y is down so invert direction
    vertexRaster.y = (1 - vertexNDC.y) / 2 * imageHeight;
    vertexRaster.z = -vertexCamera.z;
}

float min3(const float &a, const float &b, const float &c)
{ return std::min(a, std::min(b, c)); }

float max3(const float &a, const float &b, const float &c)
{ return std::max(a, std::max(b, c)); }

float edgeFunction(const Vec3f &a, const Vec3f &b, const Vec3f &c)
{ return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); }

const uint32_t imageWidth = 1920;
const uint32_t imageHeight = 1080;
Matrix44f worldToCamera = {0.707107, -0.331295, 0.624695, 0, 0, 0.883452, 0.468521, 0, -0.707107, -0.331295, 0.624695, 0, -1.63871, -5.747777, -40.400412, 1};

uint32_t ntris = 3156;
const float nearClippingPlane = 1;
const float farClippingPlane = 1000;
float focalLength = 36; // in mm
// 35mm Full Aperture in inches
float filmApertureWidth = 0.980;
float filmApertureHeight = 0.735;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* screen;
void init() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Software Rasteriser",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        imageWidth,
        imageHeight,
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

Matrix44f LookAtRH(Vec3f eye, Vec3f target, Vec3f up)
{
    Vec3f zaxis = (eye - target).normalize();         // The "forward" vector.
    Vec3f xaxis = up.crossProduct(zaxis).normalize(); // The "right" vector.
    Vec3f yaxis = zaxis.crossProduct(xaxis);          // The "up" vector.

    // Create a 4x4 orientation matrix from the right, up, and forward vectors
    Matrix44f orientation = {
        xaxis.x, yaxis.x, zaxis.x, 0,
        xaxis.y, yaxis.y, zaxis.y, 0,
        xaxis.z, yaxis.z, zaxis.z, 0,
              0,       0,       0, 1
    };

    // Create a 4x4 translation matrix.
    // The eye position is negated which is equivalent
    // to the inverse of the translation matrix. 
    Matrix44f translation = {
             1,      0,      0, 0,
             0,      1,      0, 0,
             0,      0,      1, 0,
        -eye.x, -eye.y, -eye.z, 1
    };

    // Combine the orientation and translation to compute 
    // the final view matrix. Note that the order of 
    // multiplication is reversed because the matrices
    // are already inverted.
    return (orientation * translation);
}

Matrix44f lookAt(const Vec3f from, const Vec3f to, const Vec3f _tmp = Vec3f(0, 1, 0))
{
    Vec3f tmp = _tmp;
    Vec3f forward = (from - to).normalize();
    Vec3f right = tmp.normalize().crossProduct(forward);
    Vec3f up = forward.crossProduct(right);

    Matrix44f camToWorld;

    camToWorld[0][0] = right.x;
    camToWorld[0][1] = right.y;
    camToWorld[0][2] = right.z;
    camToWorld[1][0] = up.x;
    camToWorld[1][1] = up.y;
    camToWorld[1][2] = up.z;
    camToWorld[2][0] = forward.x;
    camToWorld[2][1] = forward.y;
    camToWorld[2][2] = forward.z;

    camToWorld[3][0] = from.x;
    camToWorld[3][1] = from.y;
    camToWorld[3][2] = from.z;

    return camToWorld;
}

std::vector<Model*> renderables;
std::vector<Model*> renderables2;
std::vector<Texture*> textures;
std::vector<Texture*> textures2;

//Model* model = nullptr;

int main(int argc, char **argv)
{
    // load model
    if (2 == argc) {
        renderables.push_back(new Model(argv[1]));
    }
    else {
        renderables.push_back(new Model("Objects/eva.obj"));
        textures.push_back(new Texture("Textures/Eva/texture.png"));
        renderables.push_back(new Model("Objects/skyscraper.obj"));
        textures.push_back(new Texture("Textures/Skyscraper/skyscraper_base.png"));
        renderables.push_back(new Model("Objects/skyscraper_glass.obj"));
        textures.push_back(new Texture("Textures/Skyscraper/glass_base.png"));
        renderables.push_back(new Model("Objects/gunstore.obj"));
        textures.push_back(new Texture("Textures/Gunstore/Gun_Store_Main_base.png"));
        renderables.push_back(new Model("Objects/gunstore_lock.obj"));
        textures.push_back(new Texture("Textures/Gunstore/Handle_base.png"));
        renderables.push_back(new Model("Objects/strange1.obj"));
        textures.push_back(new Texture("Textures/Strange/wierd_base.png"));
        //renderables.push_back(new Model("Objects/angle.obj"));
        //textures.push_back(new Texture("Textures/AngleBuilding/Back_Building.png"));
        renderables.push_back(new Model("Objects/floor.obj"));
        textures.push_back(new Texture("Textures/Floor.png"));
    }

    //ntris = model->nverts();

    // initialise SDL2
    init();

    //Matrix44f cameraToWorld = worldToCamera.inverse();

    // compute screen coordinates
    float t, b, l, r;
    
    computeScreenCoordinates(
        filmApertureWidth, filmApertureHeight,
        imageWidth, imageHeight,
        kOverscan,
        nearClippingPlane,
        focalLength,
        t, b, l, r);
    
    // define the frame-buffer and the depth-buffer. Initialize depth buffer
    // to far clipping plane.
    Vec3<unsigned char> *frameBuffer = new Vec3<unsigned char>[imageWidth * imageHeight];
    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) frameBuffer[i] = Vec3<unsigned char>(255);
    float *depthBuffer = new float[imageWidth * imageHeight];
    for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) depthBuffer[i] = farClippingPlane;

    float camAngleX = 0.0f;
    float camAngleY = 0.0f;
    SDL_Event e;
    bool running = true;
    while (running) {

        auto t_start = std::chrono::high_resolution_clock::now();

        // clear back buffer, pixel data on surface and depth buffer (as movement)
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_RenderClear(renderer);
        for (uint32_t i = 0; i < imageWidth * imageHeight; ++i) depthBuffer[i] = farClippingPlane;
        /*SDL_LockSurface(screen);
           //This assumes that color value zero is black. Use
           //SDL_MapRGBA() for more robust surface color mapping! 
           //height times pitch is the size of the surface's whole buffer. 
        SDL_memset(screen->pixels, 0, screen->h * screen->pitch);
        SDL_UnlockSurface(screen);*/

        // Arcball Camera

        camAngleX++;
        if (camAngleX >= 360.f) camAngleX = 0.f;
        // Calculate the camera position using the distance and angles
        float camX = 124.726f;
        float camY = 62.9513f;
        float camZ = 0.0346f;

        Vec3f eye(camX, camY, camZ);
        Vec3f target(111.86f, 62.8965, 4.3578f);
        Vec3f up(0.f, 1.f, 0.f);
        worldToCamera = lookAt(eye, target, up).inverse(); //cameraToWorld needs to be inverted for worldToCamera
        //worldToCamera[3][2] += 0.1f;

        //Test
        float minz = 10000;
        float maxz = -10000;

        // Outer loop
        int textureIndex = 0;
        for (auto& model : renderables) {
            for (uint32_t i = 0; i < model->nfaces(); ++i) {
                /*const Vec3f& v0 = vertices[nvertices[i * 3]];
                const Vec3f& v1 = vertices[nvertices[i * 3 + 1]];
                const Vec3f& v2 = vertices[nvertices[i * 3 + 2]];*/

                const Vec3f& v0 = model->vert(model->face(i)[0]);
                const Vec3f& v1 = model->vert(model->face(i)[3]);
                const Vec3f& v2 = model->vert(model->face(i)[6]);

                // Convert the vertices of the triangle to raster space
                Vec3f v0Raster, v1Raster, v2Raster;
                convertToRaster(v0, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v0Raster);
                convertToRaster(v1, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v1Raster);
                convertToRaster(v2, worldToCamera, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v2Raster);

                // Precompute reciprocal of vertex z-coordinate
                v0Raster.z = 1 / v0Raster.z,
                    v1Raster.z = 1 / v1Raster.z,
                    v2Raster.z = 1 / v2Raster.z;

                // Prepare vertex attributes. Divde them by their vertex z-coordinate
                // (though we use a multiplication here because v.z = 1 / v.z)
                /*Vec2f st0 = st[stindices[i * 3]];
                Vec2f st1 = st[stindices[i * 3 + 1]];
                Vec2f st2 = st[stindices[i * 3 + 2]];*/

                Vec2f st0 = model->vt(model->face(i)[1]);
                Vec2f st1 = model->vt(model->face(i)[4]);
                Vec2f st2 = model->vt(model->face(i)[7]);

                st0 *= v0Raster.z, st1 *= v1Raster.z, st2 *= v2Raster.z;

                float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
                float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
                float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
                float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

                // the triangle is out of screen
                if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

                // be careful xmin/xmax/ymin/ymax can be negative. Don't cast to uint32_t
                uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
                uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
                uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
                uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));

                float area = edgeFunction(v0Raster, v1Raster, v2Raster);

                // Inner loop
                for (uint32_t y = y0; y <= y1; ++y) {
                    for (uint32_t x = x0; x <= x1; ++x) {
                        Vec3f pixelSample(x + 0.5, y + 0.5, 0);
                        //Get edge distances from the points for barycentric weightings
                        float w0 = edgeFunction(v1Raster, v2Raster, pixelSample);
                        float w1 = edgeFunction(v2Raster, v0Raster, pixelSample);
                        float w2 = edgeFunction(v0Raster, v1Raster, pixelSample);
                        //If all weightings are above 0 then the point is inside the triangle
                        if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                            //Divide by the overall area of the triangle to get the correct weightings
                            w0 /= area;
                            w1 /= area;
                            w2 /= area;
                            float oneOverZ = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2;
                            float z = 1 / oneOverZ;
                            // Depth-buffer test to remove hidden faces and pixels not in the near and far clipping planes
                            if (z < depthBuffer[y * imageWidth + x]) {
                                depthBuffer[y * imageWidth + x] = z;
                                
                                if (z < minz) minz = z;
                                if (z > maxz) maxz = z;

                                Vec2f st = st0 * w0 + st1 * w1 + st2 * w2;

                                st *= z;

                                if (st.x < 0 || st.x > 1) continue;
                                if (st.y < 0 || st.y > 1) continue;

                                // If you need to compute the actual position of the shaded
                                // point in camera space. Proceed like with the other vertex attribute.
                                // Divide the point coordinates by the vertex z-coordinate then
                                // interpolate using barycentric coordinates and finally multiply
                                // by sample depth.
                                Vec3f v0Cam, v1Cam, v2Cam;
                                worldToCamera.multVecMatrix(v0, v0Cam);
                                worldToCamera.multVecMatrix(v1, v1Cam);
                                worldToCamera.multVecMatrix(v2, v2Cam);

                                float px = (v0Cam.x / -v0Cam.z) * w0 + (v1Cam.x / -v1Cam.z) * w1 + (v2Cam.x / -v2Cam.z) * w2;
                                float py = (v0Cam.y / -v0Cam.z) * w0 + (v1Cam.y / -v1Cam.z) * w1 + (v2Cam.y / -v2Cam.z) * w2;

                                Vec3f pt(px * z, py * z, -z); // pt is in camera space

                                // Compute the face normal which is used for a simple facing ratio.
                                // Keep in mind that we are doing all calculation in camera space.
                                // Thus the view direction can be computed as the point on the object
                                // in camera space minus Vec3f(0), the position of the camera in camera
                                // space.
                                Vec3f n = (v1Cam - v0Cam).crossProduct(v2Cam - v0Cam);
                                n.normalize();
                                Vec3f viewDirection = -pt;
                                viewDirection.normalize();

                                float nDotView = std::max(0.f, n.dotProduct(viewDirection));

                                // The final color is the result of the fraction multiplied by the
                                // checkerboard pattern.
                                const int M = 10;
                                float checker = (fmod(st.x * M, 1.0) > 0.5) ^ (fmod(st.y * M, 1.0) < 0.5);
                                float c = 0.3 * (1 - checker) + 0.7 * checker;
                                nDotView *= c;
                                frameBuffer[y * imageWidth + x].x = nDotView * 255;
                                frameBuffer[y * imageWidth + x].y = nDotView * 255;
                                frameBuffer[y * imageWidth + x].z = nDotView * 255;

                                //Uint32 colour = SDL_MapRGB(screen->format, nDotView * 255, nDotView * 255, nDotView * 255);
                                RGB texturePixelColour = textures[textureIndex]->GetPixelFromUV(st.x, 1 - st.y);
                                Uint32 colour = SDL_MapRGB(screen->format, texturePixelColour.r, texturePixelColour.g, texturePixelColour.b);
                                putpixel(screen, x, y, colour);
                            }
                        }
                    }
                }
            }
            textureIndex++;
        }
        std::cout << "Min z: " << minz << " Max z: " << maxz << std::endl;

        auto t_end = std::chrono::high_resolution_clock::now();
        auto passedTime = std::chrono::duration<double, std::milli>(t_end - t_start).count();
        std::cerr << "Frame render time:  " << passedTime << " ms" << std::endl;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
        if (texture == NULL) {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(screen);

        SDL_RenderCopy(renderer, texture, NULL, NULL);
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

    // TODO: Fix image writing method
    // Store the result of the framebuffer to a PPM file (Photoshop reads PPM files).
    //std::ofstream ofs;
    //ofs.open("./output.ppm");
    //ofs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
    //ofs.write((char*)frameBuffer, imageWidth * imageWidth * 3);
    //ofs.close();
    
    delete [] frameBuffer;
    delete [] depthBuffer;

    return 0;
}
