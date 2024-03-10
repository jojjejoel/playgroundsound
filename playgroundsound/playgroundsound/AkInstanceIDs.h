#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include "AK/SpatialAudio/Common/AkSpatialAudioTypes.h"

namespace IDs {
	const AkGameObjectID musicEmitterGO = 1;
	const AkGameObjectID cameraGO = 2;
	const AkGameObjectID playerTruckGO = 3;
	const AkGameObjectID lightBulbGO = 4;
	const AkGameObjectID roomCubeGO = 5;
	const AkGameObjectID portalGO = 6;

	//Room Walls
	const AkGameObjectID roomWallLeftGO = 7;
	const AkGameObjectID roomWallRightGO = 8;
	const AkGameObjectID roomWallFrontGO = 9;
	const AkGameObjectID roomWallBackGO = 10;
	const AkGameObjectID roomWallTopGO = 11;
	const AkGameObjectID roomWallBottomGO = 12;


	const AkRoomID ROOM = 200;
	const AkPortalID PORTAL0 = 300;
	const AkPortalID PORTAL1 = 301;
	const AkGeometrySetID GEOMETRY_ROOM = 400;
	const AkGeometrySetID GEOMETRY_ROOM_INSTANCE = 401;

	const AkGeometrySetID GEOMETRY_WALL_SIDES = 1000;
	const AkGeometrySetID GEOMETRY_WALL_CEILINGFLOOR = 1001;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_1 = 2000;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_2 = 2001;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_3 = 2002;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_4 = 2003;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_5 = 2004;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_6 = 2005;


	const AkGeometrySetID GEOMETRY_WALL_OUTSIDE_SIDES = 1002;
	const AkGeometrySetID GEOMETRY_WALL_OUTSIDE_CEILINGFLOOR = 1003;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_1 = 2006;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_2 = 2007;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_3 = 2008;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_4 = 2009;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_5 = 2010;
	const AkGeometrySetID GEOMETRY_WALL_INSTANCE_OUTSIDE_6 = 2011;
}