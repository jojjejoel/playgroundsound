#include "GameObjectManager.h"

GameObjectManager::~GameObjectManager()
{
    for (size_t i = 0; i < gameObjectCount; i++)
    {
        delete m_gameObjects[i];
    }
}

GameObject* GameObjectManager::AddGameObject(std::string_view name)
{
    if (gameObjectCount < MAX_GAME_OBJECTS)
    {
        GameObject* go = new GameObject();
        GameObjectID iD = gameObjectCount;
        go->m_id = iD;
        go->m_name = name;
        m_gameObjects[iD] = go;

        gameObjectCount++;

        return go;
    }
    else
    {
        // Log error. Maximum limit of GameObjects reached.
        return m_gameObjects[0];
    }
    
}

void GameObjectManager::Init()
{
    for (size_t i = 0; i < gameObjectCount; i++)
    {
        m_gameObjects[i]->Init();
    }
}

void GameObjectManager::Update()
{
    for (size_t i = 0; i < gameObjectCount; i++)
    {
        m_gameObjects[i]->Update();
    }
}
