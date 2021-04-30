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

vec3f TransformVert(vec3f& _vert, mat4& _modelviewMat, mat4& _projectionMat) {
    vec4f vertPos = vec4f(_vert.x, _vert.y, _vert.z, 1.0f);
    //Matrix-Vector multiplication with the modelview matrix, and then the projection matrix
    vertPos = _modelviewMat.VectorMultiply(vertPos);
    vertPos = _projectionMat.VectorMultiply(vertPos);
    //Perform perspective division
    //std::cout << "X: " << vertPos.x << " Y: " << vertPos.y << " Z: " << vertPos.z << " W: " << vertPos.w << std::endl;
    vertPos.x /= vertPos.w;
    vertPos.y /= vertPos.w;
    vertPos.z /= vertPos.w;
    vertPos.w /= vertPos.w;
    return vec3f(vertPos.x, vertPos.y, vertPos.z);
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

    float viewportModifier = 1.0f;
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
    
    for (int v = 0; v < testModel->GetFaceCount() * 3; v++) {
        Vertex one = testModel->GetVertex(v);
        if ((one.x < -1 || one.x > 1) && (one.y < -1 || one.y > 1) && (one.z < -1 || one.z > 1)) {
            std::cout << "Non NDC detected at index: " << v << std::endl;
        }
        
    }

    //Start rendering faces
    for (int v = 0; v < testModel->GetFaceCount(); v++) {
        //std::cout << "Rendering face: " << v << std::endl;
        int index = v * 3;
        //Get verticies and uv texture coordinates together
        const Vertex vert0 = testModel->GetVertex(index);
        const Vertex vert1 = testModel->GetVertex(index + 1);
        const Vertex vert2 = testModel->GetVertex(index + 2);

        vec3f v0(vert0.x, vert0.y, vert0.z);
        vec3f v1(vert1.x, vert1.y, vert1.z);
        vec3f v2(vert2.x, vert2.y, vert2.z);

        vec3f rasterv0 = TransformVert(v0, modelviewMatrix, projectionMatrix);
        vec3f rasterv1 = TransformVert(v1, modelviewMatrix, projectionMatrix);
        vec3f rasterv2 = TransformVert(v2, modelviewMatrix, projectionMatrix);

        rasterv0.z = 1 / rasterv0.z;
        rasterv1.z = 1 / rasterv1.z;
        rasterv2.z = 1 / rasterv2.z;

        vec2f st0(vert0.u, vert0.v);
        vec2f st1(vert1.u, vert1.v);
        vec2f st2(vert2.u, vert2.v);
        st0 *= rasterv0.z;
        st1 *= rasterv1.z;
        st2 *= rasterv2.z;

        //Calculate the bounding box of the triangle
        float xMin = Min3(rasterv0.x, rasterv1.x, rasterv2.x);
        float yMin = Min3(rasterv0.y, rasterv1.y, rasterv2.y);
        float xMax = Max3(rasterv0.x, rasterv1.x, rasterv2.x);
        float yMax = Max3(rasterv0.y, rasterv1.y, rasterv2.y);

        //Check if the triangle is outside of the screen
        if (xMin > WIDTH - 1 || xMax < 0 || yMin > HEIGHT - 1 || yMax < 0) {
            continue;
        }

        //Define the bounds of the raster triangle
        uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xMin)));
        uint32_t x1 = std::min(int32_t(WIDTH) - 1, (int32_t)(std::floor(xMax)));
        uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(yMin)));
        uint32_t y1 = std::min(int32_t(HEIGHT) - 1, (int32_t)(std::floor(yMax)));

        //Area for the triangle
        vec3f AB = (rasterv1 - rasterv0);
        vec3f AC = (rasterv2 - rasterv0);
        float area = AB.Cross(&AC).Length() / 2.0f;

        //Loop through each pixel of the bounding box
        for (int y = 0; y <= y1; y++) {
            for (int x = 0; x <= x1; x++) {
                vec3f pixelSample(x + 0.5f, y + 0.5f, 0);

                //Calculate weightings for uvs
                vec3f QA = rasterv0 - pixelSample;
                vec3f QB = rasterv1 - pixelSample;
                vec3f QC = rasterv2 - pixelSample;
                float w0 = QB.Cross(&QC).Length() / 2.0f;
                float w1 = QA.Cross(&QC).Length() / 2.0f;
                float w2 = QA.Cross(&QB).Length() / 2.0f;


            }
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
