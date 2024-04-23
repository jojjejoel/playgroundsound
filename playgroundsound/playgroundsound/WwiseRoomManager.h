#pragma once

#include "GameObject.h"
#include "WwiseRoomComponent.h"
#include "WwiseObjectComponent.h"

class WwiseRoomManager
{

public:
	bool IsPointInBoundingBox(const GO_Vector3& point, const GO_BoundingBox& boundingBox);

	void Update();

	void AddRoom(WwiseRoomComponent* room);
	void AddObject(WwiseObjectComponent* object);

private:
	std::vector<WwiseRoomComponent*> rooms;

	std::vector<WwiseObjectComponent*> objects;
};
