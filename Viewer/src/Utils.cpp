#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils.h"


glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	ModelParameters model;
	glm::vec3 Vertex, VertexNormal;
	std::ifstream ifile(filePath.c_str());
	float Min_X = FLT_MAX, Max_X = FLT_MIN, Max_Y = FLT_MIN, Min_Y = FLT_MAX, Max_Z = FLT_MIN, Min_Z = FLT_MAX;
	//parameters to assign a scaling values
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			Vertex = Utils::Vec3fFromStream(issLine);
			if (Vertex.x > Max_X)
				Max_X = Vertex.x;
			if (Vertex.x < Min_X)
				Min_X = Vertex.x;

			if (Vertex.y > Max_Y)
				Max_Y = Vertex.y;
			if (Vertex.y < Min_Y)
				Min_Y = Vertex.y;

			if (Vertex.z > Max_Z)
				Max_Z = Vertex.z;
			if (Vertex.z < Min_Z)
				Min_Z = Vertex.z;

			model.vertices.push_back(Vertex);
		}
		else if (lineType == "vn")
		{
			VertexNormal = Utils::Vec3fFromStream(issLine);
			model.normals.push_back(VertexNormal);
		}
		else if (lineType == "vt")
		{
			model.textureCoords.push_back(Utils::Vec2fFromStream(issLine));
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			model.faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	model.modelName=Utils::GetFileName(filePath);
	model.leftTopNear = glm::vec4(Min_X, Max_Y, Max_Z, 1);
	model.rightTopNear = glm::vec4(Max_X, Max_Y, Max_Z, 1);
	model.leftTopFar = glm::vec4(Min_X, Max_Y, Min_Z, 1);
	model.rightTopFar = glm::vec4(Max_X, Max_Y, Min_Z, 1);
	model.leftBottomNear = glm::vec4(Min_X, Min_Y, Max_Z, 1);
	model.rightBottomNear = glm::vec4(Max_X, Min_Y, Max_Z, 1);
	model.leftBottomFar = glm::vec4(Min_X, Min_Y, Min_Z, 1);
	model.rightBottomFar = glm::vec4(Max_X, Min_Y, Min_Z, 1);
	model.modelName = GetFileName(filePath);
	model.modelCenter=glm::vec3((Max_X + Min_X) / 2, (Max_Y + Min_Y) / 2, (Min_Z + Max_Z) / 2);
	return std::make_shared<MeshModel>(model);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}
