#include "MeshModel.h"
#include <iostream>
#include "Renderer.h"
#include <random>
#define PHONGSHADING 999
#define WIREFRAME 990
#define TEXTURED 900
#define REFLECTION 909

MeshModel::MeshModel(ModelParameters& model) :
	faces_(model.faces),
	vertices_(model.vertices),
	normals_(model.normals),
	textureCoords(model.textureCoords),
	leftTopNear_(model.leftTopNear),
	rightTopNear_(model.rightTopNear),
	leftTopFar_(model.leftTopFar),
	rightTopFar_(model.rightTopFar),
	leftBottomNear_(model.leftBottomNear),
	rightBottomNear_(model.rightBottomNear),
	leftBottomFar_(model.leftBottomFar),
	rightBottomFar_(model.rightBottomFar),
	model_name_(model.modelName),
	ModelCenter(model.modelCenter)
{
    std::string name = GetModelName();
	colorMethod = WIREFRAME;
	if (!name.compare("Sphere.obj"))
	{
		TranslateFactor = 1000.f;
		Preffered_eye = glm::vec3(0, 0, 0.4);
		minOrtho = 0.355f;
		maxOrtho = 1.145f;
		minDensity = 300.f;
		maxDensity = 1000.f;
	}
	else
	if (!name.compare("banana.obj"))
	{
		TranslateFactor = 1000.f;
		Preffered_eye = glm::vec3(0, 0, 0.4);
		minOrtho = 0.355f;
		maxOrtho = 1.145f;
		minDensity = 300.f;
		maxDensity = 1000.f;
	}
	else
	if (!name.compare("beethoven.obj"))
	{
		TranslateFactor = 2;
		Preffered_eye = glm::vec3(0, 0, 40);
		minOrtho = 10.f;
		maxOrtho = 80.f;
	}
	else
	if (!name.compare("bishop.obj"))
	{
		TranslateFactor = 1000.f;
		Preffered_eye = glm::vec3(0, 0, 0.5);
		minOrtho = 0.355f;
		maxOrtho = 1.145f;
		minDensity = 400.f;
		maxDensity = 1000.f;
	}
	else
	if (!name.compare("blob.obj"))
	{
		TranslateFactor = 2;
		Preffered_eye = glm::vec3(0, 0, 1000);
		minOrtho = 150.f;
		maxOrtho = 450.f;
		minDensity = 1.f;
		maxDensity = 10.f;
	}
	else
	if (!name.compare("bunny.obj"))
	{
		TranslateFactor = 55.f;
		Preffered_eye = glm::vec3(0, 0, 4);
		minOrtho = 3.f;
		maxOrtho = 27.f;
		minDensity = 50.f;
		maxDensity = 200.f;
	}
	else
	if (!name.compare("camera.obj"))
	{
		TranslateFactor = 35.f;
		Preffered_eye = glm::vec3(0, 0, 10);
		minOrtho = 5.f;
		maxOrtho = 40.f;
		minDensity = 20.f;
		maxDensity = 100.f;
	}
	else
	if (!name.compare("chain.obj"))
	{
		TranslateFactor = 35.f;
		Preffered_eye = glm::vec3(0, 0, 8);
		minOrtho = 10.f;
		maxOrtho = 35.f;
	}
	else
	if (!name.compare("cow.obj"))
	{
		TranslateFactor = 30.f;
		Preffered_eye = glm::vec3(0, 0, 11);
		minOrtho = 1.f;
		maxOrtho = 50.f;
	}
	else
	if (!name.compare("demo.obj"))
	{
		TranslateFactor = 33.f;
		Preffered_eye = glm::vec3(0, 0, 15);
		minOrtho = 10.f;
		maxOrtho = 50.f;
	}
	else
	if (!name.compare("dolphin.obj"))
	{
		TranslateFactor = 1.f;
		Preffered_eye = glm::vec3(0, 0, 1000);
		minOrtho = 500.f;
		maxOrtho = 2500.f;
	}
	else
	if (!name.compare("feline.obj"))
	{
		TranslateFactor = 20.f;
		Preffered_eye = glm::vec3(0, 0, 10);
		minOrtho = 1.f;
		maxOrtho = 29.f;
	}
	else
	if (!name.compare("pawn.obj"))
	{
		TranslateFactor = 1000.f;
		Preffered_eye = glm::vec3(0, 0, 0.5);
		minOrtho = 0.355f;
		maxOrtho = 1.145f;
	}
	else
	if (!name.compare("teapot.obj"))
	{
		TranslateFactor = 50.f;
		Preffered_eye = glm::vec3(0, 0, 10);
		minOrtho = 0.f;
		maxOrtho = 30.f;
	}
	else
	{
		TranslateFactor = 50;
		Preffered_eye = glm::vec3(0, 0, 1);
		minOrtho = 0;
		maxOrtho = 10;
	}
	modelVertices.reserve(3 * model.faces.size());
	for (int i = 0; i < faces_.size(); i++)
	{
		Face currentFace = faces_.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices_[vertexIndex];
			if(normals_.size())
			vertex.normal = normals_[normalIndex];

			if (textureCoords.size() > 0)
			{
				HasVt = true;
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = model.textureCoords[textureCoordsIndex];
			}
			modelVertices.push_back(vertex);
		}
	}
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}

