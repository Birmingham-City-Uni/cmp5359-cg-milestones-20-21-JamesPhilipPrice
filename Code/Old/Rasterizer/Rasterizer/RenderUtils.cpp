﻿#include "RenderUtils.h"

void RenderUtils::SetupZBuffer()
{
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            zBuffer[y][x] = std::numeric_limits<float>::min();
        }
    }
}

void RenderUtils::RenderPoint(Vertex _point, Image* _img)
{
}

void RenderUtils::RenderLine(Vertex _pointOne, Vertex _pointTwo, Image* _img)
{
    _pointOne *= scalingFactor;
    _pointTwo *= scalingFactor;

    bool steep = false;
    if (abs(_pointOne.x - _pointTwo.x) < abs(_pointOne.y - _pointTwo.y)) {
        std::swap(_pointOne.x, _pointOne.y);
        std::swap(_pointTwo.x, _pointTwo.y);
        steep = true;
    }
    if (_pointOne.x > _pointTwo.x) {
        std::swap(_pointOne.x, _pointTwo.x);
        std::swap(_pointOne.y, _pointTwo.y);
    }
    for (float x = _pointOne.x; x < _pointTwo.x; x += 1) {
        float t = (x - _pointOne.x) / (_pointTwo.x - _pointOne.x);
        float y = _pointOne.y * (1 - t) + _pointTwo.y * t;
        if (steep) {

            _img->SetPixel(y, x, RGB(255, 255, 255));
        }
        else {
            _img->SetPixel(x, y, RGB(255, 255, 255));
        }
    }
}

vec2f CalculateBarycentricBasedUV(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, vec3f _Q) {
    vec2f uv;
    vec3f A = vec3f(_pointOne.x, _pointOne.y, _pointOne.z);
    vec3f B = vec3f(_pointTwo.x, _pointTwo.y, _pointTwo.z);
    vec3f C = vec3f(_pointThree.x, _pointThree.y, _pointThree.z);
    //Vectors to bounding verticies
    vec3f QA = A - _Q;
    vec3f QB = B - _Q;
    vec3f QC = C - _Q;
    vec3f AB = B - A;
    vec3f AC = C - A;
    //Calculation of weightings
    float fullArea = (AB.Cross(&AC)).Length() / 2.0f;
    float alpha = ((QB.Cross(&QC)).Length() / 2.0f) / fullArea;
    float beta = ((QA.Cross(&QC)).Length() / 2.0f) / fullArea;
    float gamma = ((QA.Cross(&QB)).Length() / 2.0f) / fullArea;
    if (gamma != 1 - alpha - beta) {
        std::cout << "Barycentric coordinates error" << std::endl;
    }
    uv.x = (_pointOne.u * alpha) + (_pointTwo.u * beta) + (_pointThree.u * gamma);
    uv.y = (_pointOne.v * alpha) + (_pointTwo.v * beta) + (_pointThree.v * gamma);
    return uv;
}

void RenderUtils::RenderTrianlge(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, Image* _img, Material* _mat)
{   
    float xScale = WIDTH / 2.0f;
    float yScale = HEIGHT / 2.0f;

    if (_pointOne.y == _pointTwo.y && _pointOne.y == _pointThree.y) return; // Ignore degenerate triangles 
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
    if (_pointOne.y > _pointTwo.y) std::swap(_pointOne, _pointTwo);
    if (_pointOne.y > _pointThree.y) std::swap(_pointOne, _pointThree);
    if (_pointTwo.y > _pointThree.y) std::swap(_pointTwo, _pointThree);
    int total_height = _pointThree.y - _pointOne.y;
    for (int i = 0; i < total_height; i++) {
        bool second_half = i > _pointTwo.y - _pointOne.y || _pointTwo.y == _pointOne.y;
        int segment_height = second_half ? _pointThree.y - _pointTwo.y : _pointTwo.y - _pointOne.y;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? _pointTwo.y - _pointOne.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
        //Create vertices for the line to be drawn with the UV
        Vertex A = _pointOne + (_pointThree - _pointOne) * alpha;
        Vertex B = second_half ? _pointTwo + (_pointThree - _pointTwo) * beta : _pointOne + (_pointTwo - _pointOne) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            //Make sure pixel is in the image space
            if ((j >= 0 && j < WIDTH) && (_pointOne.y + i >= 0 && _pointOne.y + i < HEIGHT)) {
                float zBuffVal = A.z + (((j - A.x) / (B.x - A.x)) * (B.z - A.z));
                int y = (_pointOne.y + i);
                if (zBuffVal > zBuffer[y][j]) {
                    //Where the actual colour calculation happens
                    // calculate u,v from barycentric coordinate of Q ( Q in Triangle)
                    vec3f Q((j / xScale)-1.0f, (i / yScale)-1.0f, zBuffVal - 1.0f);
                    Vertex pA((_pointOne.x / xScale) - 1.0f, (_pointOne.y / yScale) - 1.0f, _pointOne.z - 1.0, _pointOne.u, _pointOne.v);
                    Vertex pB((_pointTwo.x / xScale) - 1.0f, (_pointTwo.y / yScale) - 1.0f, _pointTwo.z - 1.0, _pointTwo.u, _pointTwo.v);
                    Vertex pC((_pointThree.x / xScale) - 1.0f, (_pointThree.y / yScale) - 1.0f, _pointThree.z - 1.0, _pointThree.u, _pointThree.v);
                    vec2f uv = CalculateBarycentricBasedUV(pA, pB, pC, Q);

                    _img->SetPixel(j, _pointOne.y + i, _mat->GetDifusePixel(uv.x, uv.y));
                    zBuffer[y][j] = zBuffVal;
                }
            }
            
            
        }
    }
}