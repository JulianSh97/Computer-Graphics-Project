#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include <Light.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#define PHONGSHADING 999
#define WIREFRAME 990
#define TEXTURED 900
#define REFLECTION 909
#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	//InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
	delete[] Z_Buffer;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::vec3& p1, const glm::vec3& p2, glm::vec3& color)
{
	// TODO: Implement bresenham algorithm
	int x = p1.x, y = p1.y, ReflectFlag = 0, LoopVar = p2.x;
	double deltaP = (double)(p2.x - p1.x), deltaQ = (double)(p2.y - p1.y);
	double a = deltaQ / deltaP, e = (-1) * deltaP;
	if (a == -1)
	{
		if (deltaP > 0)
		{
			while (x < p2.x)
			{
				PutPixel(x, y, color);
				x = x + 1;
				y = y - 1;
			}

		}
		else
		{
			while (x > p2.x)
			{
				PutPixel(x, y, color);
				x = x - 1;
				y = y + 1;
			}
		}
	}
	else
		if (deltaQ == 0)
		{
			if (deltaP > 0)
				while (x <= p2.x)
				{
					PutPixel(x, y, color);
					x = x + 1;
				}
			else
			{
				while (x >= p2.x)
				{
					PutPixel(x, y, color);
					x = x - 1;
				}
			}
		}
		else
			if (deltaP == 0)
			{
				if (deltaQ > 0)
					while (y <= p2.y)
					{
						PutPixel(x, y, color);
						y = y + 1;
					}
				else
					while (y >= p2.y)
					{
						PutPixel(x, y, color);
						y = y - 1;
					}
			}
			else
				if (a > 1 && !(a < -1)) //switch x and y
				{
					x = p1.y;
					y = p1.x;
					deltaQ = deltaP;
					deltaP = (p2.y - p1.y);
					if (deltaQ > 0 && deltaP > 0) {
						while (x <= p2.y)
						{
							if (e > 0)
							{
								y = y + 1;
								e = e - (2 * deltaP);
							}
							PutPixel(y, x, color);
							x = x + 1;
							e = e + (2 * deltaQ);
						}
					}
					else
						DrawLine(p2, p1, color);
				}
				else
					if ((a > -1 && a < 0) && !(a < -1)) //reflect 
					{
						if (deltaQ < 0) {
							deltaQ = (-1) * deltaQ;
							while (x <= p2.x)
							{
								if (e > 0)
								{
									y = y - 1;
									e = e - (2 * deltaP);
								}
								PutPixel(x, y, color);
								x = x + 1;
								e = e + (2 * deltaQ);
							}
						}
						else
						{
							deltaP = (-1) * deltaP;
							while (x >= p2.x)
							{
								if (e > 0)
								{
									if (y < p2.y)
										y = y + 1;
									e = e - (2 * deltaP);
								}
								PutPixel(x, y, color);
								x = x - 1;
								e = e + (2 * deltaQ);
							}
						}
					}
					else
						if (a < -1)//swtich and reflect
						{
							x = p1.y;
							y = p1.x;
							deltaQ = deltaP;
							deltaP = (p2.y - p1.y);
							LoopVar = p2.y;
							if (deltaQ < 0) {
								deltaP = (-1) * deltaP;
								while (x <= LoopVar)
								{
									if (e < 0)
									{
										y = y - 1;
										e = e - (2 * deltaP);
									}
									PutPixel(y, x, color);
									x = x + 1;
									e = e + (2 * deltaQ);
								}
							}
							else
								DrawLine(p2, p1, color);
						}
						else
						{
							if (deltaP > 0 && deltaQ > 0) {
								while (x <= p2.x)
								{
									if (e > 0)
									{
										y = y + 1;
										e = e - (2 * deltaP);
									}
									PutPixel(x, y, color);
									x = x + 1;
									e = e + (2 * deltaQ);
								}
							}
							else
								DrawLine(p2, p1, color);
						}
}

