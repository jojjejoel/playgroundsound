///*******************************************************************************************
//*
//*   raylib [models] example - first person maze
//*
//*   Example originally created with raylib 2.5, last time updated with raylib 3.5
//*
//*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
//*   BSD-like license that allows static linking with closed source software
//*
//*   Copyright (c) 2019-2024 Ramon Santamaria (@raysan5)
//*
//********************************************************************************************/
//
//#include "RayLibMaze.h"
//#include "raylib.h"
//
//#include <stdlib.h>           // Required for: free()
////------------------------------------------------------------------------------------
//// Program main entry point
////------------------------------------------------------------------------------------
//
//int RaylibMaze::Run()
//{
//	if (IsKeyPressed(KEY_F))
//	{
//		DisableCursor();
//	}
//	if (IsKeyPressed(KEY_G))
//	{
//		EnableCursor();
//	}
//	// Main game loop
//	
//		// Update
//		//----------------------------------------------------------------------------------
//		Vector3 oldCamPos = camera->position;    // Store old camera position
//
//		UpdateCamera(camera.get(), CAMERA_FIRST_PERSON);
//		cameraGameObject->position = { camera->position.x, camera->position.y, camera->position.z };
//		Matrix matrix =	GetCameraMatrix(*camera);
//		cameraGameObject->up = { matrix.m1, -matrix.m5, matrix.m9 };
//		cameraGameObject->forward = {-matrix.m2, matrix.m6, -matrix.m10 };
//		
//		// Check player collision (we simplify to 2D collision detection)
//		Vector2 playerPos = { camera->position.x, camera->position.z };
//		float playerRadius = 0.1f;  // Collision radius (player is modelled as a cilinder for collision)
//
//		int playerCellX = (int)(playerPos.x - mapPosition->x + 0.5f);
//		int playerCellY = (int)(playerPos.y - mapPosition->z + 0.5f);
//
//		// Out-of-limits security check
//		if (playerCellX < 0) playerCellX = 0;
//		else if (playerCellX >= cubicmap->width) playerCellX = cubicmap->width - 1;
//
//		if (playerCellY < 0) playerCellY = 0;
//		else if (playerCellY >= cubicmap->height) playerCellY = cubicmap->height - 1;
//
//		// Check map collisions using image data and player position
//		// TODO: Improvement: Just check player surrounding cells for collision
//		for (int y = 0; y < cubicmap->height; y++)
//		{
//			for (int x = 0; x < cubicmap->width; x++)
//			{
//				if ((mapPixels[y * cubicmap->width + x].r == 255) &&       // Collision: white pixel, only check R channel
//					(CheckCollisionCircleRec(playerPos, playerRadius,
//						 {
//					mapPosition->x - 0.5f + x * 1.0f, mapPosition->z - 0.5f + y * 1.0f, 1.0f, 1.0f
//				})))
//				{
//					// Collision detected, reset camera position
//					camera->position = oldCamPos;
//					cameraGameObject->position.x = oldCamPos.x;
//					cameraGameObject->position.y = oldCamPos.y;
//					cameraGameObject->position.z = oldCamPos.z;
//				}
//			}
//		}
//		//----------------------------------------------------------------------------------
//
//		// Draw
//		//----------------------------------------------------------------------------------
//		BeginDrawing();
//
//		ClearBackground(RAYWHITE);
//
//		BeginMode3D(*camera.get());
//		DrawModel(*model.get(), *mapPosition.get(), 1.0f, WHITE);                     // Draw maze map
//		EndMode3D();
//
//		DrawTextureEx(*cubicmap, { GetScreenWidth() - cubicmap->width * 4.0f - 20, 20.0f }, 0.0f, 4.0f, WHITE);
//		DrawRectangleLines(GetScreenWidth() - cubicmap->width * 4 - 20, 20, cubicmap->width * 4, cubicmap->height * 4, GREEN);
//
//		// Draw player position radar
//		DrawRectangle(GetScreenWidth() - cubicmap->width * 4 - 20 + playerCellX * 4, 20 + playerCellY * 4, 4, 4, RED);
//
//		DrawFPS(10, 10);
//
//		EndDrawing();
//		//----------------------------------------------------------------------------------
//	
//
//	return 0;
//}
//
//void RaylibMaze::Init()
//{
//	// Initialization
//	//--------------------------------------------------------------------------------------
//	const int screenWidth = 800;
//	const int screenHeight = 450;
//
//	InitWindow(screenWidth, screenHeight, "raylib [models] example - first person maze");
//
//	// Define the camera to look into our 3d world
//	camera = std::make_shared<Camera>();
//	camera->position = { 0.2f, 0.4f, 0.2f };    // Camera position
//	camera->target = { 0.185f, 0.4f, 0.0f };    // Camera looking at point
//	camera->up = { 0.0f, -1.0f, 0.0f };          // Camera up vector (rotation towards target)
//	camera->fovy = 45.0f;                                // Camera field-of-view Y
//	camera->projection = CAMERA_PERSPECTIVE;             // Camera projection type
//
//	cameraGameObject = std::make_shared<GameObject>();
//	cameraGameObject->position = { 0.2f, 0.4f, 0.2f };
//	cameraGameObject->forward = { 0.185f, 0.4f, 0.0f };
//	cameraGameObject->up = { 0.0f, -1.0f, 0.0f };
//
//	Vector3 position = { 0.0f, 0.0f, 0.0f };            // Set model position
//
//	Image imMap = LoadImage("resources/cubicmap.png");      // Load cubicmap image (RAM)
//	cubicmap = std::make_shared<Texture2D>();
//	*cubicmap = LoadTextureFromImage(imMap);       // Convert image to texture to display (VRAM)
//	Mesh mesh = GenMeshCubicmap(imMap, { 1.0f, 1.0f, 1.0f });
//	model = std::make_shared<Model>();
//	*model = LoadModelFromMesh(mesh);
//
//	// NOTE: By default each cube is mapped to one part of texture atlas
//	texture = std::make_shared<Texture2D>();
//	*texture = LoadTexture("resources/cubicmap_atlas.png");    // Load map texture
//	model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;    // Set map diffuse texture
//
//	// Get map image data to be used for collision detection
//	mapPixels = LoadImageColors(imMap);
//	UnloadImage(imMap);             // Unload image from RAM
//
//	mapPosition = std::make_shared<Vector3>();
//	*mapPosition = {-16.0f, 0.0f, -8.0f};  // Set model position
//
//
//	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
//	//--------------------------------------------------------------------------------------
//}
//
//void RaylibMaze::DeInit()
//{
//	// De-Initialization
//	//--------------------------------------------------------------------------------------
//	UnloadImageColors(mapPixels);   // Unload color array
//
//	UnloadTexture(*cubicmap);        // Unload cubicmap texture
//	UnloadTexture(*texture);         // Unload map texture
//	UnloadModel(*model);             // Unload map model
//
//	CloseWindow();                  // Close window and OpenGL context
//	//--------------------------------------------------------------------------------------
//}
//
//const std::shared_ptr<GameObject> RaylibMaze::GetCameraGameObject()
//{
//	return cameraGameObject;
//}
//
