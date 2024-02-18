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

#define MAX_COLUMNS 20


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
    // Main game loop
    // 
        // Update
        //----------------------------------------------------------------------------------
        // Switch camera mode
        if (IsKeyPressed(KEY_ONE))
        {
            cameraMode = CAMERA_FREE;
            camera->up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_TWO))
        {
            cameraMode = CAMERA_FIRST_PERSON;
            camera->up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_THREE))
        {
            cameraMode = CAMERA_THIRD_PERSON;
            camera->up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        if (IsKeyPressed(KEY_FOUR))
        {
            cameraMode = CAMERA_ORBITAL;
            camera->up = { 0.0f, 1.0f, 0.0f }; // Reset roll
        }

        // Switch camera projection
        if (IsKeyPressed(KEY_P))
        {
            if (camera->projection == CAMERA_PERSPECTIVE)
            {
                // Create isometric view
                cameraMode = CAMERA_THIRD_PERSON;
                // Note: The target distance is related to the render distance in the orthographic projection
                camera->position = { 0.0f, 2.0f, -100.0f };
                camera->target = { 0.0f, 2.0f, 0.0f };
                camera->up = { 0.0f, 1.0f, 0.0f };
                camera->projection = CAMERA_ORTHOGRAPHIC;
                camera->fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
                CameraYaw(camera.get(), -135 * DEG2RAD, true);
                CameraPitch(camera.get(), -45 * DEG2RAD, true, true, false);
            }
            else if (camera->projection == CAMERA_ORTHOGRAPHIC)
            {
                // Reset to default view
                cameraMode = CAMERA_THIRD_PERSON;
                camera->position = { 0.0f, 2.0f, 10.0f };
                camera->target = { 0.0f, 2.0f, 0.0f };
                camera->up = { 0.0f, 1.0f, 0.0f };
                camera->projection = CAMERA_PERSPECTIVE;
                camera->fovy = 60.0f;
            }
        }

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's reecommended to compute camera movement manually
        UpdateCamera(camera.get(), cameraMode);                  // Update camera

        cameraGameObject->position = { camera->target.x, camera->target.y, camera->target.z };
        Matrix matrix = GetCameraMatrix(*camera);
        cameraGameObject->up = { matrix.m1, -matrix.m5, matrix.m9 };
        cameraGameObject->forward = { -matrix.m2, matrix.m6, -matrix.m10 };
       
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(*camera);

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube({ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube({ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube({ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

        // Draw some cubes around
        for (int i = 0; i < MAX_COLUMNS; i++)
        {
            DrawCube(*positions[i], 2.0f, heights[i], 2.0f, *colors[i]);
            DrawCubeWires(*positions[i], 2.0f, heights[i], 2.0f, MAROON);
        }

        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON)
        {
            DrawCube(camera->target, 0.5f, 0.5f, 0.5f, PURPLE);
            DrawCubeWires(camera->target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
        }

        EndMode3D();

        // Draw info boxes
        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);

        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        DrawText("- Camera mode keys: 1, 2, 3, 4", 15, 60, 10, BLACK);
        DrawText("- Zoom keys: num-plus, num-minus or mouse scroll", 15, 75, 10, BLACK);
        DrawText("- Camera projection key: P", 15, 90, 10, BLACK);

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


    cameraMode = CAMERA_FIRST_PERSON;

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
        heights[i] = (float)GetRandomValue(1, 12);
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
    }

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

}

void RayLibThirdPerson::DeInit()
{
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

const std::shared_ptr<GameObject> RayLibThirdPerson::GetCameraGameObject()
{
    return cameraGameObject;
}
