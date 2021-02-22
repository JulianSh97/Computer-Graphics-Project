#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Transformations.h"
#include "Face.h"
#include "ModelParameters.h"
#include "Enums.h"
#include <glad/glad.h>
#include <Texture2D.h>

#define GRAYSCALE 999
#define RANDOM_COLORED 990
#define MODEL_COLOR 900

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class MeshModel
{
public:
	MeshModel(ModelParameters& model);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::mat4x4& GetPreTransformation();
	const glm::vec3 MeshModel::GetVertex(int index)const;
	void Set_S_w(glm::mat4x4& Transformation);
	void Set_R_w();
	void Set_T_w(glm::mat4x4& Transformation);
	void Set_S_m(glm::mat4x4& Transformation);
	void Set_R_m();
	void Set_T_m(glm::mat4x4& Transformation);
	void SetRotationMatrix(glm::mat4x4& Transformation,bool IsWorld,int Axis);
	void SetBoundingBoxFlag();
	bool GetBoundingBoxFlag();
	const glm::mat4x4& GetTransformation()const;
	void SetTransformation();
	void ResetModel();
	glm::vec4 GetLeftTopNear();
	glm::vec4 GetRightTopNear();
	glm::vec4 GetLeftTopFar();
	glm::vec4 GetRightTopFar();
	glm::vec4 GetLeftBottomNear();
	glm::vec4 GetLeftBottomFar();
	glm::vec4 GetRightBottomNear();
	glm::vec4 GetRightBottomFar();
	int GetVertexCount();
	void SetFacesNormalsFlag();
	void SetNormalsFlag();
	glm::vec3 GetNormals(int index) const;
	bool GetFacesNormalsFlag();
	bool GetNormalsFlag();
	glm::mat4x4 Get_R_m();
	glm::mat4x4 Get_R_w();
	glm::mat4x4 Get_S_w();
	glm::mat4x4 Get_S_m();
	glm::mat4x4 Get_T_w();
	glm::mat4x4 Get_T_m();
	void SetWorldTransformation(glm::mat4x4 Transformation);
	void SetModelTransformation(glm::mat4x4 Transformation);
	glm::mat4x4 GetWorldTransformation();
	glm::mat4x4 GetModelTransformation();
	void SetColors(float* BB, float* FN, float* VN);
	glm::vec3& GetBB();
	glm::vec3& GetFN();
	glm::vec3& GetVN();
	float GetMinOrtho();
	float GetMaxOrtho();
	float GetTranslateFactor();
	glm::vec3 GetPreffered_Eye();
	void SetColorMethod(int meth);
	int GetColorMethod();
	glm::vec3 GetDiffuseColor();
	glm::vec3 GetAmbientColor();
	glm::vec3 GetSpecularColor();
	void SetDiffuseColor(glm::vec3& collor);
	void SetAmbientColor(glm::vec3& collor);
	void SetSpecularColor(glm::vec3& collor);
	glm::vec4 GetModelCenter();
	float minDensity, maxDensity;
	GLuint GetVAO() const;
	const std::vector<Vertex>& GetModelVertices();
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec2> textureCoords;
	void SetAlpha(float a);
	float GetAlpha();
	void SetHasVt(bool a);
	bool GetHasVt();
	float GetScaleX();
	float GetScaleY();
	float GetScaleZ();
	float GetWScaleX();
	float GetWScaleY();
	float GetWScaleZ();

	float GetTranslateX();
	float GetTranslateY();
	float GetTranslateZ();
	float GetWTranslateX();
	float GetWTranslateY();
	float GetWTranslateZ();

	float GetAngleX();
	float GetAngleY();
	float GetAngleZ();
	float GetWAngleX();
	float GetWAngleY();
	float GetWAngleZ();

	void SetScaleX(float S) ;
	void SetScaleY(float S) ;
	void SetScaleZ(float S) ;
	void SetWScaleX(float S);
	void SetWScaleY(float S);
	void SetWScaleZ(float S);
	void SetTranslateX(float S) ;
	void SetTranslateY(float S) ;
	void SetTranslateZ(float S) ;
	void SetWTranslateX(float S);
	void SetWTranslateY(float S);
	void SetWTranslateZ(float S);
	void SetAngleX(float S) ;
	void SetAngleY(float S) ;
	void SetAngleZ(float S) ;
	void SetWAngleX(float S);
	void SetWAngleY(float S);
	void SetWAngleZ(float S);
	Texture2D texture1;
	void LoadTextures(const std::string& filePath);


protected:
	std::vector<Vertex> modelVertices;
	GLuint vbo;
	GLuint vao; 


private:
	
	bool HasVt=false;
	float alpha;
	glm::mat4x4 PreTransformation;
	std::vector<glm::vec3> boundingBoxVertices;
	glm::mat4x4 S_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_w= Transformations::Identity4X4Matrix();
	glm::mat4x4 S_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 R_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 T_m= Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_X = Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_Y = Transformations::Identity4X4Matrix();
	glm::mat4x4 M_Rotation_Z= Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_X = Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_Y = Transformations::Identity4X4Matrix();
	glm::mat4x4 W_Rotation_Z = Transformations::Identity4X4Matrix();
	glm::mat4x4 WorldTransformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 Transformation = Transformations::Identity4X4Matrix();
	glm::mat4x4 ModelTransformation = Transformations::Identity4X4Matrix();
	float ScaleX = 1.f;
	float ScaleY = 1.f;
	float ScaleZ = 1.f;
	float WScaleX = 1.f;
	float WScaleY = 1.f;
	float WScaleZ = 1.f;
	float TranslateX=0.f;
	float TranslateY=0.f;
	float TranslateZ=0.f;
	float WTranslateX = 0.f;
	float WTranslateY = 0.f;
	float WTranslateZ = 0.f;
	float AngleX = 0.f;
	float AngleY = 0.f;
	float AngleZ = 0.f;
	float WAngleX = 0.f;
	float WAngleY = 0.f;
	float WAngleZ = 0.f;
	glm::vec4 leftTopNear_;
	glm::vec4 rightTopNear_;
	glm::vec4 leftTopFar_;
	glm::vec4 rightTopFar_;
	glm::vec4 leftBottomNear_;
	glm::vec4 rightBottomNear_;
	glm::vec4 leftBottomFar_;
	glm::vec4 rightBottomFar_;
	std::string model_name_;
	bool ShowOrHideBoundingBox = false;
	bool ShowOrHideFacesNormals = false;
	bool NormalsFlag = false;
	glm::vec3 BoundingBoxColor_;
	glm::vec3 FacesNormalsColor_;
	glm::vec3 NormalsColor_;
	glm::vec3 DModelColor = glm::vec3(0.5f, 0.285f, 0.285f);
	glm::vec3 AModelColor = glm::vec3(0.0625f, 0.3f, 0.57f);
	glm::vec3 SModelColor;
	glm::vec3 Preffered_eye;
	float TranslateFactor;
	float minOrtho, maxOrtho;
	int colorMethod;
	glm::vec3 ModelCenter;
};