void Renderer::CreateBuffers(const int w, const int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	Z_Buffer = new float[w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
	ClearZ_Buffer();
}

void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader_color.glsl", "fshader_color.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearZ_Buffer()
{
	MinZ = FLT_MAX;
	MaxZ = FLT_MIN;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			if (i < 0) return; if (i >= viewport_width_) return;
			if (j < 0) return; if (j >= viewport_height_) return;
			Z_Buffer[Z_INDEX(viewport_width_, i, j)] = FLT_MAX;
			color_buffer_[INDEX(viewport_width_, i, j, 0)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 1)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 2)] = 0.f;
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(Scene& scene)
{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	glm::vec3 AmbientColor[10];
	glm::vec3 DiffuseColor[10];
	glm::vec3 SpecularColor[10];
	glm::vec3 lightsPositions[10];
	glm::mat4 lightTransformations[10];
	glm::vec3 lightsDirections[10];
	float lightsTypes[10];
	int LightsNumber;
	int cameraCount = scene.GetCameraCount();

	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		std::shared_ptr<Camera> camera = scene.GetActiveCamera();
		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);
			// Activate the 'colorShader' program (vertex and fragment shaders)
			for (int i = 0; i < scene.GetLightCount(); i++)
			{
				std::shared_ptr<Light> currentLight = scene.GetLight(i);
				AmbientColor[i] = currentLight->GetAmbientLightColor();
				DiffuseColor[i] = currentLight->GetDiffuseLightColor();
				SpecularColor[i] = currentLight->GetSpecularLightColor();
				lightsPositions[i] = currentLight->GetLightPosition();
				lightTransformations[i] = currentLight->GetWorldTransformation() * currentLight->GetLocalTransformation();
				lightsDirections[i] = normalize(currentLight->GetLightDirection());
				if (currentLight->GetLightType() == LightType::PARALLEL)
					lightsTypes[i] = 1;
				else
					lightsTypes[i] = 0;
			}
			if (scene.GetenvironmentMap())
			{
				colorShader.use();
				colorShader.setUniform("reflection", true);
				colorShader.setUniform("skybox", 0);
				colorShader.setUniform("material.textureMap", 0);
				colorShader.setUniform("model", currentModel->GetTransformation());
				colorShader.setUniform("DrawLight", false);
				colorShader.setUniform("view", camera->GetLookAt() * camera->GetC_inv());
				colorShader.setUniform("projection", camera->GetProjectionTransformation());
				colorShader.setUniform("ColorMethod", REFLECTION);

				currentModel->texture1.bind(0);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
				currentModel->texture1.unbind(0);

				skyboxShader.use();
				skyboxShader.setUniform("skybox", 0);
				glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
				skyboxShader.use();
				skyboxShader.setUniform("view", camera->GetLookAt() * camera->GetC_inv());
				skyboxShader.setUniform("projection", camera->GetProjectionTransformation());
				// skybox cube
				glBindVertexArray(scene.cubemap->GetskyboxVAO());
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP,scene.cubemap->cubemapTexture);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				glDepthFunc(GL_LESS);
			}
			// Set the uniform variables
			colorShader.use();
			colorShader.setUniform("reflection", false);
			colorShader.setUniform("AmbientColor", AmbientColor, scene.GetLightCount());
			colorShader.setUniform("DiffuseColor", DiffuseColor, scene.GetLightCount());
			colorShader.setUniform("SpecularColor", SpecularColor, scene.GetLightCount());
			colorShader.setUniform("lightsPositions", lightsPositions, scene.GetLightCount());
			colorShader.setUniform("lightTransformations", lightTransformations, scene.GetLightCount());
			colorShader.setUniform("lightsTypes", lightsTypes, scene.GetLightCount());
			colorShader.setUniform("lightsDirections", lightsDirections, scene.GetLightCount());
			colorShader.setUniform("LightsNumber", scene.GetLightCount());
			colorShader.setUniform("eye",normalize(camera->GetEye()));
			colorShader.setUniform("model", currentModel->GetTransformation());
			colorShader.setUniform("DrawLight", false);
			colorShader.setUniform("view", camera->GetLookAt() * camera->GetC_inv());
			colorShader.setUniform("projection", camera->GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("material.diffuseColor", currentModel->GetDiffuseColor());
			colorShader.setUniform("material.specularColor", currentModel->GetSpecularColor());
			colorShader.setUniform("material.ambientColor", currentModel->GetAmbientColor());
			colorShader.setUniform("material.alpha", currentModel->GetAlpha());
			colorShader.setUniform("ColorMethod", currentModel->GetColorMethod());
			colorShader.setUniform("HasVt", currentModel->GetHasVt());

			// Set 'texture1' as the active texture at slot #0
			currentModel->texture1.bind(0);
			if (currentModel->GetColorMethod() == TEXTURED)
			{
				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}

			// Unset 'texture1' as the active texture at slot #0
			currentModel->texture1.unbind(0);

			if (currentModel->GetColorMethod() == WIREFRAME)
			{
				// Drag our model's faces (triangles) in line mode (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}

			if (currentModel->GetColorMethod() == PHONGSHADING)
			{
				// Drag our model's faces (triangles) in fill mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glBindVertexArray(currentModel->GetVAO());
				glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
				glBindVertexArray(0);
			}
		}
		if (scene.GetLightCount())
			DrawLights(scene);
	}
}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::SetViewportWidth(int w)
{
	viewport_width_ = w;
}

