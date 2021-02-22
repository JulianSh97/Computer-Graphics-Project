#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	int GetVertexIndex(int index) const;
	int GetNormalIndex(int index) const;
	int GetTextureIndex(int index) const;
	void Face::PrintFace()const;
	glm::vec3 GetNormal();
	glm::vec3 GetCenter();
	void SetNormal(glm::vec3 normal);
	void SetCenter(glm::vec3 center);

private:
	std::vector<int> vertex_indices_;
	std::vector<int> normal_indices_;
	std::vector<int> texture_indices_;
	std::vector<glm::vec3> FaceVertices;
	glm::vec3 FaceNormal;
	glm::vec3 FaceCenter;
};