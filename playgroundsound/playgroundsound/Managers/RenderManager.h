#pragma once
#include "..\GameObject\GameObject.h"
#include "..\Components\CameraComponent.h"
#include <memory>
#include <string>
#include <vector>

#include "..\Components\RenderComponent.h"

struct RenderTexture;
typedef RenderTexture RenderTexture2D;

class RenderManager
{
public:
	void SetCamera(CameraComponent* in_camera);
	void Init();
	void LoadModels();
	void StartRender() const;
	void Render();
	void EndRender() const;

	void DrawUI() const;

	std::shared_ptr<Model> GetModel(std::string_view modelName);

	void SetLightColor(const GO_Vector3& in_color);

	static std::shared_ptr<RenderTexture2D> LoadShadowmapRenderTexture(int width, int height);

	void AddRenderObject(GameObject* in_objectToRender);

	void SetPlaybackSpeed(std::string_view in_playbackSpeed);
	void SetPortalEnabled(std::string_view in_portalIsEnabled);
	void SetBeatValue(std::string_view in_beatValueStr);
private:

	std::vector<GameObject*> objectsToRender;
	CameraComponent* camera = nullptr;
	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::map<std::string, std::shared_ptr<Model>> models;
	std::shared_ptr<RenderTexture2D> shadowMap;
	std::shared_ptr<Camera3D> lightCam;
	int lightVPLoc = 0;
	int shadowMapLoc = 0;

	int lightDirLoc = 0;
	int lightColLoc = 0;
	GO_Vector3 lightColor = {255,255,255};

	std::string playBackSpeed = "1.0";
	std::string portalIsEnabled = "OPEN";
	std::string beatValue = "1/4";
};

