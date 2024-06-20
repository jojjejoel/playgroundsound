#pragma once
#include <map>
#include "..\GameObject\GameObject.h"

#define MAX_GAME_OBJECTS 50
class GameObjectManager
{
public:
    ~GameObjectManager();
    GameObject* AddGameObject(std::string_view name);

    void Init();
    void Update();

    int gameObjectCount = 0;
    GameObject* m_gameObjects[MAX_GAME_OBJECTS];
};