void Renderer::SetViewportHeight(int h)
{
	viewport_height_ = h;
}

glm::vec3 Renderer::RandColor()
{
	return glm::vec3(static_cast <float> (rand() / static_cast <float>(RAND_MAX)), static_cast <float> (rand() / static_cast <float>(RAND_MAX)), static_cast <float> (rand() / static_cast <float>(RAND_MAX)));
}

void Renderer::FogExists(Scene& scene)
{
	scene.SetFogStart(MinZ);
	scene.SetFogEnd(MaxZ);
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	glm::vec3 c;
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				c = glm::vec3(color_buffer_[INDEX(viewport_width_, i, j, 0)], color_buffer_[INDEX(viewport_width_, i, j, 1)], color_buffer_[INDEX(viewport_width_, i, j, 2)]);
				float vertexViewDistance = z;
				float fogFactor;
				if (scene.GetIsLinearFog())
				{
					if (!scene.GetActiveCamera()->GetIsOrthographic())
						int x = 3;
					fogFactor = (scene.GetFogEnd() - vertexViewDistance) / (scene.GetFogEnd() - scene.GetFogStart());
				}
				else
				{
					fogFactor = std::exp(-(vertexViewDistance * vertexViewDistance * scene.GetFogDensity() * scene.GetFogDensity()));
					if (fogFactor < 0 || fogFactor > 1)
						fogFactor = 1;
				}
				c = ((1 - fogFactor) * glm::vec3(0.4, 0.4, 0.4) + fogFactor * c);
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = c.x;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = c.y;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = c.z;
			}
		}
}

float Renderer::CalcArea(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
	return abs(((v2.x - v1.x) * (v3.y - v1.y) - (v3.x - v1.x) * (v2.y - v1.y)) / 2.0f);
}

glm::vec3 Renderer::CalcZ(glm::vec3& P, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& value1, const glm::vec3& value2, const glm::vec3& value3)
{
	float A;
	float A1 = CalcArea(P, v1, v2);
	float A2 = CalcArea(P, v1, v3);
	float A3 = CalcArea(P, v2, v3);
	A = A1 + A2 + A3;
	return ((A1 / A) * value3) + ((A2 / A) * value2) + ((A3 / A) * value1);
}

float Renderer::sign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Renderer::ptInTriangle(const glm::vec3& pt, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);
	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
	return !(has_neg && has_pos);
}

float Renderer::GetZ(int i, int j) const
{
	if (i < 0) return FLT_MAX; if (i >= viewport_width_) return FLT_MAX;
	if (j < 0) return FLT_MAX; if (j >= viewport_height_) return FLT_MAX;
	return Z_Buffer[Z_INDEX(viewport_width_, i, j)];
}

void Renderer::PutZ(int i, int j, float z)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	this->Z_Buffer[Z_INDEX(viewport_width_, i, j)] = z;
}

