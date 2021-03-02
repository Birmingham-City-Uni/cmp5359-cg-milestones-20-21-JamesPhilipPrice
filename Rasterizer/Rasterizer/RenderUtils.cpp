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

void RenderUtils::RenderTrianlge(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, Image* _img)
{

}
