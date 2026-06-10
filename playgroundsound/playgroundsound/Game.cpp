#include "Game.h"

#include <src/raylib.h>
#include <string>
#include <src/raymath.h>

#include "Utilities/VectorConversions.h"
#define RLIGHTS_IMPLEMENTATION

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            120
#endif

#include <algorithm>
#include <iostream>
#include "Components\Components.h"
#include "Wwise_Project/GeneratedSoundBanks/Wwise_IDs.h"


void Game::Init()
{
	renderManager.Init();
	AddGameObjects();
	renderManager.SetCamera(cameraObjPtr->GetComponent<CameraComponent>());

	WwiseRoomComponent& roomComponent = roomCubeObjPtr->AddComponent<WwiseRoomComponent>();
	WwiseRoomComponent::InitRoomGeometry(roomCubeObjPtr);
	WwiseRoomComponent::InitRoom(roomWallObjPtr);
	roomComponent.SetBoundingBox({ 0,0,0 }, 10, 10, 10);
	wwiseRoomManager.AddRoom(&roomComponent);
	wwiseRoomManager.AddObject(truckObjPtr->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(cameraObjPtr->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(musicEmitterObjPtr->GetComponent<WwiseObjectComponent>());
	SetTargetFPS(30);
	GameObject* portalObj = portalCubeObjPtr;
	portalObj->GetComponent<WwisePortalComponent>()->InitPortal(portalObj, WwiseRoomComponent::GetRoomID());

	gameObjectManager.Init();
	const WwiseObjectComponent* cameraWwiseComponent = cameraObjPtr->GetComponent<WwiseObjectComponent>();
	cameraWwiseComponent->RegisterAsListener();
	truckObjPtr->GetComponent<WwiseObjectComponent>()->PostEvent(AK::EVENTS::CAR_ENGINE_LOOP);
	truckObjPtr->GetComponent<WwiseObjectComponent>()->RegisterAsDistanceProbe(cameraObjPtr->m_id);
	musicEmitterObjPtr->GetComponent<WwiseObjectComponent>()->PostMusicEvent(
		AK::EVENTS::ENERGY,
		std::bind(&Game::MusicBar, this, std::placeholders::_1),  // Bind MusicBar with a float parameter
		std::bind(&Game::MusicBeat, this));  // Bind MusicBeat with no parameters
}

void Game::DeInit()
{
	CloseWindow();
}

void Game::AddGameObjects()
{
	truckObjPtr = gameObjectManager.AddGameObject("Truck");
	truckObjPtr->m_transform.position = { 0,0,10 };
	truckObjPtr->AddComponent<ControllerComponent>();
	truckObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("truck_green").get());
	renderManager.AddRenderObject(truckObjPtr);
	truckObjPtr->AddComponent<WwiseObjectComponent>();

	cameraObjPtr = gameObjectManager.AddGameObject("Camera");
	cameraObjPtr->AddComponent<CameraComponent>().SetTarget(truckObjPtr);
	cameraObjPtr->AddComponent<WwiseObjectComponent>();

	roomCubeObjPtr = gameObjectManager.AddGameObject("RoomCube");
	roomCubeObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("RoomCube").get(), true, true, { 255,0,0 });

	roomWallObjPtr = gameObjectManager.AddGameObject("RoomWall");
	roomWallObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("RoomWall").get(), true, true);

	roomWalls.left = AddRoomWall("RoomWallSide", "RoomWallLeft", { 5,0,0 });
	roomWalls.right = AddRoomWall("RoomWallSide", "RoomWallRight", { -5,0,0 });
	roomWalls.back = AddRoomWall("RoomWallFront", "RoomWallBack", { 0,0,-5 });
	roomWalls.top = AddRoomWall("RoomWallTop", "RoomWallTop", { 0,5,0 });
	roomWalls.bottom = AddRoomWall("RoomWallTop", "RoomWallFloor", { 0,-5,0 });

	musicEmitterObjPtr = gameObjectManager.AddGameObject("Music");
	musicEmitterObjPtr->AddComponent<WwiseObjectComponent>();
	musicEmitterObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("MusicCube").get(), true, false, { 0,0,255 });
	musicEmitterObjPtr->m_transform.position = { 0,0,3 };
	renderManager.AddRenderObject(musicEmitterObjPtr);

	portalCubeObjPtr = gameObjectManager.AddGameObject("PortalCube");
	portalCubeObjPtr->m_transform.position = { 0,0,5 };
	portalCubeObjPtr->m_transform.scale = { 10,10,0.1f };
	portalCubeObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("PortalCube").get(), false, false, { 0,255,0 });
	renderManager.AddRenderObject(portalCubeObjPtr);
	portalCubeObjPtr->AddComponent<WwisePortalComponent>();

	GameObject* wallBottomObj = gameObjectManager.AddGameObject("WallBottom");
	wallBottomObj->AddComponent<RenderComponent>().SetModel(renderManager.GetModel("WallTop").get());
	renderManager.AddRenderObject(wallBottomObj);
	wallBottomObj->m_transform.position = { 0,-1.5,0 };
}