void Renderer::DrawLights(Scene& scene)
{
	float half_width = viewport_width_ / 2;
	float half_height = viewport_height_ / 2;
	glm::vec3 color = glm::vec3(1, 1, 1);
	glm::mat4x4 Lookat = scene.GetActiveCamera()->GetLookAt();
	glm::mat4x4 projectionTransformation = scene.GetActiveCamera()->GetProjectionTransformation();
	glm::mat4x4 ViewPortTransformation = Transformations::ScalingTransformation(half_width, half_height, 1) * Transformations::TranslationTransformation(1, 1, 1);
	glm::mat4x4 C_inv = scene.GetActiveCamera()->GetC_inv();
	for (int i = 0; i < scene.GetLightCount(); i++)
	{
		std::shared_ptr<Light> light = scene.GetLight(i);
		glm::mat4x4 transformation = light->GetWorldTransformation() * light->GetLocalTransformation();
		colorShader.use();

		// Set the uniform variables
		colorShader.setUniform("LightTransformation", transformation);
		colorShader.setUniform("DrawLight", true);
		if (light->GetLightType() == LightType::PARALLEL)
			colorShader.setUniform("lightType", 1);
		else
			colorShader.setUniform("lightType", 0);
		colorShader.setUniform("view", Lookat * C_inv);
		colorShader.setUniform("projection", projectionTransformation);
		colorShader.setUniform("material.textureMap", 0);
		if (light->GetLightType() == LightType::POINT)
		{
			glm::vec3 position = light->GetLightPosition();
			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(light->GetVao());
			glDrawArrays(GL_POINTS, 0, 1);
			glEnable(GL_PROGRAM_POINT_SIZE);
			glBindVertexArray(0);
		}
		else
		{
			glBindVertexArray(light->GetVao());
			glDrawArrays(GL_LINES, 0, 18);
			glBindVertexArray(0);
		}
	}
}

glm::vec3 Renderer::GetAmbientColor(const glm::vec3& Acolor, const glm::vec3& LightAcolor)
{
	glm::vec3 I_a(Acolor.x * LightAcolor.x, Acolor.y * LightAcolor.y, Acolor.z * LightAcolor.z);
	return I_a;
}

glm::vec3 Renderer::GetSpecularColor(glm::vec3& I, glm::vec3 n, const glm::vec3& eye, Light& light, const glm::vec3& Scolor)
{
	glm::vec3 temp_eye = normalize(eye);
	int alpha = 2;
	glm::vec3 temp = glm::vec3(Scolor.x * light.GetSpecularLightColor().x, Scolor.y * light.GetSpecularLightColor().y, Scolor.z * light.GetSpecularLightColor().z);
	glm::vec3 r = (2.f * glm::dot(-n, I) * n - I);
	float Power = (float)(std::pow(std::max(0.0f, glm::dot((r), (temp_eye))), alpha));
	glm::vec3 I_s(temp * Power);
	return I_s;
}

glm::vec3 Renderer::GetDiffuseColor(glm::vec3 normal, glm::vec3 I, Scene& scene,Light& light)
{
	glm::vec3 Dcolor = scene.GetActiveModel()->GetDiffuseColor();
	glm::vec3 temp(Dcolor.x * light.GetDiffuseLightColor().x, Dcolor.y * light.GetDiffuseLightColor().y, Dcolor.z * light.GetDiffuseLightColor().z);
	float IdotN = glm::dot(-(normal), I);
	return temp * IdotN;
}

void Renderer::ScanConvert_Grayscale()
{
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				float a = 1 / (MaxZ - MinZ);
				float b = -1 * a * MinZ;
				float gray = 1 - (a * z + b);
				color_buffer_[INDEX(viewport_width_, i, j, 0)] = gray;
				color_buffer_[INDEX(viewport_width_, i, j, 1)] = gray;
				color_buffer_[INDEX(viewport_width_, i, j, 2)] = gray;
			}
		}
}

void Renderer::FixColors(int coloring)
{
	for (int i = 0; i < viewport_width_; i++)
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Z_Buffer[Z_INDEX(viewport_width_, i, j)];
			if (z != FLT_MAX)
			{
				if (coloring)
				{
					if (color_buffer_[INDEX(viewport_width_, i, j, 0)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 0)] = 1.f;
					if (color_buffer_[INDEX(viewport_width_, i, j, 1)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 1)] = 1.f;
					if (color_buffer_[INDEX(viewport_width_, i, j, 2)] > 1.f) color_buffer_[INDEX(viewport_width_, i, j, 2)] = 1.f;
				}
				else
				{
					color_buffer_[INDEX(viewport_width_, i, j, 0)] /= 5;
					color_buffer_[INDEX(viewport_width_, i, j, 1)] /= 5;
					color_buffer_[INDEX(viewport_width_, i, j, 2)] /= 5;
				}
			}
		}
}

void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader_color.glsl", "fshader_color.glsl");
	skyboxShader.loadShaders("vshader_skybox.glsl", "fshader_skybox.glsl");
}

void Renderer::LoadTextures(const std::string& filePath)
{
	if (!texture1.loadTexture(filePath, true))
	{
		texture1.loadTexture(filePath, true);
	}
}