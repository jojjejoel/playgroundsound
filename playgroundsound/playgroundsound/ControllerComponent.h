#pragma once
#include "Component.h"
#include "GameObject.h"

class ControllerComponent : public Component
{
public:
    virtual void Init() override;
    void SetMovementSpeed(const float& movementSpeed);

    virtual void Update(GameObject* parent) override;

    void Rotate(GameObject* in_parentGO, const float& in_rotateSpeed);

private:
    float movementSpeed = 1;
    float angle = 0;
    float rotateSpeed = 1.0f;
};

