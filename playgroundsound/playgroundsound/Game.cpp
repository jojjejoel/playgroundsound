#include "Game.h"

/*******************************************************************************************
*
*   raylib [core] example - 3d camera first person
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "rcamera.h"
#include <string>
#include "raymath.h"
#include <math.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#include <algorithm>
#include <iostream>
#include "Components.h"
#include "Wwise_IDs.h"

Light lights[4];

void Game::Init()
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "PlaygroundSound");

	AddShader();

	LoadModels();

	AddGameObjects();


	WwiseRoomComponent& roomComponent = gameObjectManager.m_gameObjects["RoomCube"]->AddComponent<WwiseRoomComponent>();
	roomComponent.InitRoomGeometry(gameObjectManager.m_gameObjects["RoomCube"]);
	roomComponent.InitRoom(gameObjectManager.m_gameObjects["RoomWall"]);
	roomComponent.SetBoundingBox({ 0,0,0 }, 10, 10, 10);
	wwiseRoomManager.AddRoom(&roomComponent);
	wwiseRoomManager.AddObject(&gameObjectManager.m_gameObjects["Truck"]->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(&gameObjectManager.m_gameObjects["Camera"]->GetComponent<WwiseObjectComponent>());
	wwiseRoomManager.AddObject(&gameObjectManager.m_gameObjects["Music"]->GetComponent<WwiseObjectComponent>());
	SetTargetFPS(30);
	GameObject* portalObj = gameObjectManager.m_gameObjects["PortalCube"];
	portalObj->GetComponent<WwisePortalComponent>().InitPortal(portalObj, roomComponent.GetRoomID());

	gameObjectManager.Init();
	WwiseObjectComponent comp = gameObjectManager.m_gameObjects["Camera"]->GetComponent<WwiseObjectComponent>();
	comp.RegisterAsListener();
	gameObjectManager.m_gameObjects["Truck"]->GetComponent<WwiseObjectComponent>().PostEvent(AK::EVENTS::CAR_ENGINE_LOOP);
	gameObjectManager.m_gameObjects["Truck"]->GetComponent<WwiseObjectComponent>().RegisterAsDistanceProbe(gameObjectManager.m_gameObjects["Camera"]->m_id);
	gameObjectManager.m_gameObjects["Music"]->GetComponent<WwiseObjectComponent>().PostMusicEvent(AK::EVENTS::ENERGY, std::bind(&Game::MusicBar, this), std::bind(&Game::MusicBeat, this));
}

void Game::AddShader()
{
	Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");

	SetShaderValue(shader, ambientLoc, static_cast<const void*>(new float[4] {0.1f, 0.0f, 0.0f, 1.0f}), SHADER_UNIFORM_VEC4);

	shaders.insert(std::make_pair("lighting", std::make_shared<Shader>(shader)));

	lights[0] = CreateLight(LIGHT_POINT, { -5, 1, -5 }, { 5,0,5 }, { 255,255, 255, 255 }, shader);
	lights[1] = CreateLight(LIGHT_DIRECTIONAL, { 10, 1, 10 }, { 0,1,0 }, { 255, 255, 255, 255 }, shader);
}

void Game::AddGameObjects()
{
	GameObject* truckObj = gameObjectManager.AddGameObject("Truck");
	truckObj->AddComponent<ControllerComponent>().SetMovementSpeed(5);
	truckObj->AddComponent<RenderComponent>().SetModel(models["truck_green"].get());
	truckObj->AddComponent<WwiseObjectComponent>();

	GameObject* cameraObj = gameObjectManager.AddGameObject("Camera");
	cameraObj->AddComponent<CameraComponent>().SetTarget(truckObj);
	cameraObj->AddComponent<WwiseObjectComponent>();

	GameObject* roomCubeObj = gameObjectManager.AddGameObject("RoomCube");
	roomCubeObj->AddComponent<RenderComponent>().SetModel(models["RoomCube"].get(), false, true);

	GameObject* roomWallObj = gameObjectManager.AddGameObject("RoomWall");
	roomWallObj->AddComponent<RenderComponent>().SetModel(models["RoomWall"].get(), false, true);

	GameObject* musicEmitterObj = gameObjectManager.AddGameObject("Music");
	musicEmitterObj->AddComponent<WwiseObjectComponent>();

	GameObject* portalCubeObj = gameObjectManager.AddGameObject("PortalCube");
	portalCubeObj->m_transform.position = { 0,0,5 };
	portalCubeObj->m_transform.scale = { 4,6,2 };
	portalCubeObj->AddComponent<RenderComponent>().SetModel(models["PortalCube"].get(), false);
	portalCubeObj->AddComponent<WwisePortalComponent>();

	GameObject* wallFrontObj = gameObjectManager.AddGameObject("WallFront");
	wallFrontObj->AddComponent<RenderComponent>().SetModel(models["WallFront"].get());
	wallFrontObj->m_transform.position = { 0,0,20 };

	GameObject* wallBackObj = gameObjectManager.AddGameObject("WallBack");
	wallBackObj->AddComponent<RenderComponent>().SetModel(models["WallFront"].get());
	wallBackObj->m_transform.position = { 0,0,-20 };

	GameObject* wallRightObj = gameObjectManager.AddGameObject("WallRight");
	wallRightObj->AddComponent<RenderComponent>().SetModel(models["WallSide"].get());
	wallRightObj->m_transform.position = { 20,0,0 };

	GameObject* wallLeftObj = gameObjectManager.AddGameObject("WallLeft");
	wallLeftObj->AddComponent<RenderComponent>().SetModel(models["WallSide"].get());
	wallLeftObj->m_transform.position = { -20,0,0 };

	GameObject* wallBottomObj = gameObjectManager.AddGameObject("WallBottom");
	wallBottomObj->AddComponent<RenderComponent>().SetModel(models["WallTop"].get());
	wallBottomObj->m_transform.position = { 0,-2,0 };

	GameObject* wallTopObj = gameObjectManager.AddGameObject("WallTop");
	wallTopObj->AddComponent<RenderComponent>().SetModel(models["WallTop"].get());
	wallTopObj->m_transform.position = { 0,8,0 };

	GameObject* lightBulbObj = gameObjectManager.AddGameObject("LightBulb");
	lightBulbObj->AddComponent<RenderComponent>().SetModel(models["PortalCube"].get());
	lightBulbObj->m_transform.position = { 10,1,10 };
}

void Game::LoadModels()
{
	models.insert(std::make_pair("truck_green", std::make_shared<Model>(LoadModel("Resources/Models/truck_green.obj"))));
	models["truck_green"]->materials[0].shader = *shaders["lighting"];

	models.insert(std::make_pair("WallSide", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(3, 15, 40)))));
	models["WallSide"]->materials[0].shader = *shaders["lighting"];

	models.insert(std::make_pair("WallFront", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(40, 15, 3)))));
	models["WallFront"]->materials[0].shader = *shaders["lighting"];

	models.insert(std::make_pair("WallTop", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(40, 3, 40)))));
	models["WallTop"]->materials[0].shader = *shaders["lighting"];

	models.insert(std::make_pair("RoomCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(10, 10, 10)))));
	models["RoomCube"]->materials[0].shader = *shaders["lighting"];

	models.insert(std::make_pair("PortalCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(4, 6, 2)))));
	models["PortalCube"]->materials[0].shader = *shaders["lighting"];

	Model model = LoadModelFromMesh(GenMeshPlane(10, 10, 10, 20));
	model.transform = MatrixMultiply(MatrixRotateZ(1.5708f), model.transform);
	models.insert(std::make_pair("RoomWall", std::make_shared<Model>(model)));
	models["RoomWall"]->materials[0].shader = *shaders["lighting"];
}

void Game::Run()
{

	barValue -= GetFrameTime() * 0.37f * playbackSpeed;
	UpdateBlinkingLight();
	float carSpeed = gameObjectManager.m_gameObjects["Truck"]->GetComponent<ControllerComponent>().GetPercentageOfMaxSpeed();
	gameObjectManager.m_gameObjects["Truck"]->GetComponent<WwiseObjectComponent>().SetRTPC(AK::GAME_PARAMETERS::CAR_SPEED, carSpeed);

	float carGas = gameObjectManager.m_gameObjects["Truck"]->GetComponent<ControllerComponent>().GetGas();
	gameObjectManager.m_gameObjects["Truck"]->GetComponent<WwiseObjectComponent>().SetRTPC(AK::GAME_PARAMETERS::CAR_GAS, carGas);



	BeginDrawing();

	ClearBackground(RAYWHITE);
	BeginMode3D(*gameObjectManager.m_gameObjects["Camera"]->GetComponent<CameraComponent>().camera3D);

	gameObjectManager.Update();
	wwiseRoomManager.Update();
	

	std::vector<DiffractionPath> diffractionPaths = diffractionManager.GetDiffractionPath(gameObjectManager.m_gameObjects["Music"]->m_id);
	GO_Vector3 listenerPosGo = gameObjectManager.m_gameObjects["Truck"]->m_transform.position;

	Vector3 listenerPos = { listenerPosGo.x, listenerPosGo.y, listenerPosGo.z };
		for (int pathIndex = 0; pathIndex < diffractionPaths.size(); pathIndex++)
	{
		DiffractionPath diffractionPath = diffractionPaths[pathIndex];

		Color color = { Lerp(0, 255, diffractionPath.diffraction), Lerp(255, 0, diffractionPath.diffraction), 0, 255 };
		int numNodes = diffractionPath.nodeCount;
		if (numNodes > 0)
		{
			Vector3 firstNodePos = { diffractionPath.nodes[0].x, diffractionPath.nodes[0].y, diffractionPath.nodes[0].z };
			DrawLine3D(firstNodePos, listenerPos, color);
			DrawSphereWires(firstNodePos, 0.2f, 10, 10, color);

			for (int nodeIndex = 1; nodeIndex < numNodes; nodeIndex++)
			{
				Vector3 nodePos = { diffractionPath.nodes[nodeIndex].x, diffractionPath.nodes[nodeIndex].y, diffractionPath.nodes[nodeIndex].z };
				Vector3 emitterPos = { diffractionPath.nodes[nodeIndex - 1].x, diffractionPath.nodes[nodeIndex - 1].y, diffractionPath.nodes[nodeIndex - 1].z };

				DrawSphereWires(nodePos, 0.2f, 10, 10, color);
				DrawLine3D(emitterPos, nodePos, color);
			}
			Vector3 emitterPos = { diffractionPath.emitterPos.x, diffractionPath.emitterPos.y, diffractionPath.emitterPos.z };
			Vector3 nodePos = { diffractionPath.nodes[numNodes - 1].x, diffractionPath.nodes[numNodes - 1].y, diffractionPath.nodes[numNodes - 1].z };
			DrawLine3D(emitterPos, nodePos, color);
		}
		/*else
		{
			Vector3 startLinePos = { diffractionPath.emitterPos.x, diffractionPath.emitterPos.y, diffractionPath.emitterPos.z };
			Vector3 endLinePos = { listenerPos };
			DrawLine3D(startLinePos, endLinePos, color);
		}*/
	}
	
		EndMode3D();

		std::string playbackSpeedString = "Playback speed: " + std::to_string(playbackSpeed);
		DrawText(playbackSpeedString.c_str(), 2, 2, 20, WHITE);
	EndDrawing();
}

