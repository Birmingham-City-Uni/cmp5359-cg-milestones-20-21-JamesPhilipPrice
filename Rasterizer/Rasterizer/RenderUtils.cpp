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

void RenderUtils::RenderTrianlge(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, Image* _img)
{
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
        Vertex tempA = _pointOne + (_pointThree - _pointOne) * alpha;
        Vertex tempB = second_half ? _pointTwo + (_pointThree - _pointTwo) * beta : _pointOne + (_pointTwo - _pointOne) * beta;
        vec3f A = vec3f(tempA.x, tempA.y, tempA.z);
        vec3f B = vec3f(tempB.x, tempB.y, tempB.z);
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            //Make sure pixel is in the image space
            if ((j >= 0 && j < WIDTH) && (_pointOne.y + i >= 0 && _pointOne.y + i < HEIGHT)) {
                float zBuffVal = A.z + (((j - A.x) / (B.x - A.x)) * (B.z - A.z));
                int y = (_pointOne.y + i);
                if (zBuffVal > zBuffer[y][j]) {
                    _img->SetPixel(j, _pointOne.y + i, RGB(100, 100, 100));
                    zBuffer[y][j] = zBuffVal;
                }
            }
            
            
        }
    }
}
