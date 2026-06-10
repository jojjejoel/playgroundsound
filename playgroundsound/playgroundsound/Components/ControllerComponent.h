#pragma once
#include "Component.h"
#include "..\GameObject\GameObject.h"

class ControllerComponent : public Component {
public:
    virtual void Init(GameObject* in_gameObject) override;
    void SetMovementSpeed(const float& movementSpeed);

    virtual void Update(GameObject* in_gameObject) override;

    static void Rotate(GameObject* in_gameObject, const float& in_rotateSpeed);

private:
    float steerAngle = 0.f;
    float maxSteerAngle = 0.8f; // radians, how far wheels can turn
    float steerSpeed = 5.0f; // how fast wheels turn

    float acceleration = 10.0f;
    float maxSpeed = 15;
    GO_Vector3 velocity = {0, 0, 0}; // actual movement direction/speed
    float speed = 0.f; // scalar for engine input
    float slipRatio = 0.f; // 0 = full grip, 1 = full drift - drives audio
    float catchUpRate = 10.0f; // how quickly velocity corrects to match input speed. higher = less slip
    bool isBraking = false;

    float mass = 1200.0f; // kg, heavy truck feel
    float engineForce = 8000.0f; // newtons
    float brakeForce = 12000.0f;
    float dragCoefficient = 0.4f; // air/rolling resistance
};
