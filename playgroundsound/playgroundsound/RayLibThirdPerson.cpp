#include "RayLibThirdPerson.h"

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

Light lights[4];
void RayLibThirdPerson::Run()
{
	if (IsKeyPressed(KEY_F))
	{
		DisableCursor();
	}
	if (IsKeyPressed(KEY_G))
	{
		EnableCursor();
	}

	UpdateCamera(camera.get(), cameraMode);
	cameraGameObject->SetPosition({ camera->position.x, camera->position.y, -camera->position.z });
	playerGameObject->SetPosition({ camera->target.x, camera->target.y, -camera->target.z });
	Matrix matrix = GetCameraMatrix(*camera);

	cameraGameObject->SetUp({ matrix.m1 * upX, matrix.m5 * upY, matrix.m9 * upZ });
	cameraGameObject->SetForward({ matrix.m2 * forwardX, matrix.m6 * forwardY, matrix.m10 * forwardZ });

	playerGameObject->SetScale({ 0.5f, 0.5f, 0.5f });
	playerGameObject->SetUp({ 0, 1, 0 });
	playerGameObject->SetForward({ 0, 0, 1 });

	barValue -= GetFrameTime() * 0.4;
	std::cout << "Beat: " << beatValue << "Bar: " << barValue << std::endl;

	BeginDrawing();

	ClearBackground(RAYWHITE);
	BeginMode3D(*camera);

	UpdateBlinkingLight();

	DrawModel(*models[3], { 0,0,20 }, 1, BLACK);
	DrawModel(*models[3], { 0,0,-20 }, 1, BLACK);
	DrawModel(*models[4], { 20,0,0 }, 1, BLACK);
	DrawModel(*models[4], { -20,0,0 }, 1, BLACK);
	DrawModel(*models[5], { 0,-2,0 }, 1, WHITE);
	DrawModel(*models[5], { 0,8,0 }, 1, WHITE);


	DrawSphereWires({ 0,0,0 }, 0.5f, 10, 10, { 0, 255, 0, 255 });

	

	DrawModel(*models[0], camera->target, 1, WHITE);
	for (size_t i = 1; i < models.size() - 3; i++)
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

	DrawDiffractionPaths();
	EndMode3D();

	EndDrawing();
}

void RayLibThirdPerson::DrawDiffractionPaths()
{
	for (int pathIndex = 0; pathIndex < diffractionPaths.size(); pathIndex++)
	{
		DiffractionPath diffractionPath = diffractionPaths[pathIndex];

		Color color = { Lerp(0, 255, diffractionPath.diffraction), Lerp(255, 0, diffractionPath.diffraction), 0, 255 };
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

void RayLibThirdPerson::UpdateBlinkingLight()
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

	for (auto& model : models)
	{
		UpdateLightValues(model->materials[0].shader, lights[0]);
	}
}

void RayLibThirdPerson::MusicBeat() {
	beatValue += 1;
	if (beatValue > 3)
	{
		beatValue = 0;
	}
}

void RayLibThirdPerson::MusicBar() {
	barValue = 1;
}

RayCollision RayLibThirdPerson::CheckCollisions()
{
	std::vector<RayCollision> collisions;
	for (const auto& model : models)
	{
		Ray ray;
		ray.position = camera->target;
		Vector3 dir;
		dir.x = -camera->target.x;
		dir.y = -camera->target.y;
		dir.z = -camera->target.z;
		ray.direction = dir;
		RayCollision collision = GetRayCollisionBox(ray, GetModelBoundingBox(*model));
		if (collision.hit)
		{
			collisions.push_back(collision);
		}
	}

	if (collisions.size() > 0)
	{
		RayCollision shortestDistanceCollision = collisions[0];
		for (size_t i = 1; i < collisions.size(); i++)
		{
			if (collisions[i].distance < shortestDistanceCollision.distance)
			{
				shortestDistanceCollision = collisions[i];
			}
		}
		return shortestDistanceCollision;
	}
	else
	{
		RayCollision rayCollision;
		rayCollision.hit = false;
		return rayCollision;
	}
}

void RayLibThirdPerson::SetDiffractionPaths(const std::vector<DiffractionPath> in_diffractionPaths) {
	diffractionPaths = in_diffractionPaths;
}

void RayLibThirdPerson::Init()
{
	cameraMode = CAMERA_THIRD_PERSON;
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "PlaygroundSound");


	Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION), TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

	shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
	int ambientLoc = GetShaderLocation(shader, "ambient");

	SetShaderValue(shader, ambientLoc, static_cast<const void*>(new float[4] {0.1f, 0.1f, 0.1f, 1.0f}), SHADER_UNIFORM_VEC4);

	lights[0] = CreateLight(LIGHT_POINT, { -5, 1, -5 }, { 5,0,5 }, { YELLOW.r, YELLOW.g, YELLOW.b, 100 }, shader);


	camera = std::make_shared<Camera3D>();
	camera->position = { 0.0f, 2.0f, 4.0f };
	camera->target = { 0.0f, .0f, 0.0f };
	camera->up = { 0.0f, 1.0f, 0.0f };
	camera->fovy = 60.0f;
	camera->projection = CAMERA_PERSPECTIVE;
	CameraMoveToTarget(camera.get(), -2.f);

	cameraGameObject = std::make_shared<GameObject>();
	cameraGameObject->SetPosition({ 0.2f, 0.4f, 0.2f });
	cameraGameObject->SetForward({ 0.185f, 0.4f, 0.0f });
	cameraGameObject->SetUp({ 0.0f, -1.0f, 0.0f });

	playerGameObject = std::make_shared<GameObject>();

	Model model = LoadModel("Resources/Models/truck_green.obj");

	model.materials[0].shader = shader;


	std::shared_ptr<Model> modelPtr = std::make_shared<Model>(model);
	std::shared_ptr<Shader> shaderPtr = std::make_shared<Shader>(shader);
	models.push_back(modelPtr);

	GoTransform transform;
	transform.position = { 0,0,0 };
	transform.scale = { 5,5,5 };
	AddObject(transform, shaderPtr);

	transform.position = { 0,0,2.5f };
	transform.scale = { 2,3,2 };
	AddObject(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 40,15,3 };
	AddObject(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 3,15,40 };
	AddObject(transform, shaderPtr);

	transform.position = { 0,0,0 };
	transform.scale = { 40,1,40 };
	AddObject(transform, shaderPtr);


	transform.position = { 0,0,0 };
	transform.scale = { 5,5,1 };
	AddWall(transform, 0);

	SetTargetFPS(30);
}

