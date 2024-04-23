#pragma once
#include "Component.h"

class ControllerComponent : public Component
{
public:
    virtual void Init(GameObject* in_gameObject) override;
    void SetMovementSpeed(const float& movementSpeed);

    virtual void Update(GameObject* in_gameObject) override;

    void Rotate(GameObject* in_gameObject, const float& in_rotateSpeed);

    const float& GetPercentageOfMaxSpeed() const;

    const float& GetGas() const;

private:
    float angle = 0;
    float rotateSpeed = 1.0f;

    float acceleration = 0.5f;
    float maxSpeed = 10;
    float currentSpeed = 0;

    float deAcceleration = 2.0f;

    float gas = 0;
    float gasAcceleration = .6f;
    float gasDeAcceleration = .4f;
};

