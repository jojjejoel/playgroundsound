#pragma once
#include "GO_Vector3.h"
struct GO_BoundingBox
{
    GO_BoundingBox() = default;
	GO_BoundingBox(const GO_Vector3& center, const float& width, const float& length, const float& height) {
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        float halfLength = length / 2.0f;

        // Calculate min and max points
        minPoint = { center.x - halfWidth,
                          center.y - halfHeight,
                          center.z - halfLength };

        maxPoint = { center.x + halfWidth,
                          center.y + halfHeight,
                          center.z + halfLength };
	}
	GO_Vector3 minPoint;
	GO_Vector3 maxPoint;
};

