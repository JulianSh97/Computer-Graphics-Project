## InitOpenGLRendering():
### - What happens here is that it prepares the screen for us to draw on it, but at first we were not using openGL so all the rasterization and fragmentation were made by us. so what really happens is that the InitOpenGLRendering function prepares two triangles that stretch over the screen and then we used to draw on the color buffer which is a type of an image, and then it was coverted as a texture for the two triangles that InitOpenGLRendering() made and drawn on the screen. 
## - Here we are showing the new MeshModel constructor which pushes the vertices,normals,Bounding box vertices, and faces normals into vectors then generates OpenGL buffers and binds them using VAOs and VBOs.
![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/MeshModelConstructor1.JPG)
![](https://github.com/HaifaGraphicsCourses/computergraphics2021-f-r-i-e-n-d-s/blob/master/Images/MeshModelConstructor2.JPG)
## Now displaying the vertex shader code:
```
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}
```
## The next is the fragment shader code:
```
#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(1.f,0.f,0.f,1.f);
}
```
## Here is the new Render code which replaces the whole Render Function
```
int cameraCount = scene.GetCameraCount();

	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		std::shared_ptr<Camera> camera = scene.GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel->GetTransformation());
			colorShader.setUniform("view", camera->GetLookAt() * camera->GetC_inv());
			colorShader.setUniform("projection", camera->GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);

			// Set 'texture1' as the active texture at slot #0
			//texture1.bind(0);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
			// Unset 'texture1' as the active texture at slot #0
			//texture1.unbind(0);

			//colorShader.setUniform("color", glm::vec3(0, 0, 0));

			// Drag our model's faces (triangles) in line mode (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
		}
	}
```
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
