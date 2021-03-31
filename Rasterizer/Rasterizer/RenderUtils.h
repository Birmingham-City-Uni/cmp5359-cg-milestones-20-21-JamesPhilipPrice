#pragma once
#include "Geometry.h"
#include "ImageUtil.h"
#include "GeometryCore.h"

#define WIDTH 1920
#define HEIGHT 1080

class RenderUtils {
public:
	RenderUtils(float _scaleFac) : scalingFactor(_scaleFac) {}
	~RenderUtils() {}

	void SetupZBuffer();

	void RenderPoint(Vertex _point, Image* _img);
	void RenderLine(Vertex _pointOne, Vertex _pointTwo, Image* _img);
	void RenderTrianlge(Vertex _pointOne, Vertex _pointTwo, Vertex _pointThree, Image* _img);

private:
	float zBuffer[HEIGHT][WIDTH];
	float scalingFactor = 0;
};