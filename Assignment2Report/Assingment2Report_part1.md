# Filling the triangles without transforming the model
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ColorBuffer_Banana.JPG)
# Filling the triangles after translating rotating the model
## We can easily see the triangles overlapping.
 ![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ColorBuffer_BananaTransformed.JPG)
# comaprision between the first picture and another one after implementing the z_buffer
## We can see more "strictness" on the edges of each triangle which is not found in the first picture 
Without the Z_buufer       |  With the Z_buffer
:-------------------------:|:-------------------------:
![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ColorBuffer_Banana.JPG)|  ![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ZBuffer_Banana.JPG)
## The following pictures are for the bunny comparision between implementing the zbuffer and without it.
## we can see the difference in the tail of the bunny which can clearly be seen when using the zbuffer and without it it looks like its behind the rabbit
## and the number of triangles decreases because there is no overlapping.
Without the Z_buufer       |  With the Z_buffer
:-------------------------:|:-------------------------:
![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Bunny_ColorBuffer.JPG)|  ![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/Bunny_ZBuffer.JPG)

# Now this picture after transforming the model with the same transformations as before but with the zbuffer no triangles overlap.
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/ZBuffer_BananaTransformed.JPG)
# Now we will show the Cow model being colored in grayscale
## It's obvious that two of the cow's legs are dark colored because they are the "far" legs
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/GrayScale_Cow.JPG)
# applying camera rotations transformation
![GIF1](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/GrayScale_CowTransformed.JPG)
