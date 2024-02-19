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
        
        UpdateCamera(camera.get(), cameraMode);                  // Update camera

        //cameraGameObject->position = { camera->target.x, camera->target.y, camera->target.z };
        cameraGameObject->position = { camera->position.x, camera->position.y, camera->position.z };
        playerGameObject->position = { camera->target.x, camera->target.y, camera->target.z };
        Matrix matrix = GetCameraMatrix(*camera);

    if (IsKeyPressed(KEY_U))
    {
        upX *= -1.0f;
    }
    if (IsKeyPressed(KEY_I))
    {
        upY *= -1.0f;
    }
    if (IsKeyPressed(KEY_O))
    {
        upZ *= -1.0f;
    }
    if (IsKeyPressed(KEY_J))
    {
        forwardX *= -1.0f;
    }
    if (IsKeyPressed(KEY_K))
    {
        forwardY *= -1.0f;
    }
    if (IsKeyPressed(KEY_L))
    {
        forwardZ *= -1.0f;
    }

        cameraGameObject->up = { matrix.m1 * upX, matrix.m5 * upY, matrix.m9 * upZ };
        cameraGameObject->forward = { matrix.m2 * forwardX, matrix.m6 * forwardY, matrix.m10 * forwardZ };

    
    /*else
    {
        cameraGameObject->up = { matrix.m1 * upX, matrix.m10 * upY, matrix.m6 * upZ };
        cameraGameObject->forward = { matrix.m2 * forwardX, matrix.m9 * forwardY, matrix.m15 * forwardZ };
    }*/

        playerGameObject->up = {0, 1, 0 };
        playerGameObject->forward = { 0, 0, 1 };

       
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube({ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube({ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube({ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall


        RayCollision collision = CheckCollisions();
        if (collision.hit)
        {
            DrawLine3D(camera->target, collision.point, RED);
        }
        else
        {
            DrawLine3D(camera->target, {0,1,0}, GREEN);
        }


        for (const auto& model : models)
        {
            DrawModel(*model, { 0,model->transform.m7,0 }, 1, { 255,255,255,255 });
        }

        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON)
        {
            DrawCube(camera->target, 0.5f, 0.5f, 0.5f, PURPLE);
            //DrawCubeWires(camera->target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
            //DrawLine3D(camera->target, Vector3{ camera->target.x,camera->target.y + 5,camera->target.z }, { 255,0,0,255 });
        }

        EndMode3D();

        // Draw info boxes
        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);

        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);

        std::string upVector = "Up: { X " + std::to_string(upX) + ", Y " + std::to_string(upY) + ", Z " + std::to_string(upZ) + "}";
        std::string forwardVector = "Forward: { X " + std::to_string(forwardX) + ", Y " + std::to_string(forwardY) + ", Z " + std::to_string(forwardZ) + "}";
        DrawText(upVector.c_str(), 15, 75, 10, BLACK);
        DrawText(forwardVector.c_str(), 15, 90, 10, BLACK);

        DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(600, 5, 195, 100, BLUE);

      

        EndDrawing();
        //----------------------------------------------------------------------------------
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
    cameraGameObject->position = { 0.2f, 0.4f, 0.2f };
    cameraGameObject->forward = { 0.185f, 0.4f, 0.0f };
    cameraGameObject->up = { 0.0f, -1.0f, 0.0f };

    playerGameObject = std::make_shared<GameObject>();

    cameraMode = CAMERA_THIRD_PERSON;
    
    std::shared_ptr<Model> model = std::make_shared<Model>();
    *model = LoadModelFromMesh(GenMeshPlane(2, 8, 10,10));
    model->transform.m12 = 3;
    model->transform.m13 = 0;
    model->transform.m14 = 3;

    model->transform.m0 = 0;

    model->transform.m4 = -1;
    model->transform.m1 = 1;
    model->transform.m5 = 0;
    //model->meshes[0].triangleCount
    wallGameObject = std::make_shared<GameObject>();

    
    for (size_t i = 0; i < model->meshes[0].vertexCount; i++)
    {
        Playground::Vertex vertex;
        float* vertices = model->meshes[0].vertices;
        vertex.x = vertices[i];
        vertex.y = vertices[i + 1];
        vertex.z = vertices[i + 2];
        wallGameObject->mesh.vertices.push_back(vertex);
    }

    ///*model*/->transform.
    models.push_back(model);


    SetTargetFPS(60);

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

const std::shared_ptr<GameObject> RayLibThirdPerson::GetWallGameObject()
{
    return wallGameObject;
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