#include "WwiseRoomComponent.h"

#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "RenderComponent.h"
#include "Mesh.h"

GO_Vector3 Vector3Transform(GO_Vector3 v, GO_Matrix mat)
{
	GO_Vector3 result = { 0 };

	float x = v.x;
	float y = v.y;
	float z = v.z;

	result.x = mat.m0 * x + mat.m4 * y + mat.m8 * z + mat.m12;
	result.y = mat.m1 * x + mat.m5 * y + mat.m9 * z + mat.m13;
	result.z = mat.m2 * x + mat.m6 * y + mat.m10 * z + mat.m14;

	return result;
}


void WwiseRoomComponent::Init(GameObject* in_gameObject)
{
	
}

void WwiseRoomComponent::InitRoom(GameObject* in_gameObject) {
	AKRESULT result;

	std::vector<GoVertex> vertices = in_gameObject->GetComponent<RenderComponent>().GetVertices();
	std::vector<Triangle> triangles = in_gameObject->GetComponent<RenderComponent>().GetTriangles();

	AkGeometryParams geomWallsInside;
	geomWallsInside.NumVertices = vertices.size();
	std::vector<AkVertex> akVertices;
	for (size_t i = 0; i < geomWallsInside.NumVertices; i++)
	{
		AkVertex vertex;
		vertex.X = vertices[i].x;
		vertex.Y = vertices[i].y;
		vertex.Z = vertices[i].z;
		akVertices.emplace_back(vertex);
	}

	geomWallsInside.Vertices = akVertices.data();

	geomWallsInside.NumSurfaces = 2;
	AkAcousticSurface surfaces[2];
	AkPlacementNew(&surfaces[0]) AkAcousticSurface();
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 0.5f;
	AkPlacementNew(&surfaces[1]) AkAcousticSurface();
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 0.5f;
	geomWallsInside.Surfaces = surfaces;
	geomWallsInside.NumTriangles = triangles.size();

	std::vector<AkTriangle> akTriangles;

	for (size_t i = 0; i < geomWallsInside.NumTriangles; i++)
	{
		AkTriangle akTriangle;
		akTriangle.point0 = triangles[i].point0;
		akTriangle.point1 = triangles[i].point1;
		akTriangle.point2 = triangles[i].point2;
		akTriangle.surface = 0;
		akTriangles.emplace_back(akTriangle);
	}

	geomWallsInside.Triangles = akTriangles.data();

	geomWallsInside.EnableDiffraction = true;
	geomWallsInside.EnableDiffractionOnBoundaryEdges = true;
	geomWallsInside.EnableTriangles = true;


	result = AK::SpatialAudio::SetGeometry(GEOMETRY_WALL_SIDES, geomWallsInside);
	if (result != AK_Success)
	{
		// Log error/warning
	}

	AkGeometryParams geomRoofCeilingInside = geomWallsInside;
	geomRoofCeilingInside.EnableDiffractionOnBoundaryEdges = false;
	result = AK::SpatialAudio::SetGeometry(GEOMETRY_WALL_CEILINGFLOOR, geomRoofCeilingInside);
	if (result != AK_Success)
	{
		// Log error/warning
	}

	GenerateWalls(in_gameObject, ROOM, GEOMETRY_WALL_SIDES, GEOMETRY_WALL_CEILINGFLOOR,
		GEOMETRY_WALL_INSTANCE_1,
		GEOMETRY_WALL_INSTANCE_2,
		GEOMETRY_WALL_INSTANCE_3,
		GEOMETRY_WALL_INSTANCE_4,
		GEOMETRY_WALL_INSTANCE_5,
		GEOMETRY_WALL_INSTANCE_6);

	GenerateWalls(in_gameObject, AK::SpatialAudio::kOutdoorRoomID, GEOMETRY_WALL_SIDES, GEOMETRY_WALL_CEILINGFLOOR,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_1,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_2,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_3,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_4,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_5,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_6);



	AkRoomParams paramsRoom;
	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;
	paramsRoom.TransmissionLoss = 0.5f;
	paramsRoom.RoomGameObj_KeepRegistered = true;
	paramsRoom.RoomGameObj_AuxSendLevelToSelf = 0.25f;
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Inside");
	paramsRoom.GeometryInstanceID = GEOMETRY_ROOM_INSTANCE;

	result = AK::SpatialAudio::SetRoom(ROOM, paramsRoom, "Inside");
	if (result != AK_Success)
	{
		// Log error/warning
	}

	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;

	paramsRoom.TransmissionLoss = 0.5f;
	paramsRoom.RoomGameObj_KeepRegistered = true;
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Outside");
	paramsRoom.GeometryInstanceID = AkGeometryInstanceID();
	result = AK::SpatialAudio::SetRoom(AK::SpatialAudio::kOutdoorRoomID, paramsRoom, "Outside");
	if (result != AK_Success)
	{
		// Log error/warning
	}
}

