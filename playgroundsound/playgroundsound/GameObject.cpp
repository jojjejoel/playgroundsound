#include "GameObject.h"

void GameObject::Init()
{
    for (auto& [id, component] : m_components)
    {
        component->Init();
    }
}

void GameObject::Update()
{
    for (auto& [id, component] : m_components)
    {
        component->Update();
    }
}
