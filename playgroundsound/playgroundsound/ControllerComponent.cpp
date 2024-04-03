#include "ControllerComponent.h"
#include "raylib.h"
#include "raymath.h"
void ControllerComponent::Init()
{
}

void ControllerComponent::SetMovementSpeed(const float& in_movementSpeed)
{
	movementSpeed = in_movementSpeed;
}

void ControllerComponent::Update(GameObject* in_parentGO)
{
	if (IsKeyDown(KEY_SPACE))
	{
		GoVector3 posDiff = in_parentGO->m_transform.forward * movementSpeed * GetFrameTime();
		in_parentGO->m_transform.position += posDiff;
	}
	if (IsKeyDown(KEY_LEFT))
	{
		Rotate(in_parentGO, -rotateSpeed * GetFrameTime());
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		Rotate(in_parentGO, rotateSpeed * GetFrameTime());
	}

}

void ControllerComponent::Rotate(GameObject* in_parentGO, const float& in_rotateSpeed)
{
	GoTransform& goTransform = in_parentGO->m_transform;
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

	float x = in_parentGO->m_transform.forward.x;
	float z = in_parentGO->m_transform.forward.z;

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
