#include "RenderComponent.h"

#include "raylib.h"
#include "raymath.h"

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
	if (renderWireFrame)
	{
		DrawModelWires(*model, pos, 100, WHITE);
	}
	else
	{
		Color color = { 255,255,255,120 };
		DrawModel(*model, pos, 1, color);
	}
}

void RenderComponent::SetModel(Model* in_model, const bool& in_renderWireFrame, const bool& initRoom)
{
	model = in_model;
	renderWireFrame = in_renderWireFrame;
	if (initRoom)
	{
		InitRoomGeometry();
	}
}

void RenderComponent::InitRoomGeometry() {
	for (size_t i = 0, v = 0; i < model->meshes[0].vertexCount; i++, v += 3)
	{
		GoVertex vertex;
		float* vertices = model->meshes[0].vertices;
		Vector3 vector3;
		vector3.x = vertices[v];
		vector3.y = vertices[v + 1];
		vector3.z = vertices[v + 2];

		vector3 = Vector3Transform(vector3, model->transform);

		vertex.x = vector3.x;
		vertex.y = vector3.y;
		vertex.z = -vector3.z;
		m_vertices.emplace_back(vertex);
	}

	for (size_t i = 0, v = 0; i < model->meshes[0].triangleCount; i++, v += 3)
	{
		Triangle triangle;
		triangle.point0 = model->meshes[0].indices[v];
		triangle.point1 = model->meshes[0].indices[v + 1];
		triangle.point2 = model->meshes[0].indices[v + 2];
		m_triangles.emplace_back(triangle);
	}
}

const std::vector<GoVertex>& RenderComponent::GetVertices() const
{
	return m_vertices;
}

const std::vector<Triangle>& RenderComponent::GetTriangles() const
{
	return m_triangles;
}