#include "WwiseRoomManager.h"

#include "AK/SpatialAudio/Common/AkSpatialAudioTypes.h"
bool WwiseRoomManager::IsPointInBoundingBox(const GO_Vector3& point, const GO_BoundingBox& box)
{
	if (point.x >= box.minPoint.x && point.x <= box.maxPoint.x &&
		point.y >= box.minPoint.y && point.y <= box.maxPoint.y &&
		point.z >= box.minPoint.z && point.z <= box.maxPoint.z) {
		return true; // Point is inside the bounding box
	}

	// Check for collision with sides of bounding box
	if (point.x < box.minPoint.x || point.x > box.maxPoint.x ||
		point.y < box.minPoint.y || point.y > box.maxPoint.y ||
		point.z < box.minPoint.z || point.z > box.maxPoint.z) {
		return false; // Point is outside the bounding box
	}

	return true;
}

void WwiseRoomManager::Update()
{
	for (WwiseRoomComponent* room : rooms)
	{
		for (WwiseObjectComponent* object : objects)
		{
			if (IsPointInBoundingBox(object->GetPosition(), room->GetBoundingBox()))
			{
				object->SetRoomID(room->GetRoomID());
			}
			else
			{
				object->SetRoomID(AK::SpatialAudio::kOutdoorRoomID);
			}
			
		}
	}
}

void WwiseRoomManager::AddRoom(WwiseRoomComponent* room)
{
	rooms.push_back(room);
}

void WwiseRoomManager::AddObject(WwiseObjectComponent* object)
{
	objects.push_back(object);
}