void WwiseRoomComponent::Update(GameObject* in_gameObject)
{
}

void WwiseRoomComponent::GenerateWalls(const GameObject* gameObject, const unsigned int& roomID,
	const unsigned int& wallSidesGeometryID,
	const unsigned int& wallCeilingFloorGeometryID,
	const unsigned int& wallInstance1,
	const unsigned int& wallInstance2,
	const unsigned int& wallInstance3,
	const unsigned int& wallInstance4,
	const unsigned int& wallInstance5,
	const unsigned int& wallInstance6)
{
	AkGeometrySetID geometryInstanceID;
	AkGeometrySetID akGeometrySetID;
	for (size_t i = 0; i < 6; i++)
	{
		AkGeometryInstanceParams instanceParams;
		instanceParams.Scale = { 1.0001,1.0001,1.0001 };
		AkWorldTransform worldTransform;
		AkVector topVector = { 0,1,0 };
		AkVector frontVector = { 0,0,1 };
		AkVector64 position;

		if (i == 0)
		{
			geometryInstanceID = wallInstance1;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y, gameObject->m_transform.position.z + 2.5f };
			frontVector = { 1,0,0 };
		}
		else if (i == 1)
		{
			geometryInstanceID = wallInstance2;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y, gameObject->m_transform.position.z - 2.5f };
			frontVector = { -1,0,0 };

		}
		else if (i == 2)
		{
			geometryInstanceID = wallInstance3;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x + 2.5f, gameObject->m_transform.position.y, gameObject->m_transform.position.z };
			frontVector = { 0,0,1 };

		}
		else if (i == 3)
		{
			geometryInstanceID = wallInstance4;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x - 2.5, gameObject->m_transform.position.y, gameObject->m_transform.position.z };
			frontVector = { 0,0,-1 };
		}
		else if (i == 4)
		{
			geometryInstanceID = wallInstance5;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y + 2.5f, gameObject->m_transform.position.z };
			frontVector = { 1,0,0 };
			topVector = { 0,0,1 };

		}
		else if (i == 5)
		{
			geometryInstanceID = wallInstance6;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y - 2.5f, gameObject->m_transform.position.z };
			frontVector = { -1,0,0 };
			topVector = { 0,0,1 };

		}

		worldTransform.Set(position, frontVector, topVector);

		instanceParams.PositionAndOrientation = worldTransform;
		instanceParams.GeometrySetID = akGeometrySetID;
		instanceParams.RoomID = roomID;

		AK::SpatialAudio::SetGeometryInstance(geometryInstanceID, instanceParams);
	}
}

void WwiseRoomComponent::SetBoundingBox(const GO_Vector3& center, const float& width, const float& length, const float& height)
{
	boundingBox = { center, width, length, height };
}

const GO_BoundingBox& WwiseRoomComponent::GetBoundingBox()
{
	return boundingBox;
}

const unsigned int& WwiseRoomComponent::GetRoomID()
{
	return ROOM;
}

