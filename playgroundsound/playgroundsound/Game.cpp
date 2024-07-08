#include "Game.h"

#include <src/raylib.h>
#include <src/rcamera.h>
#include <string>
#include <src/raymath.h>
#include <math.h>
#define RLIGHTS_IMPLEMENTATION
#include <examples/shaders/rlights.h>

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
	renderManager.SetCamera(&cameraObjPtr->GetComponent<CameraComponent>());

	WwiseRoomComponent& roomComponent = roomCubeObjPtr->AddComponent<WwiseRoomComponent>();
	roomComponent.InitRoomGeometry(roomCubeObjPtr);
	roomComponent.InitRoom(roomWallObjPtr);
	roomComponent.SetBoundingBox({ 0,0,0 }, 10, 10, 10);
	wwiseRoomManager.AddRoom(&roomComponent);
	wwiseRoomManager.AddObject(&truckObjPtr->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(&cameraObjPtr->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(&musicEmitterObjPtr->GetComponent<WwiseObjectComponent>());
	SetTargetFPS(30);
	GameObject* portalObj = portalCubeObjPtr;
	portalObj->GetComponent<WwisePortalComponent>().InitPortal(portalObj, roomComponent.GetRoomID());

	gameObjectManager.Init();
	WwiseObjectComponent& cameraWwiseComponent = cameraObjPtr->GetComponent<WwiseObjectComponent>();
	cameraWwiseComponent.RegisterAsListener();
	truckObjPtr->GetComponent<WwiseObjectComponent>().PostEvent(AK::EVENTS::CAR_ENGINE_LOOP);
	truckObjPtr->GetComponent<WwiseObjectComponent>().RegisterAsDistanceProbe(cameraObjPtr->m_id);
	musicEmitterObjPtr->GetComponent<WwiseObjectComponent>().PostMusicEvent(
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
	truckObjPtr->AddComponent<ControllerComponent>().SetMovementSpeed(5);
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


	AddRoomWalls(roomWallLeftObjPtr, "RoomWallSide", "RoomWallLeft", { 5,0,0 });
	AddRoomWalls(roomWallRightObjPtr, "RoomWallSide", "RoomWallRight", { -5,0,0 });
	AddRoomWalls(roomWallBackObjPtr, "RoomWallFront", "RoomWallBack", { 0,0,-5 });
	AddRoomWalls(roomWallTopObjPtr, "RoomWallTop", "RoomWallTop", { 0,5,0 });
	AddRoomWalls(roomWallBottomObjPtr, "RoomWallTop", "RoomWallFloor", { 0,-5,0 });

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

void Game::AddRoomWalls(GameObject* in_roomWallObjPtr, std::string_view modelName, std::string_view gameObjectName, const GO_Vector3& position)
{
	in_roomWallObjPtr = gameObjectManager.AddGameObject(gameObjectName.data());
	in_roomWallObjPtr->AddComponent<RenderComponent>().SetModel(renderManager.GetModel(modelName.data()).get(), true, false);
	in_roomWallObjPtr->m_transform.position = position;
	renderManager.AddRenderObject(in_roomWallObjPtr);
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

void Game::ControlCarSfx()
{
	auto& controllerComponent = truckObjPtr->GetComponent<ControllerComponent>();
	auto& wwiseObjectComponent = truckObjPtr->GetComponent<WwiseObjectComponent>();

	float carSpeed = controllerComponent.GetPercentageOfMaxSpeed();
	wwiseObjectComponent.SetRTPC(AK::GAME_PARAMETERS::CAR_SPEED, carSpeed);

	float carGas = controllerComponent.GetGas();
	wwiseObjectComponent.SetRTPC(AK::GAME_PARAMETERS::CAR_GAS, carGas);
}

void Game::UpdateBouncingCube()
{
	float startHeight = 0.2f;
	float rhythmVolume = musicEmitterObjPtr->GetComponent<WwiseObjectComponent>().
		GetGameParamValueGlobal(AK::GAME_PARAMETERS::RHYTHM_VOLUME);
	float heightMultiplier = 3;
	musicEmitterObjPtr->m_transform.scale.y = startHeight + rhythmVolume * heightMultiplier;
}

void Game::ControlPortalState()
{
	if (IsKeyPressed(KEY_ONE))
	{
		portalCubeObjPtr->GetComponent<WwisePortalComponent>().TogglePortalState(portalCubeObjPtr);
		bool portalEnabled = portalCubeObjPtr->GetComponent<WwisePortalComponent>().GetIsEnabled();
		portalCubeObjPtr->GetComponent<RenderComponent>().SetShouldRender(!portalEnabled);
		std::string portalEnabledStr = portalEnabled ? "OPEN" : "CLOSED";
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
	musicEmitterObjPtr->GetComponent<WwiseObjectComponent>().SetRTPC(AK::GAME_PARAMETERS::PLAYBACK_SPEED, playbackSpeed);

	//Changes the value in the text on the screen representing the playback speed.
	renderManager.SetPlaybackSpeed(std::to_string(playbackSpeed));
}

void Game::DrawDiffractionPaths()
{
	auto diffractionPaths = diffractionManager.GetDiffractionPath(musicEmitterObjPtr->m_id);
	auto listenerPos = Vector3{
		truckObjPtr->m_transform.position.x,
		truckObjPtr->m_transform.position.y,
		truckObjPtr->m_transform.position.z
	};

	float lowestDiffractionValue = 1.0f;

	for (const auto& diffractionPath : diffractionPaths)
	{
		Color color = {
			static_cast<unsigned char>(Lerp(0, 255, diffractionPath.diffraction)),
			static_cast<unsigned char>(Lerp(255, 0, diffractionPath.diffraction)),
			0, 255
		};

		if (diffractionPath.nodeCount > 0)
		{
			auto firstNodePos = Vector3{
				diffractionPath.nodes[0].x,
				diffractionPath.nodes[0].y,
				diffractionPath.nodes[0].z
			};

			DrawLine3D(firstNodePos, listenerPos, color);
			DrawSphereWires(firstNodePos, 0.2f, 10, 10, color);

			for (int nodeIndex = 1; nodeIndex < diffractionPath.nodeCount; ++nodeIndex)
			{
				auto nodePos = Vector3{
					diffractionPath.nodes[nodeIndex].x,
					diffractionPath.nodes[nodeIndex].y,
					diffractionPath.nodes[nodeIndex].z
				};

				auto emitterPos = Vector3{
					diffractionPath.nodes[nodeIndex - 1].x,
					diffractionPath.nodes[nodeIndex - 1].y,
					diffractionPath.nodes[nodeIndex - 1].z
				};

				DrawSphereWires(nodePos, 0.2f, 10, 10, color);
				DrawLine3D(emitterPos, nodePos, color);
			}

			if (diffractionPath.diffraction < lowestDiffractionValue)
			{
				lowestDiffractionValue = diffractionPath.diffraction;

				auto emitterPos = Vector3{
					diffractionPath.emitterPos.x,
					diffractionPath.emitterPos.y,
					diffractionPath.emitterPos.z
				};

				auto lastNodePos = Vector3{
					diffractionPath.nodes[diffractionPath.nodeCount - 1].x,
					diffractionPath.nodes[diffractionPath.nodeCount - 1].y,
					diffractionPath.nodes[diffractionPath.nodeCount - 1].z
				};

				DrawLine3D(emitterPos, lastNodePos, color);
			}
		}
	}
}

void Game::UpdateBlinkingLight()
{
	// Callback from Wwise resets timeLeftOnBar to barDuration at the beginning of each bar in the music track.
	timeLeftOnBar -= GetFrameTime();

	const float barColorIntensity = std::max(0.0f, timeLeftOnBar / barDuration);

	const uint8_t MAX_COLOR = 255;
	const uint8_t MIN_COLOR = 0;

	// Define beat color configurations
	const GO_Vector3 beatColors[numberOfBeatsInBar] = {
		{ MIN_COLOR, MAX_COLOR, MIN_COLOR },  // beatValue == 0
		{ MAX_COLOR, MIN_COLOR, MIN_COLOR },  // beatValue == 1
		{ MIN_COLOR, MAX_COLOR, MAX_COLOR },  // beatValue == 2
		{ MAX_COLOR, MAX_COLOR, MIN_COLOR }   // beatValue == 3
	};

	GO_Vector3 lightColor = { 0, 0, 0 };

	// Ensure beatValue is within the expected range and assign corresponding color
	if (beatValue >= 0 && beatValue < numberOfBeatsInBar) {
		lightColor = beatColors[beatValue];
	}

	// Scale the light color by barColorIntensity
	lightColor = lightColor * barColorIntensity;

	renderManager.SetLightColor(lightColor);
}

void Game::MusicBeat() {
	// This method is called by a Wwise callback at the start of every beat in the music.
	beatValue += 1;
	if (beatValue > numberOfBeatsInBar - 1)
	{
		beatValue = 0;
	}
	std::string beatValueStr = "Beat: " + std::to_string(beatValue + 1) + "/" + std::to_string(numberOfBeatsInBar);
	renderManager.SetBeatValue(beatValueStr);
}

void Game::MusicBar(const float& in_barDuration) {
	// This method is called by a Wwise callback at the start of every bar in the music.
	timeLeftOnBar = in_barDuration;
	barDuration = in_barDuration;
}

void Game::SetDiffractionPaths(const std::vector<DiffractionPath> in_diffractionPaths) {
	diffractionPaths = in_diffractionPaths;
}

Game::~Game()
{

}
