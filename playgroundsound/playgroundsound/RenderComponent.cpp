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
}

void RenderComponent::Draw(GameObject* in_gameObject)
{
	Vector3 pos = { in_gameObject->m_transform.position.x, in_gameObject->m_transform.position.y, in_gameObject->m_transform.position.z };
	Color drawColor = { color.x, color.y, color.z, 255 };
	if (renderWireFrame)
	{
		DrawModelWires(*model, pos, 1, drawColor);
	}
	else
	{
		DrawModelEx(*model, pos, { 0.0f, 1.0f, 0.0f }, 0, { 1.0f, 1.0f, 1.0f }, drawColor);
	}
}

void RenderComponent::SetModel(Model* in_model, const bool& in_renderWireFrame, const bool& initRoom, const GO_Vector3& in_color )
{
	model = in_model;
	renderWireFrame = in_renderWireFrame;
	color = in_color;
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

void RenderComponent::SetRenderWireFrame(const bool& in_renderWireFrame)
{
	renderWireFrame = in_renderWireFrame;
}
