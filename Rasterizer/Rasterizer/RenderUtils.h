#pragma once
#include "Geometry.h"
#include "ImageUtil.h"
#include "GeometryCore.h"

class RenderUtils {
public:
	RenderUtils(float _scaleFac) : scalingFactor(_scaleFac) {}
	~RenderUtils() {}

	void RenderPoint(Vertex _point, Image* _img);
	void RenderLine(Vertex _pointOne, Vertex _pointTwo, Image* _img);
	void RenderTrianlge(vec2f _pointOne, vec2f _pointTwo, vec2f _pointThree, Image* _img);

private:
	float scalingFactor = 0;
};