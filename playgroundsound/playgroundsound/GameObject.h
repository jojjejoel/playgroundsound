#pragma once
#include <cmath>
#include "Mesh.h"
#include <memory>
struct Model;

struct GoVector3 {
	float x, y, z;
};

struct Triangle
{
	int point0, point1, point2;
};

struct GoTransform {
	GoVector3 position;
	GoVector3 forward { 0,0,1 };
	GoVector3 up { 0,1,0 };
	GoVector3 scale{ 1,1,1 };
};
class GameObject
{
public:
	const GoVector3& GetPosition() const;
	const GoVector3& GetUp() const;
	const GoVector3& GetForward() const;
	const GoVector3& GetScale() const;
	void SetTransform(const GoTransform& transform);
	void SetModel(std::shared_ptr<Model> in_model);
	std::shared_ptr<Model> GetModel();
	void SetPosition(const GoVector3& position);
	void SetForward(const GoVector3& in_position);
	void SetUp(const GoVector3& in_position);

	void SetScale(const GoVector3& in_scale);

    GoVector3 GetNormalizedForward() const;
    GoVector3 GetNormalizedUp() const;
    
	Playground::Mesh mesh;
	std::vector<Triangle> triangles;

	void SetScaleMultiplier(const float& in_scaleMultiplier);
	const float& GetScaleMultiplier();

private:
	GoVector3 GetNormalized(float x, float y, float z) const;

	GoTransform transform;

	std::shared_ptr<Model> model;

	float scaleMultiplier = 1.0f;
};

