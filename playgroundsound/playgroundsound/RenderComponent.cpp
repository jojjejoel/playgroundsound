#include "RenderComponent.h"
#include "raylib.h"

void RenderComponent::Init(GameObject* in_gameObject)
{
}

void RenderComponent::Update(GameObject* in_gameObject)
{
	model->transform.m0 = -in_gameObject->m_transform.right.x;
	model->transform.m4 = -in_gameObject->m_transform.right.y;
	model->transform.m8 = in_gameObject->m_transform.right.z;

	model->transform.m1 = in_gameObject->m_transform.up.x;
	model->transform.m5 = in_gameObject->m_transform.up.y;
	model->transform.m9 = in_gameObject->m_transform.up.z;

	model->transform.m2 = in_gameObject->m_transform.forward.x;
	model->transform.m6 = in_gameObject->m_transform.forward.y;
	model->transform.m10 = -in_gameObject->m_transform.forward.z;

	Vector3 pos = { in_gameObject->m_transform.position.x, in_gameObject->m_transform.position.y, in_gameObject->m_transform.position.z };
	DrawModel(*model, pos, 1, WHITE);
}

void RenderComponent::SetModel(Model* in_model)
{
	model = in_model;
}
