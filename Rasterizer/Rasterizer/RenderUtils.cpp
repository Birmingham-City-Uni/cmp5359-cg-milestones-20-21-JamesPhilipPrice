#include "RenderUtils.h"

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

void RenderUtils::RenderTrianlge(vec2f _pointOne, vec2f _pointTwo, vec2f _pointThree, Image* _img)
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
        vec2f A = _pointOne + (_pointThree - _pointOne) * alpha;
        vec2f B = second_half ? _pointTwo + (_pointThree - _pointTwo) * beta : _pointOne + (_pointTwo - _pointOne) * beta;
        if (A.x > B.x) std::swap(A, B);
        for (int j = A.x; j <= B.x; j++) {
            _img->SetPixel(j, _pointOne.y + i, RGB(100, 100, 100)); // attention, due to int casts t0.y+i != A.y 
        }
    }
}
