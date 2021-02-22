#pragma once
#include <glm/glm.hpp>
#include <string>
#include <Face.h>

class ModelParameters
{
public:
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::string modelName;
	glm::vec4 leftTopNear;
	glm::vec4 rightTopNear;
	glm::vec4 leftTopFar;
	glm::vec4 rightTopFar;
	glm::vec4 leftBottomNear;
	glm::vec4 rightBottomNear;
	glm::vec4 leftBottomFar;
	glm::vec4 rightBottomFar;
	glm::mat4x4 preTransformation;
	glm::mat4x4 NormalsPreTransformation;
	glm::vec3 modelCenter;
};

