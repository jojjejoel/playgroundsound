/*******************************************************************************************
*
*   raylib [models] example - first person maze
*
*   Example originally created with raylib 2.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2019-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <memory>
#include "GameObject.h"
class Camera3D;
class Vector3;
class Texture;
class Model;
class Color;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
class RaylibMaze {
public:
	int Run();
	void Init();
	void DeInit();
	const std::shared_ptr<GameObject> GetCameraGameObject();
private:
	Color* mapPixels;
	std::shared_ptr<Camera3D> camera;
	std::shared_ptr<GameObject> cameraGameObject;
	std::shared_ptr<Vector3> mapPosition;
	std::shared_ptr<Texture> cubicmap;
	std::shared_ptr<Model> model;
	std::shared_ptr<Texture> texture;
};