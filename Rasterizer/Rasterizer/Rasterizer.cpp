// Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Model.h"
#include "ImageUtil.h"
#include "RenderUtils.h"
#include "Camera.h"
#undef main

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
    //Create the basic rendering tools needed
    GeomUtils* gUtil = new GeomUtils();
    RenderUtils* renUtil = new RenderUtils(1);

    //Create the models to be rendered and transform them
    Model* testModel = new Model("test.obj", gUtil);
    Material* modelMat = new Material(
        "inputFiles/textures/checker.png",
        "inputFiles/textures/test.png",
        "inputFiles/textures/test.png"
    );

    vec3f pos = vec3f(0.0f, 0.0f, 0.0f);
    vec3f rot = vec3f(0.0f, 0.0f, 0.0f);
    vec3f sca = vec3f(1.0f, 1.0f, 1.0f);
    testModel->SetPosition(pos);
    testModel->SetRotation(rot);
    testModel->SetScale(sca);


    //Create the image object
    Image* testImage = new Image(WIDTH, HEIGHT);

    float viewportModifier = 0.9f;
    float nearClipping = 1.0f;
    float farClipping = 1000.0f;

    //Create the camera for perspective rendering
    Camera* cam = new Camera(
        vec3f(113.455f, 62.9185f, 14.8309f),
        -0.8f,
        107.0f,
        39.6f,
        0.8716f * viewportModifier,
        0.4903f * viewportModifier,
        nearClipping,
        farClipping
    );

    std::cout << "Building matricies for camera and models" << std::endl;
    //Build the neccesary matricies
    //View and the inverse of it
    mat4 viewMatrix = cam->GetViewMatrix();
    viewMatrix.PrintDebugInfo();
    std::cout << "view" << std::endl;
    mat4 viewMatrixInverse = viewMatrix.Inverse();
    viewMatrixInverse.PrintDebugInfo();
    std::cout << "view inverse" << std::endl;

    //Projection
    mat4 projectionMatrix = cam->GetProjectionMatrix();
    projectionMatrix.PrintDebugInfo();
    std::cout << "Projection" << std::endl;

    //Modelview
    mat4 modelviewMatrix = testModel->CreateModelviewMatrix(&viewMatrix, cam->GetPosition());
    modelviewMatrix = modelviewMatrix.Inverse();
    modelviewMatrix.PrintDebugInfo();
    std::cout << "Modelview" << std::endl;

    std::cout << "Applying transformations" << std::endl;
    //Manipulate the model using the matricies
    TransformModel(testModel, &modelviewMatrix, &projectionMatrix);

    std::cout << "Culling faces from model" << std::endl;
    testModel->CullFaces();

    //Start rendering
    std::cout << "Rendering..." << std::endl;
    std::cout << "Face count: " << testModel->GetFaceCount() << std::endl;
    //Create the z buffer in the render utility
    renUtil->SetupZBuffer();
    
    //Start rendering faces
    for (int v = 0; v < testModel->GetFaceCount(); v++) {
        //std::cout << "Rendering face: " << v << std::endl;
        int index = v * 3;
        Vertex one = testModel->GetVertex(index);
        Vertex two = testModel->GetVertex(index+1);
        Vertex three = testModel->GetVertex(index+2);
        if ((one.z < farClipping && one.z > nearClipping) || (two.z < farClipping && two.z > nearClipping) || (three.z < farClipping && three.z > nearClipping)) {
            one += 1.0f;
            two += 1.0f;
            three += 1.0f;
            one.x *= WIDTH / 2.0f;
            one.y *= HEIGHT / 2.0f;
            two.x *= WIDTH / 2.0f;
            two.y *= HEIGHT / 2.0f;
            three.x *= WIDTH / 2.0f;
            three.y *= HEIGHT / 2.0f;
            renUtil->RenderTrianlge(one, two, three, testImage, modelMat);
        }
    }
    std::cout << "Finished rendering. Now exporting..." << std::endl;    
    //renUtil->RenderLine(testVertOne, testVertTwo, testImage);
    //testImage->SetPixel(511, 511, RGB(255, 0, 0));

    //Flip image vertically and export
    testImage->FlipVertical();
    testImage->WriteImage("testExp.ppm");


    //Cleanup

    delete gUtil;
    delete renUtil;
    delete testModel;
    delete modelMat;
    delete testImage;
    delete cam;
    return 0;
}
