#pragma once
#include "Component.h"

#include "..\Triangle.h"
#include "..\GameObject\GameObject.h"

#include "..\GameObject\GO_BoundingBox.h"
#include <cstdint>


class WwiseRoomComponent : public Component
{
public:
	~WwiseRoomComponent() override;
	virtual void Init(GameObject* in_gameObject) override;
	virtual void Update(GameObject* in_gameObject) override;
	
	void InitRoom(GameObject* in_gameObject);
	void InitRoomGeometry(GameObject* roomObj);

	void GenerateWalls(const GameObject* gameObject, const uint64_t& roomID, const uint64_t& wallSidesGeometryID, const uint64_t& wallCeilingFloorGeometryID, const uint64_t& wallInstance1, const uint64_t& wallInstance2, const uint64_t& wallInstance3, const uint64_t& wallInstance4, const uint64_t& wallInstance5, const uint64_t& wallInstance6);
	void SetBoundingBox(const GO_Vector3& center, const float& width, const float& length, const float& height);

	const GO_BoundingBox& GetBoundingBox();
	const unsigned int& GetRoomID();
private:
	GO_BoundingBox boundingBox;

	const unsigned int ROOM = 200;
	const unsigned int GEOMETRY_ROOM = 400;
	const unsigned int GEOMETRY_ROOM_INSTANCE = 401;

	const unsigned int GEOMETRY_WALL_SIDES = 1000;
	const unsigned int GEOMETRY_WALL_CEILINGFLOOR = 1001;
	const unsigned int GEOMETRY_WALL_INSTANCE_1 = 2000;
	const unsigned int GEOMETRY_WALL_INSTANCE_2 = 2001;
	const unsigned int GEOMETRY_WALL_INSTANCE_3 = 2002;
	const unsigned int GEOMETRY_WALL_INSTANCE_4 = 2003;
	const unsigned int GEOMETRY_WALL_INSTANCE_5 = 2004;
	const unsigned int GEOMETRY_WALL_INSTANCE_6 = 2005;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_1 = 2006;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_2 = 2007;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_3 = 2008;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_4 = 2009;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_5 = 2010;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_6 = 2011;
};

