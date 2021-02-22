#include "Transformations.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
Transformations::Transformations(){}

Transformations::~Transformations(){}

glm::mat4x4 Transformations::ScalingTransformation(float sx, float sy, float sz)
{
	return glm::mat4x4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
}

glm::mat4x4 Transformations::TranslationTransformation(float tx, float ty, float tz)
{
	return glm::translate(glm::vec3(tx, ty, tz));
	//return glm::mat4x4(1, 0, 0, tx,  0, 1, 0, ty,   0, 0, 1,tz , 0, 0, 0, 1);
}

 glm::mat4x4 Transformations::Identity4X4Matrix()
{
	return glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

glm::mat4x4 Transformations::XRotationTransformation(float angle)
{
	return glm::rotate((glm::radians(angle)), glm::vec3(1, 0, 0));
}

glm::mat4x4 Transformations::YRotationTransformation(float angle)
{
	return glm::rotate((glm::radians(angle)), glm::vec3(0, 1, 0));
}

glm::mat4x4 Transformations::ZRotationTransformation(float angle)
{
	return glm::rotate((glm::radians(angle)), glm::vec3(0, 0, 1));
}
