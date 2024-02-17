#include "GameObject.h"

GoVector3 GameObject::GetNormalizedForward() const
{
    return GetNormalized(forward.x, forward.y, forward.z);
}

GoVector3 GameObject::GetNormalizedUp() const
{
    return GetNormalized(up.x, up.y, up.z);
}

GoVector3 GameObject::GetNormalized(float x, float y, float z) const
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
