#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <iostream>


class CubeMap
{
public:
	CubeMap(std::vector<std::string> faces);
	GLuint GetskyboxVAO() const;
	GLuint cubemapTexture;

private:
	GLuint skyboxVAO;
	GLuint skyboxVBO;
};

