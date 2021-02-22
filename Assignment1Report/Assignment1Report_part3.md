# Branch 1
## Showing screenshots for rotating the model in the model frame in 360 degrees in increments of 45 degrees:
![0Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/0Degrees.JPG)
![45Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/45Degrees.JPG)
![90Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/90Degrees.JPG)
![135Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/135Degrees.JPG)
![180Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/180Degrees.JPG)
![225Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/225Degrees.JPG)
![270Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/270Degrees.JPG)
![315Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/315Degrees.JPG)
![360Degrees](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/360Degrees.JPG)
## ===================================================================
# Branch 2
## Choosing T1=Translate(105,-110,0) and T2= Rotate in Z in 66 Degrees On the cow model
### first we show the screenshot displaying T1 in model frame and T2 in world frame
![T1MT2W](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1ModelT2World.JPG)
### Now displaying the screenshot where T1 is in world frame and T2 is in model frame
![T1WT2M](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1WorldT2Model.JPG)
## ===================================================================
# Branch 3
## The bellow picture shows the normals per face(in purple) and normals per vertex (in dark pink) and the bounding box (in red) without any transformations applied
![BBNFNV](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BBNFNV.JPG)
## The bellow picture displays the model after apllying translation in x and rotation in y and x
![BBNFNV_transformed](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BBNFNV_transformed.JPG)
## ===================================================================
# Branch 4
## First of all, we will show orthographic projection with low zoom and then with high zoom (orhto width is the parameter of the zooming).
### How i implemented the zooming parameter: given a new ortho width by the user we must maintain the original aspect ratio so what we do is:
### `new_width = ortho_width `
### `new_height = ortho_width/aspectRatio`
### and ofcourse `left= -new_width/2 , right= new_width/2 , top=new_height/2 , bottom=-new_height/2`;
 *In all the pictures the bounding box is beeing drawn in red in order to see the difference between the orthographic and perspective.*
![OrthoWide](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/OrhtoWide.png)
![OrthoZoom](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ZoomingOrhto.png)
## Now We will see the perspective projection first with low zoom then with high zoom.
![PerspectiveWide](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PerspectiveWide.png)
![PerspectiveZoom](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/PerspectiveZoom.png)
##### If we take a look at the bounding box we would see the difference between the orthographic projection and the perspective which is in the perspective we can see what happens on the z axis. for example applying TranslateModel(0,0,200) in the model frame on perspective projection would show us that the object is "getting closer",while on the other side applying the same transformation on the orthographic projection wouldn't show us anything.
## ===================================================================
# Branch 5
*The Axes are: X drawn in white and Y drawn in yello.*
#### First of all zooming is being done on the model by changing the fovy value. Then we translate the camera in Z. this difference is because the camera's location is on 0.2,0.2,0.2 which means eye=(0.2, 0.2, 0.2) but if we change it to (0,0,0.5) then there is no difference between translation in z axis and zooming using the fovy.
![Zooming](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ZoomingPer.JPG)
![Distance](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/DistancePer.JPG)
## ===================================================================
# Branch 6
### First we show a picture of translating(55,44,0) in camera frame then rotate(0,0,66) in world frame then Translating in world frame and rotating in camera frame both with the same parameters
![Camera_World](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1CameraT2World.JPG)
![World_Camera](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/T1WorldT2Camera.JPG)
## ===================================================================
# Branch 7
![SetCamera](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/SetCamera(aaa).JPG)
## ===================================================================
# Branch 9
#### First picture is the starting case and the second is after resizing the window shows how the model maintains its aspect ratio and stays in the center of the screen
#### *But I'am not sure whether you want the model to get bigger/smaller and maintain it's location in the screen or you want just the window to get smaller or bigger and the model in it's same size and location so i have pictures with both situations*
#### Currently in the code I have the last picture which is the model keeps its location and size no matter the window size.
#### Did that because I want the bonus and that is my last submission at 30/11/2020 21:43 and I dont have a definite answer.
![BeforeResize](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/BeforeResize.png)
![AfterResize](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/AfterResize.png)
![AfterResizeOption2](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/afterResizingOption2.png)
## ===================================================================
# Branch 10
#### A small description about the GUI and a screenshot:
In the picture shown down low showing the camera window which allows the user to move between Perspective projection or orthographic projection and if orthogrphic is chosen then the first sliderFloat would be the ortho width (now I paid attention to the typo but will change it in the code) which if changed it changes the height with it to maintain the aspectratio, if projection was chosen the sliderfloat changes to control over the fovy
A tree node which opens to control the values of eye at and up for the lookat function and applied only after clicking the look at button.
Then a tree node for the camera transformations where the user can choose whether to do a local or world transformation then translation or rotation and apply the appropriate values(rotation angles for x,y,z).
The second window is choosing the colors for the model,normals per face,normals per vertex,bounding box and background color.
The last window is the transformations for the model which starts by choosing world or local transformation then the transformation type and appropriate values.
The transformations are done incrementally, this means when moving one of the sliders the transformation is applied.
A reset button which resets the model's transformations and ofcourse buttons for showing or hiding the bounding box,normals per face,normals per vertes.
![GUI](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/GuiPic.png)
