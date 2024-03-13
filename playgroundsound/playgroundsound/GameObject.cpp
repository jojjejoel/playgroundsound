#include "GameObject.h"
#include "Game.h"

void GameObject::SetTransform(const GoTransform& in_transform)
{
    transform = in_transform;
}

void GameObject::SetModel(std::shared_ptr<Model> in_model)
{
    model = in_model;
}

const std::shared_ptr<Model> GameObject::GetModel() const {
    return model;
}

void GameObject::SetPosition(const GoVector3& in_position)
{
    transform.position = in_position;
}

void GameObject::SetForward(const GoVector3& in_forward)
{
    transform.forward = in_forward;
}

void GameObject::SetUp(const GoVector3& in_up)
{
    transform.up = in_up;
}

void GameObject::SetScale(const GoVector3& in_scale)
{
    transform.scale = in_scale;
}

const unsigned int& GameObject::GetID() const {
    return iD;
}

const GoVector3& GameObject::GetPosition() const
{
    return transform.position;
}

const GoVector3& GameObject::GetUp() const
{
    return transform.up;
}

const GoVector3& GameObject::GetForward() const
{
   return  transform.forward;
}

const GoVector3& GameObject::GetScale() const
{
    return  transform.scale;
}

GoVector3 GameObject::GetNormalizedForward() const
{
    return GetNormalized(transform.forward.x, transform.forward.y, transform.forward.z);
}

GoVector3 GameObject::GetNormalizedUp() const
{
    return GetNormalized(transform.up.x, transform.up.y, transform.up.z);
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

void GameObject::SetScaleMultiplier(const float& in_scaleMultiplier)
{
    scaleMultiplier = in_scaleMultiplier;
}

void GameObject::SetID(const unsigned int& in_iD)
{
    iD = in_iD;
}

const bool& GameObject::GetIsWwiseRegistered() const
{
    return isWwiseRegistered;
}

void GameObject::SetName(const std::string_view in_name)
{
    name = in_name;
}

std::string_view GameObject::GetName() const
{
    return name;
}

void GameObject::SetRoomID(const unsigned int& in_roomID)
{
    roomID = in_roomID;
}

const unsigned int& GameObject::GetRoomID() const
{
    return roomID;
}

void GameObject::SetIsWwiseRegistered(const bool& in_isWwiseRegistered)
{
    isWwiseRegistered = in_isWwiseRegistered;
}

const float& GameObject::GetScaleMultiplier()
{
    return scaleMultiplier;
}