void Game::UpdateBlinkingLight()
{
	lights[0].color = { 255,255,255, 255 };
	lights[1].color = { 255,255,255, 255 };

	if (updateFirstLight)
	{

	unsigned char gValue = 255 * std::max(0.0f, barValue);

	if (beatValue == 0)
	{
		unsigned char aValueBar = 255 * std::max(0.0f, barValue);
		lights[0].color = { aValueBar,0,gValue, 255 };
	}
	else if (beatValue == 1)
	{
		lights[0].color = { 0,0,gValue, 255 };
	}
	else if (beatValue == 2)
	{
		unsigned char aValueBar = 255 * std::max(0.0f, barValue);
		lights[0].color = { 0,aValueBar,gValue, 255 };
	}
	else if (beatValue == 3)
	{
		unsigned char aValueBar = 100 * std::max(0.0f, barValue);
		lights[0].color = { aValueBar,aValueBar,gValue, 255 };
	}
	}
	/*if (updateSecondLight)
	{
	unsigned char r = 155 * (1 - lightFlickerValue);
	unsigned char g = 255 * (1 - lightFlickerValue);
	unsigned char b = 50 * (1 - lightFlickerValue);
	lights[1].color = { r,g,b, 150 };
	lights[1].attenuation = 0;

	}*/


	UpdateLightValues(models["truck_green"]->materials[0].shader, lights[0]);

	//UpdateLightValues(models[0]->materials[0].shader, lights[1]);

}

void Game::MusicBeat() {
	beatValue += 1;
	if (beatValue > 3)
	{
		beatValue = 0;
	}
}

void Game::MusicBar() {
	barValue = 1;
}

void Game::SetDiffractionPaths(const std::vector<DiffractionPath> in_diffractionPaths) {
	diffractionPaths = in_diffractionPaths;
}

void Game::SetLightFlickerValue(const float& value) {
	lightFlickerValue = value;
}

void Game::DeInit()
{
	CloseWindow();
}


