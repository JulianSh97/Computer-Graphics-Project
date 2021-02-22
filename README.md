# Computer-Graphics-Project
### By using C++, ImGui, OpenGL,my project was loading a mesh model as an obj file then displaying it as a wire-frame model and applying transformations on it, fill the model with a color then implementing the Z_buffer algorithm to remove hidden surface,and now currently working on converting the project into working on the GPU instead of the CPU using OpenGL and GLSL. here is the report for the final part of the project with some pictures describing the results:

## Showing pictures that show the model drawn once in wireframe and once filled
![WireFrame](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/WireFrameBeethoven.JPG)
![Filled](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/FilledBunny.JPG)

## For the next step we will show the phong shading with different type of lights any both light types(parallel and point),different colors,and different view points

### First of all we are gonna start with the beetle model that was provided by my lecturer.
#### Due to better visuality the windows has been hidden, but in short the projection here is orthographic projection, both the light and the model passed by local and world transformations.
![car1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Car1.JPG)
![Car2](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Car2.JPG)
## Now showing the sphere model with two lights,the one on the right is a parallel light which shows its direction by the lines, and the second one is a point light source which is behind the sphere.
![Sphere](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Sphere.JPG)
## Last but not least, this model is the one provided by the skelton code at the start of the semester, it's the Feline i know i wasn't supposed to show it here but i kind of liked it :) so... here it is.
![Felline](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Felline.JPG)
## Oh Wait,now we say last but not least :)
### Beethoven: "That banana is mine!!"
![HungryBethoven](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/HungryBethoven.JPG)

## Next step is showing models with texture mapping.
### First we're gonna start with the teapot using plane projection and we can see the effect that's produced around the part that pours the tea (I don't know what they call that part).
![Teapot](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Teapot1.JPG)

### Now you can see some models that have vt coordinates in their obj file, here is maybe the most famous Jim Carrey character "The Mask" once shown in perspective projection and once in orthographic projection.
![MaskPerspective](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Mask.JPG)
![Mask](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/MaskOrtho.JPG)

### Ok Now the most exciting picture the solar system,I generated this by adding multiple spheres models and for each one using different texture map. Oh wait there is a star too!
![SolarSystem](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/solarSystem.JPG)

## Now I'am showing two images of the crate model with environment mapping with a sea and mountains cube map.
![Cube1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Crate1.JPG)
![Cube2](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Crate2.JPG)
### And that's our teapot when it traveled to Yokohama after taking the Covid-19 vaccine :)
![Teapot](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/TeapotYokohama.JPG)
