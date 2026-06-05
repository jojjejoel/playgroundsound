#include "GameObject.h"

#include <ranges>

void GameObject::Init()
{
    for (const auto& component : m_components | std::views::values)
    {
        component->Init(this);
    }
}

void GameObject::Update()
{
    for (const auto& component : m_components | std::views::values)
    {
        component->Update(this);
    }
}
