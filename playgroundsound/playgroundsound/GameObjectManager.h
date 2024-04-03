#pragma once
#include <map>
#include "GameObject.h"

class GameObjectManager
{
public:
    ~GameObjectManager();

    GameObject* AddGameObject(std::string_view name);

    void Init();
    void Update();

    std::map<std::string_view, GameObject*> m_gameObjects;
};

