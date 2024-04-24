#include "Game.h"

#include "raylib.h"
#include "rcamera.h"
#include <string>
#include "raymath.h"
#include <math.h>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define GLSL_VERSION            330

#include <algorithm>
#include <iostream>
#include "GameObjectIDs.h"
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

	GO_Transform transform;
	transform.position = { 0,0,0 };
	transform.scale = { 5,5,1 };
	AddWall(transform, 0);

	gameObjectManager.m_gameObjects["RoomCube"]->AddComponent<WwiseRoomComponent>().InitRoom(gameObjectManager.m_gameObjects["RoomWall"]);
	WwiseRoomComponent& roomComponent = gameObjectManager.m_gameObjects["RoomCube"]->GetComponent<WwiseRoomComponent>();
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
	gameObjectManager.m_gameObjects["Music"]->GetComponent<WwiseObjectComponent>().PostEvent(AK::EVENTS::GOOD_OLD_DAYS);
}

void Game::AddShader()
{
	Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");

	SetShaderValue(shader, ambientLoc, static_cast<const void*>(new float[4] {0.1f, 0.0f, 0.0f, 1.0f}), SHADER_UNIFORM_VEC4);

	shaders.insert(std::make_pair("lighting", std::make_shared<Shader>(shader)));

	lights[0] = CreateLight(LIGHT_POINT, { -5, 1, -5 }, { 5,0,5 }, { 0, 0, 0, 0 }, shader);
	lights[1] = CreateLight(LIGHT_DIRECTIONAL, { 10, 1, 10 }, { 0,1,0 }, { 0, 0, 0, 0 }, shader);
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
	roomCubeObj->AddComponent<RenderComponent>().SetModel(models["RoomCube"].get(), true);

	GameObject* roomWallObj = gameObjectManager.AddGameObject("RoomWall");
	roomWallObj->AddComponent<RenderComponent>().SetModel(models["RoomWall"].get(), true);

	GameObject* musicEmitterObj = gameObjectManager.AddGameObject("Music");
	musicEmitterObj->AddComponent<WwiseObjectComponent>();

	GameObject* portalCubeObj = gameObjectManager.AddGameObject("PortalCube");
	portalCubeObj->m_transform.position = { 0,0,2.5f };
	portalCubeObj->AddComponent<RenderComponent>().SetModel(models["PortalCube"].get());
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

	models.insert(std::make_pair("PortalCube", std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(2, 3, 2)))));
	models["PortalCube"]->materials[0].shader = *shaders["lighting"];

	Model model = LoadModelFromMesh(GenMeshPlane(10, 10, 20, 20));
	model.transform = MatrixMultiply(MatrixRotateZ(1.5708f), model.transform);
	models.insert(std::make_pair("RoomWall", std::make_shared<Model>(model)));
	models["RoomWall"]->materials[0].shader = *shaders["lighting"];
}

void Game::Run()
{
	InputControls();

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
	DrawDiffractionPaths();
	EndMode3D();

	std::string playbackSpeedString = "Playback speed: " + std::to_string(playbackSpeed);
	DrawText(playbackSpeedString.c_str(), 2, 2, 20, WHITE);

	EndDrawing();
}

void Game::InputControls()
{
	if (IsKeyPressed(KEY_F))
	{
		DisableCursor();
	}
	if (IsKeyPressed(KEY_G))
	{
		EnableCursor();
	}
	if (IsKeyPressed(KEY_ONE))
	{
		updateFirstLight = !updateFirstLight;
	}
	if (IsKeyPressed(KEY_TWO))
	{
		updateSecondLight = !updateSecondLight;
	}
	if (IsKeyDown(KEY_THREE))
	{
		playbackSpeed -= GetFrameTime();
		setRtpcFunction(playbackSpeed);
	}
	if (IsKeyDown(KEY_FOUR))
	{
		playbackSpeed += GetFrameTime();
		setRtpcFunction(playbackSpeed);
	}

	if (IsKeyDown(KEY_FOUR))
	{
		playbackSpeed += GetFrameTime();
		setRtpcFunction(playbackSpeed);
	}
}

