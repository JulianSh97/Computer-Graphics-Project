#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "MeshModel.h"
#include "Enums.h"


class Light 
{
public:
	Light(glm::vec3 pos, LightType type);

	const glm::vec3& GetDiffuseLightColor() const;
	const glm::vec3& GetAmbientLightColor() const;
	const glm::vec3& GetSpecularLightColor() const;
	const glm::vec3& GetLightDirection() const;
	const glm::vec3& GetLightPosition() const;
	const LightType& GetLightType() const;
	glm::mat4x4 GetWorldTransformation() const;
	glm::mat4x4 GetLocalTransformation() const;
	void SetDiffuseLightColor(glm::vec3 color);
	void SetLightDirection(glm::vec3 direction);
	void SetLightPosition(glm::vec4 position);
	void SetAmbientLightColor(glm::vec3 color);
	void SetSpecularLightColor(glm::vec3 color);
	void SetRotationMatrix(float angle, bool IsWorld, int Axis);
	void SetTranslationMatrix(float Tx, float Ty, float Tz, bool IsWorld);
	void SetWorldTransformation();
	void SetLocalTransformation();
	void ResetTransformations();

	void SetLightType(LightType type);
	float GetWRotationX();
	float GetWRotationY();
	float GetWRotationZ();	
	float GetLRotationX();
	float GetLRotationY();
	float GetLRotationZ();
	float GetWTranslateX();
	float GetWTranslateY();
	float GetWTranslateZ();
	float GetLTranslateX();
	float GetLTranslateY();
	float GetLTranslateZ();
	GLuint GetVao();
	GLuint GetVbo();


private:
	glm::vec3 DlightColor;
	glm::vec3 AlightColor;
	glm::vec3 SlightColor;
	glm::vec3 lightDirection;
	glm::vec3 lightPosition;
	LightType lightType;
	ShadingType shadingType;
	glm::mat4x4 WorldTransformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 LocalTransformation = Transformations::Identity4X4Matrix();
	float WRotationX = 0.f;
	float WRotationY = 0.f;
	float WRotationZ = 0.f;
	float LRotationX = 0.f;
	float LRotationY = 0.f;
	float LRotationZ = 0.f;
	float WTranslateX = 0.f;
	float WTranslateY = 0.f;
	float WTranslateZ = 0.f;
	float LTranslateX = 0.f;
	float LTranslateY = 0.f;
	float LTranslateZ = 0.f;
	glm::vec3 parallelLights = glm::vec3(0.05f,0.f,0.f);
	GLuint LightVao;
	GLuint LightVbo;
};