#include "ControllerComponent.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include "GameObject.h"
void ControllerComponent::Init(GameObject* in_gameObject)
{
}

void ControllerComponent::SetMovementSpeed(const float& in_movementSpeed)
{
	acceleration = in_movementSpeed;
}

void ControllerComponent::Update(GameObject* in_gameObject)
{
	if (IsKeyDown(KEY_SPACE))
	{
		currentSpeed += acceleration * GetFrameTime();
		if (currentSpeed > maxSpeed)
		{
			currentSpeed = maxSpeed;
		}
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

	std::cout << currentSpeed << std::endl;	
	if (currentSpeed > 0)
	{
		currentSpeed -= deAcceleration * GetFrameTime();
		if (currentSpeed <= 0)
		{
			currentSpeed = 0;
		}
	}
	gas -= gasDeAcceleration * GetFrameTime();
	if (gas <0)
	{
		gas = 0;
	}
}

void ControllerComponent::Rotate(GameObject* in_gameObject, const float& in_rotateSpeed)
{
	GO_Transform& goTransform = in_gameObject->m_transform;
	Matrix currentMatrix;
	currentMatrix.m0 = goTransform.right.x;
	currentMatrix.m4 = goTransform.right.y;
	currentMatrix.m8 = goTransform.right.z;

	currentMatrix.m1 = goTransform.up.x;
	currentMatrix.m5 = goTransform.up.y;
	currentMatrix.m9 = goTransform.up.z;

	currentMatrix.m2 = goTransform.forward.x;
	currentMatrix.m6 = goTransform.forward.y;
	currentMatrix.m10 = goTransform.forward.z;

	float x = in_gameObject->m_transform.forward.x;
	float z = in_gameObject->m_transform.forward.z;

	Matrix rot = MatrixRotateY(in_rotateSpeed);

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

const float& ControllerComponent::GetPercentageOfMaxSpeed() const
{
	return currentSpeed / maxSpeed;
}

const float& ControllerComponent::GetGas() const
{
	return gas;
}