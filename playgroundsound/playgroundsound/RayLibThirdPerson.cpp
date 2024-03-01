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
        cameraGameObject->SetPosition({camera->position.x, camera->position.y, -camera->position.z});
        playerGameObject->SetPosition({camera->target.x, camera->target.y, -camera->target.z});
        Matrix matrix = GetCameraMatrix(*camera);


        cameraGameObject->SetUp({matrix.m1 * upX, matrix.m5 * upY, matrix.m9 * upZ});
        cameraGameObject->SetForward({matrix.m2 * forwardX, matrix.m6 * forwardY, matrix.m10 * forwardZ});



        playerGameObject->SetScale({ 0.5f, 0.5f, 0.5f });


        playerGameObject->SetUp({0, 1, 0});
        playerGameObject->SetForward({0, 0, 1});
        


        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube({ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube({ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube({ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall


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
                Vector3 emitterPos = { diffractionPath.nodes[nodeIndex -1].x, diffractionPath.nodes[nodeIndex -1].y, diffractionPath.nodes[nodeIndex -1].z };


                DrawSphereWires(nodePos, 0.2f, 10, 10, color);

                DrawLine3D(emitterPos, nodePos, color);
            }
            Vector3 emitterPos = { diffractionPath.emitterPos.x, diffractionPath.emitterPos.y, diffractionPath.emitterPos.z };
            Vector3 nodePos = { diffractionPath.nodes[numNodes -1].x, diffractionPath.nodes[numNodes - 1].y, diffractionPath.nodes[numNodes - 1].z };
            DrawLine3D(emitterPos, nodePos, color);
            }
            else
            {
                Vector3 startLinePos = { diffractionPath.emitterPos.x, diffractionPath.emitterPos.y, diffractionPath.emitterPos.z };
                Vector3 endLinePos = { camera->target };
                DrawLine3D(startLinePos, endLinePos, color);
            }

            
        }

        DrawSphereWires({0,0,0}, 0.5f, 10, 10, GREEN);

        DrawModel(*models[0], camera->target, 1, WHITE);
        for (size_t i = 1; i < models.size(); i++)
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
            Vector3 pos = { models[i]->transform.m12, models[i]->transform.m13,models[i]->transform.m13 };
            DrawModel(*models[i], pos, 1, WHITE);
        }

        EndMode3D();

      

        EndDrawing();
}

RayCollision RayLibThirdPerson::CheckCollisions()
{
    std::vector<RayCollision> collisions;
    for (const auto& model : models)
    {
        Ray ray;
        ray.position = camera->target;
        Vector3 dir;
        dir.x = 0 - camera->target.x;
        dir.y = 0 - camera->target.y;
        dir.z = 0 - camera->target.z;
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
    

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PlaygroundSound");

    // Define the camera to look into our 3d world (position, target, up vector)
    camera = std::make_shared<Camera3D>();
    camera->position = { 0.0f, 2.0f, 4.0f };    // Camera position
    camera->target = { 0.0f, .0f, 0.0f };      // Camera looking at point
    camera->up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera->fovy = 60.0f;                                // Camera field-of-view Y
    camera->projection = CAMERA_PERSPECTIVE;             // Camera projection type

    cameraGameObject = std::make_shared<GameObject>();
    cameraGameObject->SetPosition({0.2f, 0.4f, 0.2f});
    cameraGameObject->SetForward({0.185f, 0.4f, 0.0f});
    cameraGameObject->SetUp({0.0f, -1.0f, 0.0f});

    playerGameObject = std::make_shared<GameObject>();

    Model model = LoadModel("Resources/Models/truck_green.obj");
    
   /*
    model.meshMaterial = model.materials;
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture;*/

   

    std::shared_ptr<Model> modelPtr = std::make_shared<Model>(model);
    models.push_back(modelPtr);
    
    GoTransform transform;
    transform.position = { 0,0,0 };
    transform.scale = { 10,10,10 };
    AddObject(transform);

    transform.position = { 0,0,5 };
    transform.scale = { 3,3,3 };
    transform.forward = { 0,0,1 };
    transform.up = { 0,1,0 };
    AddObject(transform);

   
    transform.position = { 0,0,0 };
    transform.scale = { 10,10,1 };
    transform.forward = { 0,0,1 };
    transform.up = { 0,1,0 };
    AddWall(transform, 0);

   /* transform.position = { 0,0,-5 };
    transform.scale = { 10,10,1 };
    transform.forward = { 0,0,1 };
    transform.up = { 0,1,0 };
    AddWall(transform, -1.5708f);

    transform.position = { 5,0,0 };
    transform.scale = { 10,10,1 };
    transform.forward = { 0,0,1 };
    transform.up = { 0,1,0 };
    AddWall(transform,0);

    transform.position = { -5,0,0 };
    transform.scale = { 10,10,1 };
    transform.forward = { 0,0,1 };
    transform.up = { 0,1,0 };
    AddWall(transform, 3.14159f);*/


    SetTargetFPS(60);
}

void RayLibThirdPerson::AddObject(const GoTransform& transform)
{
    std::shared_ptr<Model> model = std::make_shared<Model>();
    *model = LoadModelFromMesh(GenMeshCube(transform.scale.x, transform.scale.y, transform.scale.z));
    model->transform.m12 = transform.position.x;
    model->transform.m13 = transform.position.y;
    model->transform.m14 = transform.position.z;
    
   /* model->transform.m1 = transform.up.x;
        model->transform.m5 = transform.up.y;
        model->transform.m9 = transform.up.z;

        model->transform.m2 = transform.forward.x;
        model->transform.m6 = transform.forward.y;
        model->transform.m9 = transform.forward.z;*/

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

void RayLibThirdPerson::AddWall(const GoTransform& transform, float radians)
{
    std::shared_ptr<Model> model = std::make_shared<Model>();
    *model = LoadModelFromMesh(GenMeshPlane(transform.scale.x, transform.scale.y, 10,10));
    model->transform.m12 = transform.position.x;
    model->transform.m13 = transform.position.y;
    model->transform.m14 = transform.position.z;
    model->transform = MatrixMultiply(MatrixRotateZ(1.5708f), model->transform);
    model->transform = MatrixMultiply(MatrixRotateX(radians), model->transform);
    //model->transform

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
    //models.push_back(model);
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

    // Calculate minimum vertex
    boundingBox.min.x = center.x - halfWidth;
    boundingBox.min.y = center.y - halfHeight;
    boundingBox.min.z = center.z - halfLength;

    // Calculate maximum vertex
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
