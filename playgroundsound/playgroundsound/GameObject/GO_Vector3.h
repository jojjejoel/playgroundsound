#pragma once
#include <cmath>

struct GO_Vector3 {
    float x, y, z;

    GO_Vector3 operator*(const float scalar) const {
        return {x * scalar, y * scalar, z * scalar};
    }

    GO_Vector3 operator+(const GO_Vector3& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    GO_Vector3 operator-(const GO_Vector3& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    // Overloading the += operator for vector addition and assignment
    GO_Vector3& operator+=(const GO_Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this; // Return a reference to the modified vector
    }

    // Overloading the -= operator for vector subtraction and assignment
    GO_Vector3& operator-=(const GO_Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this; // Return a reference to the modified vector
    }


    GO_Vector3 Normalized() const {
        // Check if magnitude is non-zero to avoid division by zero
        if (const float magnitude = std::sqrt(x * x + y * y + z * z); magnitude != 0.0f) {
            return {x / magnitude, y / magnitude, z / magnitude};
        }
        return {0.0f, 0.0f, 0.0f}; // Returning a default value
    }
};
