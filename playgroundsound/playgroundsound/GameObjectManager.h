#pragma once
#include "GameObject.h"
class GameObjectManager
{
public:
    ~GameObjectManager();

    GameObject* AddGameObject(std::string_view name);

    void Init();
    void Update();

    std::vector<GameObject*> m_gameObjects;
};