const glm::vec3 MeshModel::GetVertex(int index)const {
	return vertices_[index];
}

const glm::mat4x4& MeshModel::GetPreTransformation() {
	return PreTransformation;
}

void MeshModel::SetTransformation() {
	this->Transformation = WorldTransformation * ModelTransformation;//S_w * R_w * T_w * S_m * T_m *R_m ;
}

void MeshModel::SetWorldTransformation(glm::mat4x4 Transformation) {
	WorldTransformation = Transformation;
}

void MeshModel::SetModelTransformation(glm::mat4x4 Transformation) {
	ModelTransformation = Transformation;
}

const glm::mat4x4& MeshModel::GetTransformation()const {
	return this->Transformation;
}

glm::mat4x4 MeshModel::GetWorldTransformation() {
	return WorldTransformation;
}

glm::mat4x4 MeshModel::GetModelTransformation() {
	return ModelTransformation;
}

void MeshModel::Set_S_w(glm::mat4x4& Transformation) {
	S_w = Transformation;
}

void MeshModel::Set_R_w() {
	R_w = (W_Rotation_X) *(W_Rotation_Y) *(W_Rotation_Z);
}

void MeshModel::Set_T_w(glm::mat4x4& Transformation) {
	T_w = Transformation;
}

void MeshModel::Set_S_m(glm::mat4x4& Transformation) {
	S_m = Transformation;
}

void MeshModel::Set_R_m() {
	R_m =(M_Rotation_X)*(M_Rotation_Y)*(M_Rotation_Z);
}

void MeshModel::Set_T_m(glm::mat4x4& Transformation) {
	T_m = Transformation;
}

glm::mat4x4 MeshModel::Get_R_m()
{
	return R_m;
}

glm::mat4x4 MeshModel::Get_S_m()
{
	return S_m;
}

glm::mat4x4 MeshModel::Get_T_m()
{
	return T_m;
}

glm::mat4x4 MeshModel::Get_R_w()
{
	return R_w;
}

glm::mat4x4 MeshModel::Get_S_w()
{
	return S_w;
}

glm::mat4x4 MeshModel::Get_T_w()
{
	return T_w;
}

