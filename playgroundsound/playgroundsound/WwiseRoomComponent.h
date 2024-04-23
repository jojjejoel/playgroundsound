#pragma once
#include "Component.h"
//#include <AK/SoundEngine/Common/AkTypes.h>
//#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>

#include "Triangle.h"
#include "GameObject.h"

#include "GO_BoundingBox.h"

class WwiseRoomComponent : public Component
{
public:
	virtual void Init(GameObject* in_gameObject) override;
	void InitRoom(GameObject* in_gameObject);
	virtual void Update(GameObject* in_gameObject) override;

	void GenerateWalls(const GameObject* gameObject, const unsigned int& roomID, const unsigned int& wallSidesGeometryID, const unsigned int& wallCeilingFloorGeometryID, const unsigned int& wallInstance1, const unsigned int& wallInstance2, const unsigned int& wallInstance3, const unsigned int& wallInstance4, const unsigned int& wallInstance5, const unsigned int& wallInstance6);

	void SetBoundingBox(const GO_Vector3& center, const float& width, const float& length, const float& height);

	const GO_BoundingBox& GetBoundingBox();

	const unsigned int& GetRoomID();

private:
	//AkRoomID roomID;

	GO_BoundingBox boundingBox;

		const unsigned int ROOM = 200;
		const unsigned int PORTAL0 = 300;
		const unsigned int PORTAL1 = 301;
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
		const unsigned int GEOMETRY_WALL_OUTSIDE_SIDES = 1002;
	const unsigned int GEOMETRY_WALL_OUTSIDE_CEILINGFLOOR = 1003;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_1 = 2006;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_2 = 2007;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_3 = 2008;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_4 = 2009;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_5 = 2010;
	const unsigned int GEOMETRY_WALL_INSTANCE_OUTSIDE_6 = 2011;
};

