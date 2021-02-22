#pragma once
#include <glm/glm.hpp>

class Transformations
{
public:
	Transformations();
	virtual ~Transformations();
	static glm::mat4x4 ScalingTransformation(float sx, float sy, float sz);
	static glm::mat4x4 TranslationTransformation(float tx, float ty, float tz);
	static glm::mat4x4 XRotationTransformation(float angle);
	static glm::mat4x4 YRotationTransformation(float angle);
	static glm::mat4x4 ZRotationTransformation(float angle);
	static glm::mat4x4 Identity4X4Matrix();
};

