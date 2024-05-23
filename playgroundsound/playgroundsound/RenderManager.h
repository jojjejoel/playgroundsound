#pragma once
#include "GameObject.h"
#include "CameraComponent.h"
#include <memory>
#include <string>
#include <vector>

#include "RenderComponent.h"

struct RenderTexture;
typedef RenderTexture RenderTexture2D;

class RenderManager
{
public:
	void SetCamera(CameraComponent* in_camera);
	void Init();
	void LoadModels();
	void StartRender();
	void Render();
	void EndRender();

	std::shared_ptr<Model> GetModel(std::string_view modelName);

	void SetLightColor(const GO_Vector3& in_color);

	RenderTexture2D* LoadShadowmapRenderTexture(int width, int height);
	void UnloadShadowmapRenderTexture(RenderTexture2D* target);

	void AddRenderObject(GameObject* in_objectToRender);

	void SetPlaybackSpeed(const float& speed);
private:

	std::vector<GameObject*> objectsToRender;
	CameraComponent* camera;
	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::map<std::string, std::shared_ptr<Model>> models;
	RenderTexture2D* shadowMap;
	Camera3D* lightCam;
	//Shader* shadowShader;
	int lightVPLoc;
	int shadowMapLoc;

	int lightDirLoc;
	int lightColLoc;
	GO_Vector3 lightColor;

	float playBackSpeed;
};

