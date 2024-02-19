#pragma once
#include <cmath>
#include "Mesh.h"
struct GoVector3 {
	float x, y, z;
};

class GameObject
{
public:
	GoVector3 position;
	GoVector3 forward;
	GoVector3 up;
	GoVector3 scale;

    GoVector3 GetNormalizedForward() const;
    GoVector3 GetNormalizedUp() const;
    
	Playground::Mesh mesh;

private:
	GoVector3 GetNormalized(float x, float y, float z) const;
};

