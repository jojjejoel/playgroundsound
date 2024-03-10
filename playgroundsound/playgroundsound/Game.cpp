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

Light lights[4];
void Game::Run()
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
	}if (IsKeyPressed(KEY_TWO))
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

	UpdateCamera(camera.get(), CAMERA_THIRD_PERSON);
	cameraGameObject->SetPosition({ camera->position.x, camera->position.y, camera->position.z });
	playerGameObject->SetPosition({ camera->target.x, camera->target.y, camera->target.z });
	Matrix matrix = GetCameraMatrix(*camera);

	cameraGameObject->SetUp({ matrix.m1, matrix.m5, matrix.m9 });
	cameraGameObject->SetForward({ matrix.m2, matrix.m6, matrix.m10 });

	playerGameObject->SetScale({ 0.5f, 0.5f, 0.5f });

	barValue -= GetFrameTime() * 0.37f * playbackSpeed;
	std::cout << "Beat: " << beatValue << "Bar: " << barValue << std::endl;
	UpdateBlinkingLight();

	BeginDrawing();

	ClearBackground(RAYWHITE);
	BeginMode3D(*camera);

	DrawModel(*models[3], { 0,0,20 }, 1, BLACK);
	DrawModel(*models[3], { 0,0,-20 }, 1, BLACK);
	DrawModel(*models[4], { 20,0,0 }, 1, BLACK);
	DrawModel(*models[4], { -20,0,0 }, 1, BLACK);
	DrawModel(*models[5], { 0,-2,0 }, 1, WHITE);
	DrawModel(*models[5], { 0,8,0 }, 1, WHITE);

	DrawSphereWires({ 0,0,0 }, 0.5f, 10, 10, { 0, 255, 0, 255 });

	DrawModel(*models[0], camera->target, 1, WHITE);
	for (size_t i = 1; i < models.size() - 4; i++)
	{
		Color color;
		if (i == 0)
		{
			color = { 255,255,255,255 };
		}
		else
		{
			color = { 0,255,255,255 };
		}
		DrawModel(*models[i], { 0,0,0 }, 1, WHITE);
	}

	if (!updateSecondLight || lightFlickerValue == 0)
	{
		DrawModel(*models[models.size() - 1], { 0,0,0 }, 1, WHITE);
	}

	DrawDiffractionPaths();
	EndMode3D();

	std::string playbackSpeedString = "Playback speed: " + std::to_string(playbackSpeed);

	DrawText(playbackSpeedString.c_str(), 2, 2, 20, WHITE);

	EndDrawing();
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
			DrawLine3D(firstNodePos, camera->target, color);
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
			Vector3 endLinePos = { camera->target };
			DrawLine3D(startLinePos, endLinePos, color);
		}
	}
}

void Game::UpdateBlinkingLight()
{
	lights[0].color = { 0,0,0, 0 };
	lights[1].color = { 0,0,0, 0 };

	if (updateFirstLight)
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
	}
	if (updateSecondLight)
	{
		unsigned char r = static_cast<unsigned char>(155 * (1 - lightFlickerValue));
		unsigned char g = static_cast<unsigned char>(255 * (1 - lightFlickerValue));
		unsigned char b = static_cast<unsigned char>(50 * (1 - lightFlickerValue));
		lights[1].color = { r,g,b, 150 };
		lights[1].attenuation = 0;
	}

	UpdateLightValues(models[0]->materials[0].shader, lights[0]);
	UpdateLightValues(models[0]->materials[0].shader, lights[1]);
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

void Game::Init()
{
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "PlaygroundSound");


	Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");

	SetShaderValue(shader, ambientLoc, static_cast<const void*>(new float[4] {0.1f, 0.0f, 0.0f, 1.0f}), SHADER_UNIFORM_VEC4);

	lights[0] = CreateLight(LIGHT_POINT, { -5, 1, -5 }, { 5,0,5 }, { 0, 0, 0, 0 }, shader);
	lights[1] = CreateLight(LIGHT_DIRECTIONAL, { 10, 1, 10 }, { 0,1,0 }, { 0, 0, 0, 0 }, shader);


	camera = std::make_shared<Camera3D>();
	camera->position = { 0.0f, 2.0f, 4.0f };
	camera->target = { 0.0f, .0f, 0.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->fovy = 60.0f;
	camera->projection = CAMERA_PERSPECTIVE;
	CameraMoveToTarget(camera.get(), -2.f);

	cameraGameObject = std::make_shared<GameObject>();

	playerGameObject = std::make_shared<GameObject>();

	Model model = LoadModel("Resources/Models/truck_green.obj");

	model.materials[0].shader = shader;


	std::shared_ptr<Model> modelPtr = std::make_shared<Model>(model);
	std::shared_ptr<Shader> shaderPtr = std::make_shared<Shader>(shader);
	models.emplace_back(modelPtr);

	GoTransform transform;
	transform.position = { 0,0,0 };
	transform.scale = { 5,5,5 };
	AddCube(transform, shaderPtr);

	transform.position = { 0,0,2.5f };
	transform.scale = { 2,3,2 };
	AddCube(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 40,15,3 };
	AddCube(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 3,15,40 };
	AddCube(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 40,1,40 };
	AddCube(transform, shaderPtr);


	//Light Bulb
	transform.position = { 10,1,-10 };
	transform.scale = { 1,1,1 };
	AddCube(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 5,5,1 };
	AddWall(transform, 0);

	SetTargetFPS(30);
}

