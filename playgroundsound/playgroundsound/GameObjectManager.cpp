#include "GameObjectManager.h"

GameObjectManager::~GameObjectManager()
{
    for (auto [id, go] : m_gameObjects)
    {
        delete go;
    }
}

GameObject* GameObjectManager::AddGameObject(std::string_view name)
{
    GameObject* go = new GameObject;
    go->m_id = m_gameObjects.size();
    go->m_name = name;
    m_gameObjects.insert(std::make_pair(name, go));
    return go;
}

void GameObjectManager::Init()
{
    for (auto [id, go] : m_gameObjects)
    {
        go->Init();
    }
}

void GameObjectManager::Update()
{
    for (auto [id, go] : m_gameObjects)
    {
        go->Update();
    }
}
