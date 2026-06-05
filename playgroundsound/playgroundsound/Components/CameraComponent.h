#pragma once
#include "Component.h"
struct Camera3D;


class CameraComponent : public Component {
public:
    void Init(GameObject* in_gameObject) override;

    void Update(GameObject* in_gameObject) override;

    void SetTarget(const GameObject* in_targetGO);

private:
    const GameObject* targetGO = nullptr;
    float distance = 5; // Distance of the camera from the target

public:
    Camera3D* camera3D;
};
