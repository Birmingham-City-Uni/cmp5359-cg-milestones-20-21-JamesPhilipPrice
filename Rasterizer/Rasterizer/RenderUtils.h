#pragma once
#include "Geometry.h"
#include "ImageUtil.h"

class RenderUtils {
public:
	RenderUtils() {}
	~RenderUtils() {}

	void RenderPoint(Vertex _point, Image* _img);
	void RenderLine(Vertex _pointOne, Vertex _pointTwo, Image* _img);
	void RenderTrianlge(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, Image* _img);
};