void MeshModel::ResetModel()
{
	this->S_m = Transformations::Identity4X4Matrix();
	this->R_m = Transformations::Identity4X4Matrix();
	this->T_m = Transformations::Identity4X4Matrix();
	this->S_w = Transformations::Identity4X4Matrix();
	this->R_w= Transformations::Identity4X4Matrix();
	this->T_w = Transformations::Identity4X4Matrix();
	this->M_Rotation_X = Transformations::Identity4X4Matrix();
	this->M_Rotation_Y = Transformations::Identity4X4Matrix();
	this->M_Rotation_Z = Transformations::Identity4X4Matrix();
	this->W_Rotation_X = Transformations::Identity4X4Matrix();
	this->W_Rotation_Y = Transformations::Identity4X4Matrix();
	this->W_Rotation_Z = Transformations::Identity4X4Matrix();
	this->WorldTransformation = Transformations::Identity4X4Matrix();
	this->ModelTransformation = Transformations::Identity4X4Matrix();
	this->Transformation = Transformations::Identity4X4Matrix();
}

void MeshModel::SetBoundingBoxFlag()
{
	ShowOrHideBoundingBox = !ShowOrHideBoundingBox;
}

bool MeshModel::GetBoundingBoxFlag()
{
	return ShowOrHideBoundingBox;
}

void MeshModel::SetFacesNormalsFlag()
{
	ShowOrHideFacesNormals = !ShowOrHideFacesNormals;
}

bool MeshModel::GetFacesNormalsFlag()
{
	return ShowOrHideFacesNormals;
}

void MeshModel::SetNormalsFlag()
{
	NormalsFlag = !NormalsFlag;
}

glm::vec4 MeshModel::GetLeftTopNear()
{
	return leftTopNear_;
}

glm::vec4 MeshModel::GetRightTopNear()
{
	return rightTopNear_;
}

glm::vec4 MeshModel::GetLeftTopFar()
{
	return leftTopFar_;
}

glm::vec4 MeshModel::GetRightTopFar()
{
	return rightTopFar_;
}

glm::vec4 MeshModel::GetLeftBottomNear()
{
	return leftBottomNear_;
}

glm::vec4 MeshModel::GetLeftBottomFar()
{
	return leftBottomFar_;
}

glm::vec4 MeshModel::GetRightBottomNear()
{
	return rightBottomNear_;
}

glm::vec4 MeshModel::GetRightBottomFar()
{
	return rightBottomFar_;
}

void MeshModel::SetRotationMatrix(glm::mat4x4& Transformation, bool IsWorld, int Axis)
{
	if (IsWorld)
	{
		switch (Axis) {
		case 1:
			W_Rotation_X = Transformation;
			break;
		case 2:
			W_Rotation_Y = Transformation;
			break;
		case 3:
			W_Rotation_Z = Transformation;
			break;
		}
	}
	else
	{
		switch (Axis) {
		case 1:
			M_Rotation_X = Transformation;
			break;
		case 2:
			M_Rotation_Y = Transformation;
			break;
		case 3:
			M_Rotation_Z = Transformation;
			break;
		}
	}
}

bool MeshModel::GetNormalsFlag()
{
	return NormalsFlag;
}

glm::vec3 MeshModel::GetNormals(int index) const
{
	return normals_[(index - 1)];
}

int MeshModel::GetVertexCount()
{
	return vertices_.size();
}

void MeshModel::SetColors(float* BB, float* FN, float* VN)
{
	BoundingBoxColor_ = glm::vec3(BB[0],BB[1],BB[2]);
	FacesNormalsColor_ = glm::vec3(FN[0], FN[1], FN[2]);
	NormalsColor_ = glm::vec3(VN[0], VN[1], VN[2]);
}

glm::vec3& MeshModel::GetBB() 
{ 
	return BoundingBoxColor_;
}

glm::vec3& MeshModel::GetFN() 
{ return FacesNormalsColor_; }

glm::vec3& MeshModel::GetVN() 
{ return NormalsColor_; }

float MeshModel::GetMinOrtho()
{
	return minOrtho;
}

float MeshModel::GetMaxOrtho()
{
	return maxOrtho;
}

glm::vec3 MeshModel::GetPreffered_Eye()
{
	return Preffered_eye;
}

