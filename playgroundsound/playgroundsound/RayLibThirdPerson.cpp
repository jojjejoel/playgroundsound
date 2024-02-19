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

#define MAX_COLUMNS 10


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

    if (IsKeyPressed(KEY_H))
    {
        leftHand = !leftHand;
    }

    if (leftHand)
    {
        cameraGameObject->up = { matrix.m1 * upX, matrix.m5 * upY, matrix.m9 * upZ };
        cameraGameObject->forward = { matrix.m2 * forwardX, matrix.m6 * forwardY, matrix.m10 * forwardZ };

    }
    else
    {
        cameraGameObject->up = { matrix.m1 * upX, matrix.m10 * upY, matrix.m6 * upZ };
        cameraGameObject->forward = { matrix.m2 * forwardX, matrix.m9 * forwardY, matrix.m15 * forwardZ };
    }

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

        DrawCube({ 0.0f, 0.0f, 0.0f }, 2, 2, 2, GOLD);      // Draw a yellow wall

        RayCollision collision = CheckCollisions();
        if (collision.hit)
        {
            DrawLine3D(camera->target, collision.point, RED);
        }
        else
        {
            DrawLine3D(camera->target, {0,1,0}, GREEN);
        }


        // Draw some cubes around
        for (int i = 0; i < MAX_COLUMNS; i++)
        {
            DrawCube(*positions[i], 2.0f, heights[i], 2.0f, *colors[i]);
            //DrawCubeWires(*positions[i], 2.0f, heights[i], 2.0f, MAROON);
            DrawBoundingBox(*boundingBoxes[i], BLACK);
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
        DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);

        std::string upVector = "Up: { X " + std::to_string(upX) + ", Y " + std::to_string(upY) + ", Z " + std::to_string(upZ) + "}";
        std::string forwardVector = "Forward: { X " + std::to_string(forwardX) + ", Y " + std::to_string(forwardY) + ", Z " + std::to_string(forwardZ) + "}";
        DrawText(upVector.c_str(), 15, 75, 10, BLACK);
        DrawText(forwardVector.c_str(), 15, 90, 10, BLACK);

        DrawRectangle(600, 5, 195, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(600, 5, 195, 100, BLUE);

        DrawText("Camera status:", 610, 15, 10, BLACK);
        DrawText(TextFormat("- Mode: %s", (cameraMode == CAMERA_FREE) ? "FREE" :
            (cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
            (cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
            (cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 30, 10, BLACK);
        DrawText(TextFormat("- Projection: %s", (camera->projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
            (camera->projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 45, 10, BLACK);
        DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera->position.x, camera->position.y, camera->position.z), 610, 60, 10, BLACK);
        DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera->target.x, camera->target.y, camera->target.z), 610, 75, 10, BLACK);
        DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera->up.x, camera->up.y, camera->up.z), 610, 90, 10, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
}

RayCollision RayLibThirdPerson::CheckCollisions()
{
    std::vector<RayCollision> collisions;
    for (const auto& box : boundingBoxes)
    {
        Ray ray;
        ray.position = camera->target;
        Vector3 dir;
        dir.x = 0 - camera->target.x;
        dir.y = 0 - camera->target.y;
        dir.z = 0 - camera->target.z;
        ray.direction = dir;
        RayCollision collision = GetRayCollisionBox(ray, *box);
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

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    camera = std::make_shared<Camera3D>();
    camera->position = { 0.0f, 2.0f, 4.0f };    // Camera position
    camera->target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera->up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera->fovy = 60.0f;                                // Camera field-of-view Y
    camera->projection = CAMERA_PERSPECTIVE;             // Camera projection type

    cameraGameObject = std::make_shared<GameObject>();
    cameraGameObject->position = { 0.2f, 0.4f, 0.2f };
    cameraGameObject->forward = { 0.185f, 0.4f, 0.0f };
    cameraGameObject->up = { 0.0f, -1.0f, 0.0f };

    playerGameObject = std::make_shared<GameObject>();

    cameraMode = CAMERA_THIRD_PERSON;

    for (size_t i = 0; i < MAX_COLUMNS; i++)
    {
        heights.push_back({ 0 });
    }
    for (size_t i = 0; i < MAX_COLUMNS; i++)
    {
        std::shared_ptr<Vector3> position = std::make_shared<Vector3>();
        positions.push_back(position);

    }
    for (size_t i = 0; i < MAX_COLUMNS; i++)
    {
        std::shared_ptr<Color> color = std::make_shared<Color>();
        colors.push_back(color);
    }
    for (int i = 0; i < MAX_COLUMNS; i++)
    {
        heights[i] = 8;
        std::shared_ptr<Vector3> position = std::make_shared<Vector3>();
        position->x = (float)GetRandomValue(-15, 15);
        position->y = heights[i] / 2.0f;
        position->z = (float)GetRandomValue(-15, 15);
        positions[i] = position;
        std::shared_ptr<Color> color = std::make_shared<Color>();
        color->r = static_cast<unsigned char>(GetRandomValue(20, 255));
        color->g = static_cast<unsigned char>(GetRandomValue(10, 55));
        color->b = 30;
        color->a = 255;
        colors[i] = color;


        std::shared_ptr<BoundingBox> box = std::make_shared<BoundingBox>();
        box->min.x = CalculateBoundingBox(*position, 2, 8, 1).min.x;
        box->min.y = CalculateBoundingBox(*position, 2, 8, 1).min.y;
        box->min.z = CalculateBoundingBox(*position, 2, 8, 1).min.z;
        box->max.x = CalculateBoundingBox(*position, 2, 8, 1).max.x;
        box->max.y = CalculateBoundingBox(*position, 2, 8, 1).max.y;
        box->max.z = CalculateBoundingBox(*position, 2, 8, 1).max.z;
        boundingBoxes.push_back(box);
    }

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