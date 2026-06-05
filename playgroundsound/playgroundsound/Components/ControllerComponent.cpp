#include "ControllerComponent.h"
#include <src/raylib.h>
#include <src/raymath.h>

#include <algorithm>
#include "..\GameObject\GameObject.h"
void ControllerComponent::Init(GameObject* in_gameObject)
{
}

void ControllerComponent::SetMovementSpeed(const float& in_movementSpeed)
{
	acceleration = in_movementSpeed;
}

void ControllerComponent::Update(GameObject* in_gameObject)
{
	if (IsKeyDown(KEY_UP))
	{
		currentSpeed += acceleration * GetFrameTime();
		currentSpeed = std::min(currentSpeed, maxSpeed);
		gas += gasAcceleration * GetFrameTime();
		if (gas > 1)
		{
			gas = 0;
		}
	}
	if (IsKeyDown(KEY_DOWN))
	{
		currentSpeed -= acceleration * GetFrameTime();
		currentSpeed = std::max(currentSpeed, -maxSpeed);
		gas += gasAcceleration * GetFrameTime();
		if (gas > 1)
		{
			gas = 0;
		}
	}
	if (IsKeyDown(KEY_LEFT))
	{
		Rotate(in_gameObject, -rotateSpeed * GetFrameTime());
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		Rotate(in_gameObject, rotateSpeed * GetFrameTime());
	}
	GO_Vector3 posDiff = in_gameObject->m_transform.forward.Normalized() * currentSpeed * GetFrameTime();
	in_gameObject->m_transform.position += posDiff;

	if (currentSpeed > 0)
	{
		currentSpeed -= deAcceleration * GetFrameTime();
		currentSpeed = std::max<float>(currentSpeed, 0);
	}
	else if (currentSpeed < 0)
	{
		currentSpeed += deAcceleration * GetFrameTime();
		currentSpeed = std::min<float>(currentSpeed, 0);
	}
	gas -= gasDeAcceleration * GetFrameTime();
	gas = std::max<float>(gas, 0);
}

void ControllerComponent::Rotate(GameObject* in_gameObject, const float& in_rotateSpeed)
{
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

float ControllerComponent::GetPercentageOfMaxSpeed() const {
	return currentSpeed / maxSpeed;
}

const float& ControllerComponent::GetGas() const
{
	return gas;
}