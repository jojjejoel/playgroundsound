#include "RenderComponent.h"
#include "raylib.h"

void RenderComponent::Init()
{
}

void RenderComponent::Update(GameObject* parent)
{
	model->transform.m0 = -parent->m_transform.right.x;
	model->transform.m4 = -parent->m_transform.right.y;
	model->transform.m8 = parent->m_transform.right.z;

	model->transform.m1 = parent->m_transform.up.x;
	model->transform.m5 = parent->m_transform.up.y;
	model->transform.m9 = parent->m_transform.up.z;

	model->transform.m2 = parent->m_transform.forward.x;
	model->transform.m6 = parent->m_transform.forward.y;
	model->transform.m10 = -parent->m_transform.forward.z;

	Vector3 pos = { parent->m_transform.position.x, parent->m_transform.position.y, parent->m_transform.position.z };
	DrawModel(*model, pos, 1, WHITE);
}

void RenderComponent::SetModel(Model* in_model)
{
	model = in_model;
}
