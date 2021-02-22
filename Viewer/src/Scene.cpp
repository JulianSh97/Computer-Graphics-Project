#include "Scene.h"


Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0),
	active_light_index_(0)
{

}

void Scene::ClearActiveModel()
{
	mesh_models_[active_model_index_].reset();
	mesh_models_.clear();
}

void Scene::SetenvironmentMap(bool flag)
{
	environmentMap = flag;
}

bool Scene::GetenvironmentMap()
{
	return environmentMap;
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

std::shared_ptr<MeshModel> Scene::GetModel(int index) const
{
		return mesh_models_[index];
}

std::shared_ptr<MeshModel> Scene::GetActiveModel() const
{
	return mesh_models_[active_model_index_];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

std::shared_ptr<Camera> Scene::GetCamera(int index)
{
	return cameras_[index];
}

std::shared_ptr<Camera> Scene::GetActiveCamera()
{
	return cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

void Scene::AddLight(const std::shared_ptr<Light>& light)
{
	lights_.push_back(light);
}

int Scene::GetLightCount() const
{
	return lights_.size();
}

std::shared_ptr<Light> Scene::GetLight(int index) const
{
	return lights_[index];
}

std::shared_ptr<Light> Scene::GetActiveLight()
{
	return lights_[active_light_index_];
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index_ = index;
}

int Scene::GetActiveLightIndex()
{
	return active_light_index_;
}

void Scene::SetFogEnd(float value)
{
	FogEnd = value;
}

const float Scene::GetFogEnd() const
{
	return FogEnd;
}

void Scene::SetFogStart(float value)
{
	FogStart = value;
}

const float Scene::GetFogStart() const
{
	return FogStart;
}

void Scene::SetIsLinearFog(bool value)
{
	LinearFog = value;
}

const bool Scene::GetIsLinearFog() const
{
	return LinearFog;
}

void Scene::SetFogDensity(float value)
{
	FogDensity = value;
}

const float Scene::GetFogDensity() const
{
	return FogDensity;
}

void Scene::SetColoring(int type)
{
	Coloring = type;
}

int Scene::GetColoring()
{
	return Coloring;
}

void Scene::SetShadingtype(ShadingType type)
{
	shadingType = type;
}

ShadingType Scene::GetShadingtype()
{
	return shadingType;
}
