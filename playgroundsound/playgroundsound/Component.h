#pragma once
class GameObject;

class Component
{
public:
    virtual ~Component() {}

    virtual void Init() = 0;
    virtual void Update(GameObject* in_gameObject) = 0;
};

