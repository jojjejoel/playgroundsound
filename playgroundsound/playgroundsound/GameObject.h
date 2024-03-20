#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <map>
#include <typeindex>

#include "GoTransform.h"
 

using GameObjectID = int;
class GameObject
{
public:
    ~GameObject()
    {
        for (auto [id, c] : m_components)
        {
            delete c;
        }
    }

    void Init();
    void Update();

    template<typename T>
    void AddComponent()
    {
        m_components[typeid(T)] = new T;
    }

    template<typename T>
    T& GetComponent()
    {
        return *(T*)m_components[typeid(T)];
    }

    std::map<std::type_index, Component*> m_components;

    GoTransform m_transform;
    GameObjectID m_id;
    std::string m_name;
};