void RayLibThirdPerson::AddObject(const GoTransform& transform, const std::shared_ptr<Shader>& shader)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	*model = LoadModelFromMesh(GenMeshCube(transform.scale.x, transform.scale.y, transform.scale.z));
	model->transform.m12 = transform.position.x;
	model->transform.m13 = transform.position.y;
	model->transform.m14 = transform.position.z;
	model->materials[0].shader = *shader.get();
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();

	gameObject->SetTransform(transform);
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
		gameObject->mesh.vertices.push_back(vertex);
	}

	for (size_t i = 0, v = 0; i < model->meshes[0].triangleCount; i++, v += 3)
	{
		Triangle triangle;
		triangle.point0 = model->meshes[0].indices[v];
		triangle.point1 = model->meshes[0].indices[v + 1];
		triangle.point2 = model->meshes[0].indices[v + 2];
		gameObject->triangles.push_back(triangle);
	}

	soundBlockingObjects.push_back(gameObject);
	models.push_back(model);
}

void RayLibThirdPerson::AddWall(const GoTransform& transform, const float& radians)
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
		gameObject->mesh.vertices.push_back(vertex);
	}

	for (size_t i = 0, v = 0; i < model->meshes[0].triangleCount; i++, v += 3)
	{
		Triangle triangle;
		triangle.point0 = model->meshes[0].indices[v];
		triangle.point1 = model->meshes[0].indices[v + 1];
		triangle.point2 = model->meshes[0].indices[v + 2];
		gameObject->triangles.push_back(triangle);
	}

	roomWalls.push_back(gameObject);
}

void RayLibThirdPerson::DeInit()
{
	CloseWindow();
}

const std::shared_ptr<GameObject> RayLibThirdPerson::GetCameraGameObject()
{
	return cameraGameObject;
}

const std::shared_ptr<GameObject> RayLibThirdPerson::GetPlayerGameObject()
{
	return playerGameObject;
}

const std::vector<std::shared_ptr<GameObject>>& RayLibThirdPerson::GetSoundBlockingObjects()
{
	return soundBlockingObjects;
}

const std::vector<std::shared_ptr<GameObject>>& RayLibThirdPerson::GetWalls()
{
	return roomWalls;
}

BoundingBox RayLibThirdPerson::CalculateBoundingBox(const Vector3& center, const float& width, const float& height, const float& length) const {
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

bool RayLibThirdPerson::IsPlayerInRoom()
{
	GameObject roomGameObject = *soundBlockingObjects[0];
	BoundingBox boxRoom = CalculateBoundingBox({ roomGameObject.GetPosition().x, roomGameObject.GetPosition().y, roomGameObject.GetPosition().z },
		roomGameObject.GetScale().x, roomGameObject.GetScale().y, roomGameObject.GetScale().z);
	BoundingBox boxPlayer = CalculateBoundingBox({ cameraGameObject->GetPosition().x, cameraGameObject->GetPosition().y, cameraGameObject->GetPosition().z },
		cameraGameObject->GetScale().x, cameraGameObject->GetScale().y, cameraGameObject->GetScale().z);
	return CheckCollisionBoxes(boxRoom, boxPlayer);
}
