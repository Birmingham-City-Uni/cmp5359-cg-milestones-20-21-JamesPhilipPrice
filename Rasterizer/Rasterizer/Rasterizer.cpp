// Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "Model.h"
#include "ImageUtil.h"
#include "RenderUtils.h"
#include "Camera.h"

#define WIDTH 1920
#define HEIGHT 1080

void TransformModel(Model* _model, mat4* _modelviewMat, mat4* _projectionMat) {
    //Run through all verticies in the model
    int vertCount = _model->GetFaceCount() * 3;
    std::cout << "Vert count: " << vertCount << std::endl;
    for (int i = 0; i < vertCount; i++) {
        //Get a vec4f from the Vertex
        Vertex tempVert = _model->GetVertex(i);
        vec4f vertPos = vec4f(tempVert.x, tempVert.y, tempVert.z, 1.0f);
        //Matrix-Vector multiplication with the modelview matrix, and then the projection matrix
        vertPos = _modelviewMat->VectorMultiply(vertPos);
        vertPos = _projectionMat->VectorMultiply(vertPos);
        //Perform perspective division
        //std::cout << "X: " << vertPos.x << " Y: " << vertPos.y << " Z: " << vertPos.z << " W: " << vertPos.w << std::endl;
        vertPos.x /= vertPos.w;
        vertPos.y /= vertPos.w;
        vertPos.z /= vertPos.w;
        vertPos.w /= vertPos.w;
        //Apply it to the model
        tempVert.x = vertPos.x;
        tempVert.y = vertPos.y;
        tempVert.z = vertPos.z;
        _model->SetVertex(i, tempVert);
    }
    std::cout << "Finished processing verts" << std::endl;
}

int main()
{
    GeomUtils* gUtil = new GeomUtils();
    RenderUtils* renUtil = new RenderUtils(1);

    Model* testModel = new Model("cc.obj", gUtil);

    Vertex testVertOne = Vertex(10, 0, 0, 0, 0);
    Vertex testVertTwo = Vertex(20, 10, 0, 0, 0);

    Image* testImage = new Image(WIDTH, HEIGHT);

    float aspect = (float)(1080.0f / 1920.0f);
    std::cout << "aspect: " << (float)aspect << std::endl;
    //Important world stuff
    Camera* cam = new Camera(
        vec3f(0.0f, 0.0f, 0.0f),
        0.0f,
        0.0f,
        90.0f,
        aspect,
        0.01f,
        2000.0f
    );

    std::cout << "Building matricies for camera and models" << std::endl;
    //Build the neccesary matricies
    mat4 viewMatrix = cam->GetViewMatrix();
    viewMatrix.PrintDebugInfo();
    std::cout << "view" << std::endl;
    mat4 projectionMatrix = cam->GetProjectionMatrix();
    projectionMatrix.PrintDebugInfo();
    std::cout << "Projection" << std::endl;
    mat4 modelviewMatrix = testModel->CreateModelviewMatrix(&viewMatrix);
    modelviewMatrix.PrintDebugInfo();
    std::cout << "Modelview" << std::endl;

    std::cout << "Applying transformations" << std::endl;
    //Manipulate the model using the matricies
    TransformModel(testModel, &modelviewMatrix, &projectionMatrix);

    std::cout << "Rendering..." << std::endl;
    std::cout << "Face count: " << testModel->GetFaceCount() << std::endl;
    for (int v = 0; v < testModel->GetFaceCount(); v++) {
        //std::cout << "Rendering face: " << v << std::endl;
        int index = v * 3;
        Vertex one = testModel->GetVertex(index);
        Vertex two = testModel->GetVertex(index+1);
        Vertex three = testModel->GetVertex(index+2);
        one += 1.0f;
        two += 1.0f;
        three += 1.0f;
        one.x *= WIDTH / 2.0f;
        one.y *= HEIGHT / 2.0f;
        two.x *= WIDTH / 2.0f;
        two.y *= HEIGHT / 2.0f;
        three.x *= WIDTH / 2.0f;
        three.y *= HEIGHT / 2.0f;
        renUtil->RenderLine(one, two, testImage);
        renUtil->RenderLine(two, three, testImage);
        renUtil->RenderLine(three, one, testImage);
    }
    /*int index = 204 * 3;
    Vertex one = testModel->GetVertex(index);
    Vertex two = testModel->GetVertex(index + 1);
    Vertex three = testModel->GetVertex(index + 2);
    one += 1.0f;
    two += 1.0f;
    three += 1.0f;
    one *= WIDTH / 2.0f;
    two *= WIDTH / 2.0f;
    three *= WIDTH / 2.0f;
    renUtil->RenderLine(one, two, testImage);
    renUtil->RenderLine(two, three, testImage);
    renUtil->RenderLine(three, one, testImage);*/
    std::cout << "Finished rendering. Now exporting..." << std::endl;    
    //renUtil->RenderLine(testVertOne, testVertTwo, testImage);
    //testImage->SetPixel(511, 511, RGB(255, 0, 0));

    //Flip image vertically and export
    testImage->FlipVertical();
    testImage->WriteImage("testExp.ppm");
}