float MeshModel::GetTranslateFactor()
{
	return TranslateFactor;
}
void MeshModel::SetColorMethod(int meth)
{
	colorMethod = meth;
}
int MeshModel::GetColorMethod()
{
	return colorMethod;
}

glm::vec3 MeshModel::GetDiffuseColor() { return DModelColor; }
glm::vec3 MeshModel::GetAmbientColor() { return AModelColor; }
glm::vec3 MeshModel::GetSpecularColor() { return SModelColor; }
void MeshModel::SetDiffuseColor(glm::vec3& color) { DModelColor = color; }
void MeshModel::SetAmbientColor(glm::vec3& color) { AModelColor = color; }
void MeshModel::SetSpecularColor(glm::vec3& color) { SModelColor = color; }
glm::vec4 MeshModel::GetModelCenter() { return glm::vec4(ModelCenter, 1); }

GLuint MeshModel::GetVAO() const
{
	return vao;
}
const std::vector<Vertex>& MeshModel::GetModelVertices()
{
	return modelVertices;
}

void  MeshModel::SetAlpha(float a)
{
	alpha = a;
}
float MeshModel::GetAlpha()
{
	return alpha;
}

void MeshModel::SetHasVt(bool a)
{
	HasVt = a;
}

bool MeshModel::GetHasVt()
{
	return HasVt;
}

float MeshModel::GetScaleX() { return ScaleX; }
float MeshModel::GetScaleY() { return ScaleY; }
float MeshModel::GetScaleZ() { return ScaleZ; }
float MeshModel::GetWScaleX() { return WScaleX; }
float MeshModel::GetWScaleY() { return WScaleY; }
float MeshModel::GetWScaleZ() { return  WScaleZ; }
float MeshModel::GetTranslateX(){return TranslateX;}
float MeshModel::GetTranslateY(){return TranslateY;}
float MeshModel::GetTranslateZ(){return TranslateZ;}
float MeshModel::GetWTranslateX(){return WTranslateX;}
float MeshModel::GetWTranslateY(){return WTranslateY;}
float MeshModel::GetWTranslateZ(){return WTranslateZ;}
float MeshModel::GetAngleX(){return AngleX;}
float MeshModel::GetAngleY(){return AngleY;}
float MeshModel::GetAngleZ(){return AngleZ;}
float MeshModel::GetWAngleX(){return WAngleX;}
float MeshModel::GetWAngleY(){return WAngleY;}
float MeshModel::GetWAngleZ(){return WAngleZ;}

void MeshModel::SetScaleX(float S) { ScaleX = S; }
void MeshModel::SetScaleY(float S) {ScaleY=S; }
void MeshModel::SetScaleZ(float S) {ScaleZ=S; }
void MeshModel::SetWScaleX(float S) { WScaleX = S; }
void MeshModel::SetWScaleY(float S) { WScaleY = S; }
void MeshModel::SetWScaleZ(float S) { WScaleZ = S; }
void MeshModel::SetTranslateX(float S) { TranslateX = S; }
void MeshModel::SetTranslateY(float S) { TranslateY = S; }
void MeshModel::SetTranslateZ(float S) { TranslateZ = S; }
void MeshModel::SetWTranslateX(float S) { WTranslateX=S; }
void MeshModel::SetWTranslateY(float S) { WTranslateY=S; }
void MeshModel::SetWTranslateZ(float S) { WTranslateZ=S; }
void MeshModel::SetAngleX(float S) { AngleX=S;}
void MeshModel::SetAngleY(float S) { AngleY=S;}
void MeshModel::SetAngleZ(float S) { AngleZ=S;}
void MeshModel::SetWAngleX(float S) {WAngleX = S;}
void MeshModel::SetWAngleY(float S) { WAngleY = S; }
void MeshModel::SetWAngleZ(float S) { WAngleZ = S; }
void MeshModel::LoadTextures(const std::string& filePath)
{
	if (!texture1.loadTexture(filePath, true))
	{
		texture1.loadTexture(filePath, true);
	}
}
