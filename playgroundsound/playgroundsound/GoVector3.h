#pragma once
#include <cmath>
struct GoVector3 {
	float x, y, z;

	GoVector3 operator*(float scalar) const {
		return GoVector3(x * scalar, y * scalar, z * scalar);
	}

    GoVector3 operator+(const GoVector3& other) const {
        return GoVector3(x + other.x, y + other.y, z + other.z);
    }

    GoVector3 operator-(const GoVector3& other) const {
        return GoVector3(x - other.x, y - other.y, z - other.z);
    }

    // Overloading the += operator for vector addition and assignment
    GoVector3& operator+=(const GoVector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this; // Return a reference to the modified vector
    }

    // Overloading the -= operator for vector subtraction and assignment
    GoVector3& operator-=(const GoVector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this; // Return a reference to the modified vector
    }


    GoVector3 Normalized() const
    {
        float magnitude = std::sqrt(x * x + y * y + z * z);

        // Check if magnitude is non-zero to avoid division by zero
        if (magnitude != 0.0f) {
            return { x / magnitude, y / magnitude, z / magnitude };
        }
        else {
            return { 0.0f, 0.0f, 0.0f };  // Returning a default value
        }
    }
};