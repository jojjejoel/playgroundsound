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
	void StartRender();
	void Render();
	void EndRender();

	void DrawUI();

	std::shared_ptr<Model> GetModel(std::string_view modelName);

	void SetLightColor(const GO_Vector3& in_color);

	RenderTexture2D* LoadShadowmapRenderTexture(int width, int height);
	void UnloadShadowmapRenderTexture(RenderTexture2D* target);

	void AddRenderObject(GameObject* in_objectToRender);

	void SetPlaybackSpeed(std::string_view in_playbackSpeed);
	void SetPortalEnabled(std::string_view in_portalIsEnabled);
	void SetBeatValue(std::string_view in_beatValueStr);
private:

	std::vector<GameObject*> objectsToRender;
	CameraComponent* camera;
	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::map<std::string, std::shared_ptr<Model>> models;
	RenderTexture2D* shadowMap;
	Camera3D* lightCam;
	int lightVPLoc;
	int shadowMapLoc;

	int lightDirLoc;
	int lightColLoc;
	GO_Vector3 lightColor;

	std::string playBackSpeed = "1.0";
	std::string portalIsEnabled = "OPEN";
	std::string beatValue = "1/4";
};

