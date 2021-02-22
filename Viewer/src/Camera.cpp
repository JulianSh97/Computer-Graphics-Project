#include "Camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/transform.hpp>
#include <Transformations.h>

Camera::Camera(glm::vec3 Preffered_Eye)
{
	this->width = 1920;
	this->height = 1080;
	this->eye = glm::vec3(0,0,1);
	this->at = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
	WorldTransformation = Transformations::Identity4X4Matrix();
	SetCameraLookAt();
	SetOrthographicWidth(this->width);
	view_transformation_ = Transformations::Identity4X4Matrix();
}

Camera::~Camera()
{

}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{
	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}

void Camera::SetCameraLookAt()
{
	LookAt = glm::lookAt(eye, at, up);
}

glm::mat4x4 Camera::GetLookAt()
{
	return this->LookAt;
}

void Camera::SetOrthographicWidth(float Orthowidth) 
{
	if (Orthowidth == 0)
		return;
	this->width = Orthowidth;
	this->height = Orthowidth / aspectratio;
	this->left = -Orthowidth / 2;
	this->right = Orthowidth / 2;
	this->bottom = -this->height/2;
	this->top = this->height / 2;	
	UpdateOrthographicMat();
}

void Camera::SetFovy(float fovy) {
	this->Fovy = glm::radians(fovy);
	UpdatePerspectiveMat();
}

void Camera::UpdateOrthographicMat()
{
	//glm::vec4 c1(2 / (right - left), 0, 0, 0);
	//glm::vec4 c2(0, 2 / (top - bottom), 0, 0);
	//glm::vec4 c3(0, 0, -2 / (Far - Near), 0);
	//glm::vec4 c4(-((right + left) / (right - left)), -((top + bottom) / (top - bottom)), -((Far + Near) / (Far - Near)), 1);
	//projection_transformation_ = glm::mat4(c1, c2, c3, c4);
	projection_transformation_ = glm::ortho(left, right, bottom, top,Near,Far);
}

void Camera::UpdatePerspectiveMat() 
{
	projection_transformation_ = glm::perspective(this->Fovy, aspectratio, Near, Far);
}

bool Camera::GetIsOrthographic()
{
	return IsOrthographic;
}

void Camera::SetIsOrthographic(bool val)
{
	this->IsOrthographic = val;
}

void Camera::SetAspectRatio(float W, float H) 
{
	this->width = W;
	this->height = H;
	this->aspectratio = W / H;
}

glm::vec3 Camera::GetEye()
{
	return eye;
}

glm::vec3 Camera::GetAt()
{
	return at;
}

glm::vec3 Camera::GetUp()
{
	return up;
}

void Camera::SetEye(glm::vec3 Eye)
{
	this->eye = Eye;
}

void Camera::SetAt(glm::vec3 At)
{
	this->at = At;
}

void Camera::SetUp(glm::vec3 Up)
{
	this->up = Up;
}

void Camera::SetWTransformation()
{
	WorldTransformation = (WRotationX) *(WRotationY) *(WRotationZ) * WTranslate;
}

void Camera::SetCTransformation()
{
	CameraTransformation = (CRotationX) * (CRotationY) * (CRotationZ)*CTranslate;
}

void Camera::SetCTranslate(glm::mat4x4 T)
{
	CTranslate = T;
}

void Camera::SetWTranslate(glm::mat4x4 T)
{
	WTranslate = T;
}

void Camera::SetTransformations()
{
	C = WorldTransformation * CameraTransformation;
	C_inv = glm::inverse(C);
}

void Camera::SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis)
{
	if (IsWorld)
	{
		switch (Axis) {
		case 1:
			WRotationX = Transformation;
			break;
		case 2:
			WRotationY = Transformation;
			break;
		case 3:
			WRotationZ = Transformation;
			break;
		}
	}
	else
	{
		switch (Axis) {
		case 1:
			CRotationX = Transformation;
			break;
		case 2:
			CRotationY = Transformation;
			break;
		case 3:
			CRotationZ = Transformation;
			break;
		}
	}
}

void Camera::ResetTransformations()
{
	WRotationX = Transformations::Identity4X4Matrix();
	WRotationY = Transformations::Identity4X4Matrix();
	WRotationZ = Transformations::Identity4X4Matrix();
	CRotationX = Transformations::Identity4X4Matrix();
	CRotationY = Transformations::Identity4X4Matrix();
	CRotationZ = Transformations::Identity4X4Matrix();
	WTranslate = Transformations::Identity4X4Matrix();
	CTranslate = Transformations::Identity4X4Matrix();
	WorldTransformation = Transformations::Identity4X4Matrix();
	CameraTransformation = Transformations::Identity4X4Matrix();
	C_inv = Transformations::Identity4X4Matrix();
	C = Transformations::Identity4X4Matrix();
}

glm::mat4x4 Camera::GetC_inv() {
	return C_inv;
}

float Camera::GetFar()
{
	return Far;
}

void Camera::SetWidth(float w)
{
	width = w;
}

void Camera::SetHeight(float h)
{
	height = h;
}