void Game::DrawDiffractionPaths()
{
	for (int pathIndex = 0; pathIndex < diffractionPaths.size(); pathIndex++)
	{
		DiffractionPath diffractionPath = diffractionPaths[pathIndex];

		Color color = { static_cast<unsigned char>(Lerp(0, 255, diffractionPath.diffraction)),
						static_cast<unsigned char>(Lerp(255, 0, diffractionPath.diffraction)),
						0, 255 };
		int numNodes = diffractionPath.nodeCount;
		if (numNodes > 0)
		{
			Vector3 firstNodePos = { diffractionPath.nodes[0].x, diffractionPath.nodes[0].y, diffractionPath.nodes[0].z };
			//DrawLine3D(firstNodePos, camera->target, color);
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
		else
		{
			Vector3 startLinePos = { diffractionPath.emitterPos.x, diffractionPath.emitterPos.y, diffractionPath.emitterPos.z };
			//Vector3 endLinePos = { camera->target };
			//DrawLine3D(startLinePos, endLinePos, color);
		}
	}
}

void Game::UpdateBlinkingLight()
{
	lights[0].color = { 255,255,255, 255 };
	lights[1].color = { 255,255,255, 255 };

	/*if (updateFirstLight)
	{
		unsigned char gValue = static_cast<unsigned char>(255 * std::max(0.0f, barValue));

		if (beatValue == 0)
		{
			unsigned char aValueBar = static_cast<unsigned char>(255 * std::max(0.0f, barValue));
			lights[0].color = { aValueBar,0,gValue, 255 };
		}
		else if (beatValue == 1)
		{
			lights[0].color = { 0,0,gValue, 255 };
		}
		else if (beatValue == 2)
		{
			unsigned char aValueBar = static_cast<unsigned char>(255 * std::max(0.0f, barValue));
			lights[0].color = { 0,aValueBar,gValue, 255 };
		}
		else if (beatValue == 3)
		{
			unsigned char aValueBar = static_cast<unsigned char>(100 * std::max(0.0f, barValue));
			lights[0].color = { aValueBar,aValueBar,gValue, 255 };
		}
	}*/
	/*if (updateSecondLight)
	{
		unsigned char r = static_cast<unsigned char>(155 * (1 - lightFlickerValue));
		unsigned char g = static_cast<unsigned char>(255 * (1 - lightFlickerValue));
		unsigned char b = static_cast<unsigned char>(50 * (1 - lightFlickerValue));
		lights[1].color = { r,g,b, 150 };
		lights[1].attenuation = 0;
		gameObjectManager.m_gameObjects["LightBulb"]->SetScaleMultiplier(1 - lightFlickerValue);
	}*/

	UpdateLightValues(*shaders["lighting"], lights[0]);
	UpdateLightValues(*shaders["lighting"], lights[1]);
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

void Game::AddGameObject(std::shared_ptr<Model> model, const unsigned int& goID, const std::string_view name, const GO_Vector3& in_position, const GO_Vector3& in_scale)
{
	OldGameObject gameObject;
	gameObject.SetName(name);
	gameObject.SetPosition(in_position);
	gameObject.SetScale(in_scale);
	if (model != nullptr)
	{
		ConvertVertices(model, gameObject);

		ConvertTriangles(model, gameObject);

	}

	gameObject.SetModel(model);
	gameObject.SetID(goID);

	//gameObjects.insert(std::make_pair(goID, std::make_shared<OldGameObject>(gameObject)));
}

void Game::ConvertVertices(const std::shared_ptr <Model>& model, OldGameObject& gameObject)
{
	for (size_t i = 0, v = 0; i < model->meshes[0].vertexCount; i++, v += 3)
	{
		GoVertex vertex;
		float* vertices = model->meshes[0].vertices;
		Vector3 vector3;
		vector3.x = vertices[v];
		vector3.y = vertices[v + 1];
		vector3.z = vertices[v + 2];
		vector3 = Vector3Transform(vector3, model->transform);

		vertex.x = vector3.x;
		vertex.y = vector3.y;
		vertex.z = -vector3.z;
		gameObject.mesh.vertices.emplace_back(vertex);
	}
}

void Game::ConvertTriangles(const std::shared_ptr <Model>& model, OldGameObject& ref_gameObject)
{
	for (size_t i = 0, v = 0; i < model->meshes[0].triangleCount; i++, v += 3)
	{
		Triangle triangle;
		triangle.point0 = model->meshes[0].indices[v];
		triangle.point1 = model->meshes[0].indices[v + 1];
		triangle.point2 = model->meshes[0].indices[v + 2];
		ref_gameObject.triangles.emplace_back(triangle);
	}
}

void Game::AddWall(const GO_Transform& transform, const float& radians)
{
	Model model = LoadModelFromMesh(GenMeshPlane(transform.scale.x, transform.scale.y, 20, 20));
	model.transform.m12 = transform.position.x;
	model.transform.m13 = transform.position.y;
	model.transform.m14 = transform.position.z;

	model.transform = MatrixMultiply(MatrixRotateZ(1.5708f), model.transform);
	model.transform = MatrixMultiply(MatrixRotateX(radians), model.transform);

	OldGameObject gameObject;

	gameObject.SetTransform(transform);
	std::shared_ptr<Model> modelPtr = std::make_shared<Model>(model);
	ConvertVertices(modelPtr, gameObject);

	ConvertTriangles(modelPtr, gameObject);

	roomWalls.emplace_back(std::make_shared<OldGameObject>(gameObject));
}

void Game::SetLightFlickerValue(const float& value) {
	lightFlickerValue = value;
}

void Game::DeInit()
{
	CloseWindow();
}

const std::shared_ptr<OldGameObject>& Game::GetGameObject(const unsigned int& goID) {
	//if (gameObjects.find(goID) == gameObjects.end()) {
	//	return nullptr;
	//}
	//else {
	//	return gameObjects[goID];
	//}
	return nullptr;
}

const std::vector<std::shared_ptr<OldGameObject>>& Game::GetWalls()
{
	return roomWalls;
}

BoundingBox Game::CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const {
	BoundingBox boundingBox;

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfLength = length / 2.0f;

	boundingBox.min.x = center.x - halfWidth;
	boundingBox.min.y = center.y - halfHeight;
	boundingBox.min.z = center.z - halfLength;

	boundingBox.max.x = center.x + halfWidth;
	boundingBox.max.y = center.y + halfHeight;
	boundingBox.max.z = center.z + halfLength;

	return boundingBox;
}



void Game::AssignRtpcFunction(std::function<void(const float&)> function)
{
	setRtpcFunction = function;
}

const std::map<unsigned int, std::shared_ptr<OldGameObject>>& Game::GetAllGameObjects()
{
	std::map<unsigned int, std::shared_ptr<OldGameObject>> map;
	return map;
	//return gameObjects;
}

void Game::DrawGameObject(std::shared_ptr<OldGameObject> in_gameObject) {
	if (in_gameObject->GetModel())
	{
		DrawModel(*in_gameObject->GetModel(), ConvertGoVector3(in_gameObject->GetPosition()), in_gameObject->GetScaleMultiplier(), WHITE);
	}
}

Vector3 Game::ConvertGoVector3(const GO_Vector3& in_goVector) {
	return { in_goVector.x, in_goVector.y, in_goVector.z };
}