#pragma once
#include <istream>
#include "Face.h"
#include <iostream>

Face::Face(std::istream& issLine)
{
	vertex_indices_  = { 0,0,0 };
	normal_indices_  = { 0,0,0 };
	texture_indices_ = { 0,0,0 };

	char c;
	for (int i = 0; i < 3; i++)
	{
		issLine >> std::ws >> vertex_indices_.at(i) >> std::ws;
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> std::ws;
		if (issLine.peek() == '/')
		{
			issLine >> c >> std::ws >> normal_indices_.at(i);
			continue;
		}

		issLine >> texture_indices_.at(i);
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> normal_indices_.at(i);
	}
}

int Face::GetVertexIndex(int internal_index) const
{
	return vertex_indices_[internal_index];
}

int Face::GetNormalIndex(int internal_index) const
{
	return normal_indices_[internal_index];
}

int Face::GetTextureIndex(int internal_index) const
{
	return texture_indices_[internal_index];
}
void Face::PrintFace()const
{
	for (std::vector<int>::size_type i = 0; i != vertex_indices_.size(); i++) {
		std::cout << "Vertex Index: "<< vertex_indices_[i];
		std::cout << " Normal Index: " << normal_indices_[i];
		std::cout << " Texture Index: " << texture_indices_[i]<<"\t";
	}
	
}
glm::vec3 Face::GetNormal()
{
	return FaceNormal;
}

glm::vec3 Face::GetCenter()
{
	return FaceCenter;
}

void Face::SetNormal(glm::vec3 normal)
{
	FaceNormal = normal;
}

void Face::SetCenter(glm::vec3 center)
{
	FaceCenter = center;
}
