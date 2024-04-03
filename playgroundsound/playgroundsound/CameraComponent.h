#pragma once
#include "Component.h"
#include "GameObject.h"
#include "GoVector3.h"
struct Camera3D;


class CameraComponent : public Component
{
public:
    virtual void Init() override;

    virtual void Update(GameObject* in_parentGO) override;

    void SetTarget(const GameObject* in_targetGO);
private:
    const GameObject* targetGO;
    float distance = 3; // Distance of the camera from the target
    float angleX;   // Angle of rotation around the target on X-axis
    float angleY;   // Angle of rotation around the target on Y-axis
    float sensitivity = 1; // Mouse sensitivity for camera movement

public:
    Camera3D* camera3D;
};
