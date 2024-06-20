#pragma once

#include "..\Components\Component.h"

#include <string>
#include <vector>
#include <map>
#include <typeindex>

#include "GO_Transform.h"


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
    T& AddComponent()
    {
        T* t = new T;
        m_components[typeid(T)] = t;
        return *t;
    }

    template<typename T>
    T& GetComponent()
    {
        return *(T*)m_components[typeid(T)];
    }

    std::map<std::type_index, Component*> m_components;

    GO_Transform m_transform;
    GameObjectID m_id;
    std::string m_name;
};