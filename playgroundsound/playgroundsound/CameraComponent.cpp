#include "CameraComponent.h"

#include <src/rcamera.h>
#include <src/raylib.h>
#define RCAMERA_IMPLEMENTATION
#include "GameObject.h"
void CameraComponent::Init(GameObject* in_gameObject)
{
	camera3D = new Camera3D();
	camera3D->position = { 0.0f, 5.0f, 8.0f };
	camera3D->target = { 0.0f, .0f, 0.0f };
	camera3D->up = { 0.0f, 1.0f, 0.0f };
	camera3D->fovy = 60.0f;
	camera3D->projection = CAMERA_PERSPECTIVE;
}

void CameraComponent::Update(GameObject* in_gameObject)
{
	camera3D->target = { targetGO->m_transform.position.x, targetGO->m_transform.position.y,targetGO->m_transform.position.z };

	GO_Vector3 newCameraPos = { targetGO->m_transform.position.x - (targetGO->m_transform.forward.x + targetGO->m_transform.forward.Normalized().x * distance), 
		targetGO->m_transform.position.y - (targetGO->m_transform.forward.y + targetGO->m_transform.forward.Normalized().y * distance), 
		targetGO->m_transform.position.z - (targetGO->m_transform.forward.z + targetGO->m_transform.forward.Normalized().z * distance)};

	camera3D->position = { newCameraPos.x, newCameraPos.y + 2.5f, newCameraPos.z };
	in_gameObject->m_transform.position = newCameraPos;
	in_gameObject->m_transform.forward = newCameraPos - targetGO->m_transform.position;
	in_gameObject->m_transform.up = { camera3D->up.x, camera3D->up.y, camera3D->up.z };
}

void CameraComponent::SetTarget(const GameObject* in_targetGO)
{
	targetGO = in_targetGO;
}
