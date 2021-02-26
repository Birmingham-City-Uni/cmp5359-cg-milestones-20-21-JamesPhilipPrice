// Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Model.h"
#include "ImageUtil.h"
#include "RenderUtils.h"

int main()
{
    Image* testImage = new Image(512, 512);
    testImage->SetPixel(511, 511, RGB(255, 0, 0));
    testImage->WriteImage("testExp.ppm");
}
