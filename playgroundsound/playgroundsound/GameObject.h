#pragma once
#include <cmath>
#include "Mesh.h"
struct GoVector3 {
	float x, y, z;
};

struct Triangle
{
	int point0, point1, point2;
};

struct GoTransform {
	GoVector3 position;
	GoVector3 forward;
	GoVector3 up;
	GoVector3 scale;
};
class GameObject
{
public:
	const GoTransform& GetTransform() const;
	const GoVector3& GetPosition() const;
	const GoVector3& GetUp() const;
	const GoVector3& GetForward() const;
	void SetTransform(const GoTransform& transform);
	void SetPosition(const GoVector3& position);
	void SetForward(const GoVector3& in_position);
	void SetUp(const GoVector3& in_position);

    GoVector3 GetNormalizedForward() const;
    GoVector3 GetNormalizedUp() const;
    
	Playground::Mesh mesh;
	std::vector<Triangle> triangles;

private:
	GoVector3 GetNormalized(float x, float y, float z) const;

	GoTransform transform;
};