GameObject* Game::AddRoomWall(const std::string_view modelName, const std::string_view gameObjectName, const GO_Vector3& position)
{
	GameObject* roomWall = gameObjectManager.AddGameObject(std::string(gameObjectName));
	roomWall->AddComponent<RenderComponent>().SetModel(renderManager.GetModel(std::string(modelName)).get(), true, false);
	roomWall->m_transform.position = position;
	renderManager.AddRenderObject(roomWall);
	return roomWall;
}

void Game::Run(bool& shouldExit)
{
	ControlPortalState();

	ControlPlaybackSpeed();

	UpdateBlinkingLight();

	ControlCarSfx();

	UpdateBouncingCube();

	gameObjectManager.Update();
	renderManager.Render();
	DrawDiffractionPaths();
	wwiseRoomManager.Update();
	renderManager.EndRender();

	if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose())
	{
		shouldExit = true;
	}
}

void Game::ControlCarSfx() const {
	const ControllerComponent* controllerComponent = truckObjPtr->GetComponent<ControllerComponent>();
	const WwiseObjectComponent* wwiseObjectComponent = truckObjPtr->GetComponent<WwiseObjectComponent>();
}

void Game::UpdateBouncingCube() const {
	const float rhythmVolume = musicEmitterObjPtr->GetComponent<WwiseObjectComponent>()->
		GetGameParamValueGlobal(AK::GAME_PARAMETERS::RHYTHM_VOLUME);
	musicEmitterObjPtr->m_transform.scale.y = bouncingCubeBaseHeight + rhythmVolume * bouncingCubeHeightMultiplier;
}

void Game::ControlPortalState()
{
	if (IsKeyPressed(KEY_ONE))
	{
		portalCubeObjPtr->GetComponent<WwisePortalComponent>()->TogglePortalState(portalCubeObjPtr);
		const bool portalEnabled = portalCubeObjPtr->GetComponent<WwisePortalComponent>()->GetIsEnabled();
		portalCubeObjPtr->GetComponent<RenderComponent>()->SetShouldRender(!portalEnabled);
		const std::string portalEnabledStr = portalEnabled ? "OPEN" : "CLOSED";
		renderManager.SetPortalEnabled(portalEnabledStr);
	}
}

void Game::ControlPlaybackSpeed()
{
	if (IsKeyDown(KEY_TWO))
	{
		playbackSpeed -= GetFrameTime();
	}
	if (IsKeyDown(KEY_THREE))
	{
		playbackSpeed += GetFrameTime();
	}

	playbackSpeed = std::clamp(playbackSpeed, minPlaybackSpeed, maxPlaybackSpeed);
	musicEmitterObjPtr->GetComponent<WwiseObjectComponent>()->SetRTPC(AK::GAME_PARAMETERS::PLAYBACK_SPEED, playbackSpeed);

	//Changes the value in the text on the screen representing the playback speed.
	renderManager.SetPlaybackSpeed(std::to_string(playbackSpeed));
}

