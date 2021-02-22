#pragma once
#include <glm/glm.hpp>
#include <Transformations.h>

class Camera
{
public:
	Camera(glm::vec3 Preffered_eye);
	virtual ~Camera();
	void SetCameraLookAt();
	glm::mat4x4 GetLookAt();
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;
	void SetOrthographicWidth(float width);
	void UpdateOrthographicMat();
	void SetFovy(float fovy);
	void UpdatePerspectiveMat();
	bool GetIsOrthographic();
	void SetIsOrthographic(bool val);
	void SetAspectRatio(float W, float H);
	glm::vec3 GetEye();
	glm::vec3 GetAt();
	glm::vec3 GetUp();
	void SetEye(glm::vec3 eye);
	void SetAt(glm::vec3 at);
	void SetUp(glm::vec3 at);
	void SetWTransformation();
	void SetCTransformation();
	void SetTransformations();
	void SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis);
	void ResetTransformations();
	void SetCTranslate(glm::mat4x4 T);
	void SetWTranslate(glm::mat4x4 T);
	void SetWidth(float w);
	void SetHeight(float h);
	float GetFar();
	glm::mat4x4 GetC_inv();
	

private:
	float width=1920.f;
	float height=1080.f;
	float aspectratio = width/height;   
	float left = -width / 2;
	float right = width / 2;
	float bottom = -height / 2;
	float top = height / 2;
	float Near = 0.1f;
	float Far = 200.f;
	float Fovy = glm::radians(45.f);
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
	glm::mat4x4 LookAt;
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
	glm::mat4x4 WorldTransformation=Transformations::Identity4X4Matrix();
	glm::mat4x4 CameraTransformation=Transformations::Identity4X4Matrix();
	glm::mat4x4 C_inv = Transformations::Identity4X4Matrix();
	glm::mat4x4 C = Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationX=Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationY=Transformations::Identity4X4Matrix();
	glm::mat4x4 WRotationZ=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationX=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationY=Transformations::Identity4X4Matrix();
	glm::mat4x4 CRotationZ=Transformations::Identity4X4Matrix();
	glm::mat4x4 WTranslate = Transformations::Identity4X4Matrix();
	glm::mat4x4 CTranslate = Transformations::Identity4X4Matrix();
	bool IsOrthographic = true;
	
};