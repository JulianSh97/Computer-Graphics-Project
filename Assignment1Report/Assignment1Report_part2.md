# Printing the faces,vertices and normals of the banana model:
===============================
![Banana Model Faces](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PrintFaces.JPG)
![Banana Model Vertices](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PrintVertices.JPG)
![Banana Model Normals](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PrintNormals.JPG)
===============================

# Transformation description:
===============================
#### -While loading the vertices from the .obj file i calculated the minimum value and maximum value of each dimension (x,y,z).
#### -Make 4x4 Scaling matrix=diag 3x3(330 / Max(max_x,max_y,max_z)) 
#### -Make 4x4 Translation matrix with tx=|min_x| ty=|min_y| tz=|min_z| 
#### -For each Face:
#### -Get face's vertices :v1 v2 v3
#### -Apply the Transformation=scaling * Translation on v1 v2 v3 after moving to the homogenous coordinates
#### -Get back to 3d coordinates (v1=(v1.x/v1.w,v1.y/v1.w,v1.z/v1.w))
#### -draw lines from v1 to v2 , v2 to v3 , v1 to v3 depending on the x and y coordinates only.
===============================

# Drawing the model in Render
===============================
![Cow Model](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/CowModel.JPG)
===============================

# Showing the GUI and explaining how it works
===============================
#### The GUI is shown in the pictures bellow before loading a model and after loading a model.
#### The user can pick World or Local transformation and the transformation type(scale,translate,rotate)
#### With the appropriate parameters can be chosen with sliderInt or sliderFloat
#### The transformation is done immediatly 
![Before Loading](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BeforeLoading.JPG)
![After Loading](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/AfterLoading.JPG)
===============================

# Showing th deffirence between Translating in the world frame then Rotating in the model frame Vs. translating in the Model frame then Rotating in the world frame.
===============================
#### The parameters are Translation in X with parameter 200 and Rotation arround Y with 120 degree angle.
#### We can obviously see that there is a diffrence in the placement of the model.
#### This happens because of the fact that rotating around the model frame rotates the model in it's own placement 
#### while rotating in the world frame it rotates the model arround the axis of the world which is place is in the left bottom corner of the screen.
![Translate Local & Rotate World](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/TranslateLocal_RotateWorld.JPG)
![Translate World & Rotate Local](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/TranslateWorld_RotateLocal.JPG)
===============================
