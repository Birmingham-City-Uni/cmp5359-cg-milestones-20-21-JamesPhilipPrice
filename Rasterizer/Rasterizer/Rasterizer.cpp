// Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Model.h"
#include "ImageUtil.h"

int main()
{
    Image* testImage = new Image(512, 512);
    testImage->WriteImage("testExp.ppm");
}
