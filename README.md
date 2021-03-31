# cmp5359-cg-milestones-20-21-JamesPhilipPrice
cmp5359-cg-milestones-20-21-JamesPhilipPrice created by GitHub Classroom

Trello link: https://trello.com/b/EbUSjTMb/compute-graphics

# Project start: 23/02/21
I have created the initial project files from visual studio and have setup a trello board to keep track of tasks. I still need to finish a scene to renderer because I couldn't find high enough quality assets.

## Inspiration
I because heavily inspired by my obsession with the Evangelion Movies and the Neon Genesis: Evangelion anime so I wanted to try to create a simple but beautiful looking render of Eva unit 01 in a moody and dense environment surrounded by buildings.

# Timeline
## 23/02/21 - Project start
Initial visual studio project creation and upload of basic blender scene.

## 25/02/21 - Started work on Image class and colour struct
Created the image class with the ability to write images and change pixel colours.

## 26/02/21 - Progress on blender scene
Worked on building models and putting stuff together.

## 02/03/21 - Get the render utility class working on models
Have a wireframe render of a testing model from module labs.
Currently an issue with offsetting the model and need to look back into the vector transformations lectures and math support.

## 03/03/21 - Got blender scene to a decent state
I would be happy at this point to use this scene as-is and export it to render.
<img src="Renderings/Blender_Render_Cycles.png?raw=true"/>

## 12/03/21 - Started to work on matricies
I have begun to research into implementing matricies, vector2/3/4 and matrix transformations for manipulating models.

## 18/03/21 - The matrix transformations have been mostly implemented
There are some glaring bugs in the matrix transformations code that is making it impossible to render anything. I will look into this in tommorows lesson hopefully.

## 19/03/21 - The bugs have been fixed
The bugs in the matrix transformation code has been fixed and now models can ben rendered correctly.

## 25/03/21 - Odd issue found with distortion only when moveing or rotationg the camera
I plan to work on the actual rendering functionality of the project to cull faces that are out of view as it may be a cause of the issue
<img src="Documentation_assets/HugeHead.png?raw=true"/>

## 26/03/21 - FIXED: camera matrix issues are now solved
No abnormal/unexpected distortions occur

## 30/03/21 - Got solid triangles rendering without any shading
I haven't implemented normals for shading yet and will work towards implementing texturing
<img src="Documentation_assets/no_normals_solid.png?raw=true"/>

## 31/03/21 - Implemented very basic texture system (with faults: no UV mapping)
I still need to implement UV controlled texturing. At the moment the models are only textured using a single pixel of a texture, to prove as proof of concept that the texturing works.
<img src="Documentation_assets/TextureTest.png?raw=true"/>
