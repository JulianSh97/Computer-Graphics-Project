#pragma once

#include <vector>
#include <memory>
#include "Camera.h"
#include "MeshModel.h"
#include "Light.h"
#include <CubeMap.h>
#include <string>

class Scene {
public:
	Scene();

	void AddLight(const std::shared_ptr<Light>& light);
	int GetLightCount() const;
	std::shared_ptr<Light> GetLight(int index) const;
	std::shared_ptr<Light> GetActiveLight();
	int GetActiveLightIndex();
	void SetActiveLightIndex(int index);

	void AddModel(const std::shared_ptr<MeshModel>& mesh_model);
	int GetModelCount() const;
	std::shared_ptr<MeshModel> GetModel(int index) const;
	std::shared_ptr<MeshModel> GetActiveModel() const;
	
	void AddCamera(const std::shared_ptr<Camera>& camera);

	int GetCameraCount() const;
	std::shared_ptr<Camera> GetCamera(int index);
	std::shared_ptr<Camera> GetActiveCamera();

	void SetActiveCameraIndex(int index);
	int GetActiveCameraIndex() const;

	void SetActiveModelIndex(int index);
	int GetActiveModelIndex() const;
	void ClearActiveModel();
	void SetenvironmentMap(bool flag);
	bool GetenvironmentMap();
	void SetFogEnd(float value);
	const float GetFogEnd() const;
	void SetFogStart(float value);
	const float GetFogStart() const;
	void SetIsLinearFog(bool value);
	const bool GetIsLinearFog() const;
	void SetFogDensity(float value);
	const float GetFogDensity() const;
	void SetColoring(int type);
	int GetColoring();
	void SetShadingtype(ShadingType type);
	ShadingType GetShadingtype();
	std::shared_ptr<CubeMap> cubemap;
	
private:
	bool environmentMap=false;
	bool LinearFog=true;
	float FogStart = -1;
	float FogEnd = 1;
	float FogDensity = -3;
	std::vector<std::shared_ptr<MeshModel>> mesh_models_;
	std::vector<std::shared_ptr<Camera>> cameras_;
	std::vector<std::shared_ptr<Light>> lights_;
	int active_camera_index_;
	int active_model_index_;
	int active_light_index_;
	int Coloring;
	ShadingType shadingType;
};