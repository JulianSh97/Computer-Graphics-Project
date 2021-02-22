#pragma once
#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"
#include "ModelParameters.h"
#include "Transformations.h"

class Utils
{
public:
	static glm::vec3 Vec3fFromStream(std::istream& issLine);
	static glm::vec2 Vec2fFromStream(std::istream& issLine);
	static std::shared_ptr<MeshModel> LoadMeshModel(const std::string& filePath);
	static std::string GetFileName(const std::string& filePath);
};
