// Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Model.h"
#include "ImageUtil.h"
#include "RenderUtils.h"

#define WIDTH 512
#define HEIGHT 512

int main()
{
    GeomUtils* gUtil = new GeomUtils();
    RenderUtils* renUtil = new RenderUtils(1);

    Model* testModel = new Model("cc.obj", gUtil);

    Vertex testVertOne = Vertex(10, 0, 0, 0, 0);
    Vertex testVertTwo = Vertex(20, 10, 0, 0, 0);

    Image* testImage = new Image(WIDTH, HEIGHT);

    for (int v = 0; v < testModel->GetFaceCount(); v++) {
        int index = v * 3;
        Vertex one = testModel->GetVertex(index);
        Vertex two = testModel->GetVertex(index+1);
        Vertex three = testModel->GetVertex(index+2);
        one *= WIDTH;
        two *= WIDTH;
        three *= WIDTH;
        renUtil->RenderLine(one, two, testImage);
        renUtil->RenderLine(two, three, testImage);
        renUtil->RenderLine(three, one, testImage);
    }

    //renUtil->RenderLine(testVertOne, testVertTwo, testImage);
    //testImage->SetPixel(511, 511, RGB(255, 0, 0));
    testImage->WriteImage("testExp.ppm");
}