Color Game::GetDiffractionColor(float diffractionValue) {
	return Color{
		static_cast<unsigned char>(Lerp(0, 255, diffractionValue)),
		static_cast<unsigned char>(Lerp(255, 0, diffractionValue)),
		0, 255
	};
}

void Game::DrawPathNodes(const DiffractionPath& path, const Vector3& listenerPos, const Color& color)
{
	if (path.nodeCount <= 0)
		return;

	const auto firstNodePos = VectorConversions::GOToRaylib(path.nodes[0]);
	DrawLine3D(firstNodePos, listenerPos, color);
	DrawSphereWires(firstNodePos, diffracationSphereRadius, diffracationSphereSegments, diffracationSphereSegments, color);

	for (int nodeIndex = 1; nodeIndex < path.nodeCount; ++nodeIndex)
	{
		const auto nodePos = VectorConversions::GOToRaylib(path.nodes[nodeIndex]);
		const auto prevNodePos = VectorConversions::GOToRaylib(path.nodes[nodeIndex - 1]);

		DrawSphereWires(nodePos, diffracationSphereRadius, diffracationSphereSegments, diffracationSphereSegments, color);
		DrawLine3D(prevNodePos, nodePos, color);
	}
}

void Game::DrawDiffractionPaths()
{
	const Vector3 listenerPos = VectorConversions::GOToRaylib(truckObjPtr->m_transform.position);

	float lowestDiffractionValue = 1.0f;

	for (const auto& diffractionPath : diffractionManager.GetDiffractionPath(musicEmitterObjPtr->m_id))
	{
		Color color = GetDiffractionColor(diffractionPath.diffraction);
		DrawPathNodes(diffractionPath, listenerPos, color);

		if (diffractionPath.diffraction < lowestDiffractionValue && diffractionPath.nodeCount > 0)
		{
			lowestDiffractionValue = diffractionPath.diffraction;

			const Vector3 emitterPos = VectorConversions::GOToRaylib(diffractionPath.emitterPos);
			const Vector3 lastNodePos = VectorConversions::GOToRaylib(diffractionPath.nodes[diffractionPath.nodeCount - 1]);

			DrawLine3D(emitterPos, lastNodePos, color);
		}
	}
}

void Game::UpdateBlinkingLight()
{
	// Callback from Wwise resets timeLeftOnBar to barDuration at the beginning of each bar in the music track.
	timeLeftOnBar -= GetFrameTime();

	const float barColorIntensity = std::max(0.0f, timeLeftOnBar / barDuration);

	// Define beat color configurations
	constexpr GO_Vector3 beatColors[numberOfBeatsInBar] = {
		{ colorMin, colorMax, colorMin },  // beatValue == 0 (green)
		{ colorMax, colorMin, colorMin },  // beatValue == 1 (red)
		{ colorMin, colorMax, colorMax },  // beatValue == 2 (cyan)
		{ colorMax, colorMax, colorMin }   // beatValue == 3 (yellow)
	};

	GO_Vector3 lightColor = { 0, 0, 0 };

	// Ensure beatValue is within the expected range and assign corresponding color
	if (beatValue >= 0 && beatValue < numberOfBeatsInBar) {
		lightColor = beatColors[beatValue];
	}

	// Scale the light color by barColorIntensity
	// lightColor = lightColor * barColorIntensity;

	renderManager.SetLightColor(lightColor);
}

void Game::MusicBeat() {
	// This method is called by a Wwise callback at the start of every beat in the music.
	beatValue += 1;
	if (beatValue > numberOfBeatsInBar - 1)
	{
		beatValue = 0;
	}
	const std::string beatValueStr = "Beat: " + std::to_string(beatValue + 1) + "/" + std::to_string(numberOfBeatsInBar);
	renderManager.SetBeatValue(beatValueStr);
}

void Game::MusicBar(const float& in_barDuration) {
	// This method is called by a Wwise callback at the start of every bar in the music.
	timeLeftOnBar = in_barDuration;
	barDuration = in_barDuration;
}