void Game::AddCube(const GoTransform& transform, const std::shared_ptr<Shader>& shader)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	*model = LoadModelFromMesh(GenMeshCube(transform.scale.x, transform.scale.y, transform.scale.z));
	model->transform.m12 = transform.position.x;
	model->transform.m13 = transform.position.y;
	model->transform.m14 = transform.position.z;
	model->materials[0].shader = *shader.get();
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

	gameObject->SetTransform(transform);
	ConvertVertices(model, gameObject);

	ConvertTriangles(model, gameObject);

	soundBlockingObjects.emplace_back(gameObject);
	models.emplace_back(model);
}

void Game::ConvertVertices(std::shared_ptr<Model>& model, std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0, v = 0; i < model->meshes[0].vertexCount; i++, v += 3)
	{
		Playground::Vertex vertex;
		float* vertices = model->meshes[0].vertices;
		Vector3 vector3;
		vector3.x = vertices[v];
		vector3.y = vertices[v + 1];
		vector3.z = vertices[v + 2];
		vector3 = Vector3Transform(vector3, model->transform);

		vertex.x = vector3.x;
		vertex.y = vector3.y;
		vertex.z = -vector3.z;
		gameObject->mesh.vertices.emplace_back(vertex);
	}
}

void Game::ConvertTriangles(std::shared_ptr<Model>& model, std::shared_ptr<GameObject>& gameObject)
{
	for (size_t i = 0, v = 0; i < model->meshes[0].triangleCount; i++, v += 3)
	{
		Triangle triangle;
		triangle.point0 = model->meshes[0].indices[v];
		triangle.point1 = model->meshes[0].indices[v + 1];
		triangle.point2 = model->meshes[0].indices[v + 2];
		gameObject->triangles.emplace_back(triangle);
	}
}

void Game::AddWall(const GoTransform& transform, const float& radians)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	*model = LoadModelFromMesh(GenMeshPlane(transform.scale.x, transform.scale.y, 10, 10));
	model->transform.m12 = transform.position.x;
	model->transform.m13 = transform.position.y;
	model->transform.m14 = transform.position.z;

	model->transform = MatrixMultiply(MatrixRotateZ(1.5708f), model->transform);
	model->transform = MatrixMultiply(MatrixRotateX(radians), model->transform);

	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

	gameObject->SetTransform(transform);
	ConvertVertices(model, gameObject);

	ConvertTriangles(model, gameObject);

	roomWalls.emplace_back(gameObject);
}

void Game::SetLightFlickerValue(const float& value) {
	lightFlickerValue = value;
}

void Game::DeInit()
{
	CloseWindow();
}

const std::shared_ptr<GameObject> Game::GetLightBulbGameObject()
{
	return soundBlockingObjects[soundBlockingObjects.size() - 1];
}

const std::shared_ptr<GameObject> Game::GetCameraGameObject()
{
	return cameraGameObject;
}

const std::shared_ptr<GameObject> Game::GetPlayerGameObject()
{
	return playerGameObject;
}

const std::vector<std::shared_ptr<GameObject>>& Game::GetSoundBlockingObjects()
{
	return soundBlockingObjects;
}

const std::vector<std::shared_ptr<GameObject>>& Game::GetWalls()
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

bool Game::IsGameObjectInRoom(const std::shared_ptr<GameObject>& gameObject)
{
	GameObject roomGameObject = *soundBlockingObjects[0];

	BoundingBox boxRoom = CalculateBoundingBox({ roomGameObject.GetPosition().x, roomGameObject.GetPosition().y, roomGameObject.GetPosition().z },
		roomGameObject.GetScale().x, roomGameObject.GetScale().y, roomGameObject.GetScale().z);

	BoundingBox boxPlayer = CalculateBoundingBox({ gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z },
		gameObject->GetScale().x, gameObject->GetScale().y, gameObject->GetScale().z);

	return CheckCollisionBoxes(boxRoom, boxPlayer);
}

void Game::AssignRtpcFunction(std::function<void(const float&)> function)
{
	setRtpcFunction = function;
}
