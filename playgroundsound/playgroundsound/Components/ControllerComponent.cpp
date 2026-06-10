#include "ControllerComponent.h"
#include <src/raylib.h>
#include <src/raymath.h>

#include <algorithm>

void ControllerComponent::Init(GameObject* in_gameObject) {
}

void ControllerComponent::SetMovementSpeed(const float& in_movementSpeed) {
    acceleration = in_movementSpeed;
}

void ControllerComponent::Update(GameObject* in_gameObject) {
    const float dt = GetFrameTime();

    // --- Input ---
    const bool handbrake = IsKeyDown(KEY_SPACE);

    isBraking = false;
    if (IsKeyDown(KEY_DOWN) && speed > 0.5f) {
        isBraking = true;
    }

    // --- Forces ---
    float driveForce = 0.f;
    if (IsKeyDown(KEY_UP) && !handbrake)
        driveForce = engineForce;
    if (isBraking)
        driveForce = -brakeForce;

    const float velMag = Vector3Length({velocity.x, velocity.y, velocity.z});
    const float dragForce = dragCoefficient * velMag * velMag;
    const float netForce = driveForce - (speed > 0 ? dragForce : -dragForce);
    const float accelerationThisFrame = netForce / mass;
    speed += accelerationThisFrame * dt;
    speed = std::clamp(speed, -maxSpeed, maxSpeed);

    // Bleed to near stop when handbraking
    if (handbrake)
        speed = speed + (0.f - speed) * 5.0f * dt;

    // --- Steering ---
    float steerInput = 0.f;
    if (IsKeyDown(KEY_LEFT)) steerInput = -1.f;
    if (IsKeyDown(KEY_RIGHT)) steerInput = 1.f;

    const float steerTarget = steerInput * maxSteerAngle;
    steerAngle += (steerTarget - steerAngle) * steerSpeed * dt;

    const float speedNorm = std::abs(speed) / maxSpeed;
    const float speedFactor = 0.5f + (1.0f - speedNorm) * 0.8f;
    const float steerAmount = steerAngle * speedFactor * dt;
    if (std::abs(speed) > 0.5f)
        Rotate(in_gameObject, steerAmount);

    // --- Velocity ---
    const GO_Vector3 forward = in_gameObject->m_transform.forward.Normalized();
    const float effectiveGrip = handbrake ? catchUpRate * 0.15f : (isBraking ? catchUpRate * 0.6f : catchUpRate);
    velocity = velocity + (forward * speed - velocity) * effectiveGrip * dt;

    in_gameObject->m_transform.position += velocity * dt;

    // --- Slip ratio ---
    if (velMag > 0.1f) {
        const GO_Vector3 velNorm = GO_Vector3{velocity.x, velocity.y, velocity.z}.Normalized();
        const float dot = forward.x * velNorm.x + forward.y * velNorm.y + forward.z * velNorm.z;
        slipRatio = 1.f - std::max(0.f, dot);
    }
    else {
        slipRatio = 0.f;
    }

    if (handbrake)
        slipRatio = std::min(slipRatio + 0.3f, 1.f);
}

void ControllerComponent::Rotate(GameObject* in_gameObject, const float& in_rotateSpeed) {
    GO_Transform& goTransform = in_gameObject->m_transform;
    Matrix currentMatrix = MatrixIdentity();
    currentMatrix.m0 = goTransform.right.x;
    currentMatrix.m4 = goTransform.right.y;
    currentMatrix.m8 = goTransform.right.z;

    currentMatrix.m1 = goTransform.up.x;
    currentMatrix.m5 = goTransform.up.y;
    currentMatrix.m9 = goTransform.up.z;

    currentMatrix.m2 = goTransform.forward.x;
    currentMatrix.m6 = goTransform.forward.y;
    currentMatrix.m10 = goTransform.forward.z;

    const Matrix rot = MatrixRotateY(in_rotateSpeed);

    currentMatrix = MatrixMultiply(currentMatrix, rot);

    goTransform.right.x = currentMatrix.m0;
    goTransform.right.y = currentMatrix.m4;
    goTransform.right.z = currentMatrix.m8;

    goTransform.up.x = currentMatrix.m1;
    goTransform.up.y = currentMatrix.m5;
    goTransform.up.z = currentMatrix.m9;

    goTransform.forward.x = currentMatrix.m2;
    goTransform.forward.y = currentMatrix.m6;
    goTransform.forward.z = currentMatrix.m10;
}
