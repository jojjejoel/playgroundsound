#pragma once

#include "..\Components\Component.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <typeindex>

#include "GO_Transform.h"


using GameObjectID = int;
class GameObject
{
public:
    void Init();
    void Update();

    template<typename T>
    T& AddComponent()
    {
        auto t = std::make_unique<T>();
        T* raw = t.get();
        m_components[typeid(T)] = std::move(t);
        return *raw;
    }

    template<typename T>
    T* GetComponent()
    {
        const auto it = m_components.find(typeid(T));
        if (it == m_components.end()) return nullptr;
        return static_cast<T*>(it->second.get());
    }

    std::map<std::type_index, std::unique_ptr<Component>> m_components;

    GO_Transform m_transform;
    GameObjectID m_id;
    std::string m_name;
};
