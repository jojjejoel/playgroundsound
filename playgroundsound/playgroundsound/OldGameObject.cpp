#include "OldGameObject.h"
#include "Game.h"

void OldGameObject::SetTransform(const GoTransform& in_transform)
{
    transform = in_transform;
}

void OldGameObject::SetModel(std::shared_ptr<Model> in_model)
{
    model = in_model;
}

const std::shared_ptr<Model> OldGameObject::GetModel() const {
    return model;
}

void OldGameObject::SetPosition(const GoVector3& in_position)
{
    transform.position = in_position;
}

void OldGameObject::SetForward(const GoVector3& in_forward)
{
    transform.forward = in_forward;
}

void OldGameObject::SetUp(const GoVector3& in_up)
{
    transform.up = in_up;
}

void OldGameObject::SetScale(const GoVector3& in_scale)
{
    transform.scale = in_scale;
}

const unsigned int& OldGameObject::GetID() const {
    return iD;
}

const GoVector3& OldGameObject::GetPosition() const
{
    return transform.position;
}

const GoVector3& OldGameObject::GetUp() const
{
    return transform.up;
}

const GoVector3& OldGameObject::GetForward() const
{
   return  transform.forward;
}

const GoVector3& OldGameObject::GetScale() const
{
    return  transform.scale;
}

GoVector3 OldGameObject::GetNormalizedForward() const
{
    return GetNormalized(transform.forward.x, transform.forward.y, transform.forward.z);
}

GoVector3 OldGameObject::GetNormalizedUp() const
{
    return GetNormalized(transform.up.x, transform.up.y, transform.up.z);
}

GoVector3 OldGameObject::GetNormalized(float x, float y, float z) const
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

void OldGameObject::SetScaleMultiplier(const float& in_scaleMultiplier)
{
    scaleMultiplier = in_scaleMultiplier;
}

void OldGameObject::SetID(const unsigned int& in_iD)
{
    iD = in_iD;
}

const bool& OldGameObject::GetIsWwiseRegistered() const
{
    return isWwiseRegistered;
}

void OldGameObject::SetName(const std::string_view in_name)
{
    name = in_name;
}

std::string_view OldGameObject::GetName() const
{
    return name;
}

void OldGameObject::SetRoomID(const unsigned int& in_roomID)
{
    roomID = in_roomID;
}

const unsigned int& OldGameObject::GetRoomID() const
{
    return roomID;
}

void OldGameObject::SetIsWwiseRegistered(const bool& in_isWwiseRegistered)
{
    isWwiseRegistered = in_isWwiseRegistered;
}

const float& OldGameObject::GetScaleMultiplier()
{
    return scaleMultiplier;
}
