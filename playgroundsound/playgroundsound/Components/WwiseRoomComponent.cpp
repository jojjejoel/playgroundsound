#include "WwiseRoomComponent.h"

#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "RenderComponent.h"
#include "..\GameObject\GoMesh.h"

void WwiseRoomComponent::Init(GameObject* in_gameObject)
{
	
}

void WwiseRoomComponent::InitRoom(GameObject* in_gameObject) {
	auto& renderComponent = in_gameObject->GetComponent<RenderComponent>();
	const auto& vertices = renderComponent.GetVertices();
	const auto& triangles = renderComponent.GetTriangles();

	std::vector<AkVertex> akVertices;
	akVertices.reserve(vertices.size());
	for (const auto& vertex : vertices) {
		akVertices.push_back({ vertex.x, vertex.y, vertex.z });
	}

	AkAcousticSurface surfaces[2];
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 0.8f;
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 0.8f;
	
	std::vector<AkTriangle> akTriangles;
	akTriangles.reserve(triangles.size());
	for (const auto& triangle : triangles) {
		akTriangles.push_back({(AkVertIdx)triangle.point0, (AkVertIdx)triangle.point1, (AkVertIdx)triangle.point2, (AkSurfIdx)0 });
	}

	auto setupGeometryParams = [&](AkGeometryParams& geomParams) {
		geomParams.NumVertices = static_cast<AkVertIdx>(akVertices.size());
		geomParams.Vertices = akVertices.data();
		geomParams.NumSurfaces = 2;
		geomParams.Surfaces = surfaces;
		geomParams.NumTriangles = static_cast<AkTriIdx>(akTriangles.size());
		geomParams.Triangles = akTriangles.data();
		geomParams.EnableDiffraction = true;
		geomParams.EnableDiffractionOnBoundaryEdges = true;
		geomParams.EnableTriangles = true;
		};

	AkGeometryParams geomWallsInside;
	setupGeometryParams(geomWallsInside);

	AK::SpatialAudio::SetGeometry(GEOMETRY_WALL_SIDES, geomWallsInside);

	AkGeometryParams geomFloorCeilingInside = geomWallsInside;
	geomFloorCeilingInside.EnableDiffractionOnBoundaryEdges = false;

	AK::SpatialAudio::SetGeometry(GEOMETRY_WALL_CEILINGFLOOR, geomFloorCeilingInside);

	GenerateWalls(in_gameObject, ROOM, GEOMETRY_WALL_SIDES, GEOMETRY_WALL_CEILINGFLOOR,
		GEOMETRY_WALL_INSTANCE_1, GEOMETRY_WALL_INSTANCE_2, GEOMETRY_WALL_INSTANCE_3,
		GEOMETRY_WALL_INSTANCE_4, GEOMETRY_WALL_INSTANCE_5, GEOMETRY_WALL_INSTANCE_6);
}

void WwiseRoomComponent::Update(GameObject* in_gameObject)
{
}

void WwiseRoomComponent::InitRoomGeometry(GameObject* roomObj) {

	AKRESULT result;
	
	AkGeometryParams geom;
	geom.NumVertices = static_cast<AkVertIdx>(roomObj->GetComponent<RenderComponent>().GetVertices().size());
	std::vector<AkVertex> vertices;
	for (size_t i = 0; i < geom.NumVertices; i++)
	{
		AkVertex vertex;
		vertex.X = roomObj->GetComponent<RenderComponent>().GetVertices()[i].x;
		vertex.Y = roomObj->GetComponent<RenderComponent>().GetVertices()[i].y;
		vertex.Z = roomObj->GetComponent<RenderComponent>().GetVertices()[i].z;
		vertices.emplace_back(vertex);
	}

	geom.Vertices = vertices.data();

	geom.NumSurfaces = 2;
	AkAcousticSurface surfaces[2];
	AkPlacementNew(&surfaces[0]) AkAcousticSurface();
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 0.8f;
	AkPlacementNew(&surfaces[1]) AkAcousticSurface();
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 0.8f;
	geom.Surfaces = surfaces;
	geom.NumTriangles = static_cast<AkTriIdx>(roomObj->GetComponent<RenderComponent>().GetTriangles().size());

	std::vector<AkTriangle> akTriangles;

	for (size_t i = 0; i < geom.NumTriangles; i++)
	{
		AkTriangle akTriangle;
		akTriangle.point0 = roomObj->GetComponent<RenderComponent>().GetTriangles()[i].point0;
		akTriangle.point1 = roomObj->GetComponent<RenderComponent>().GetTriangles()[i].point1;
		akTriangle.point2 = roomObj->GetComponent<RenderComponent>().GetTriangles()[i].point2;
		akTriangle.surface = 0;
		akTriangles.emplace_back(akTriangle);
	}

	geom.Triangles = akTriangles.data();

	geom.EnableDiffraction = true;
	geom.EnableDiffractionOnBoundaryEdges = false;
	geom.EnableTriangles = true;

	result = AK::SpatialAudio::SetGeometry(GEOMETRY_ROOM, geom);

	AkGeometryInstanceParams instanceParams;

	instanceParams.GeometrySetID = GEOMETRY_ROOM;
	instanceParams.RoomID = ROOM;

	result = AK::SpatialAudio::SetGeometryInstance(GEOMETRY_ROOM_INSTANCE, instanceParams);



	AkRoomParams paramsRoom;

	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;

	paramsRoom.TransmissionLoss = 0.f;
	paramsRoom.RoomGameObj_KeepRegistered = true;
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Outside");
	paramsRoom.GeometryInstanceID = AkGeometryInstanceID();
	result = AK::SpatialAudio::SetRoom(AK::SpatialAudio::kOutdoorRoomID, paramsRoom, "Outside");	


	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;
	paramsRoom.TransmissionLoss = 0.f;
	paramsRoom.RoomGameObj_KeepRegistered = true;
	paramsRoom.RoomGameObj_AuxSendLevelToSelf = 0;
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Inside");
	paramsRoom.GeometryInstanceID = GEOMETRY_ROOM_INSTANCE;

	result = AK::SpatialAudio::SetRoom(ROOM, paramsRoom, "Inside");
}

void WwiseRoomComponent::GenerateWalls(const GameObject* gameObject, const uint64_t& roomID,
	const uint64_t& wallSidesGeometryID,
	const uint64_t& wallCeilingFloorGeometryID,
	const uint64_t& wallInstance1,
	const uint64_t& wallInstance2,
	const uint64_t& wallInstance3,
	const uint64_t& wallInstance4,
	const uint64_t& wallInstance5,
	const uint64_t& wallInstance6)
{
	AkGeometrySetID geometryInstanceID;
	AkGeometrySetID akGeometrySetID;
	for (size_t i = 0; i < 6; i++)
	{
		AkGeometryInstanceParams instanceParams;
		instanceParams.Scale = { 1.0001f,1.0001f,1.0001f };
		AkWorldTransform worldTransform;
		AkVector topVector = { 0,1,0 };
		AkVector frontVector = { 0,0,1 };
		AkVector64 position;

		if (i == 0)
		{
			geometryInstanceID = wallInstance1;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y, gameObject->m_transform.position.z + 5 };
			frontVector = { 1,0,0 };
		}
		else if (i == 1)
		{
			geometryInstanceID = wallInstance2;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y, gameObject->m_transform.position.z - 5 };
			frontVector = { -1,0,0 };
		}
		else if (i == 2)
		{
			geometryInstanceID = wallInstance3;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x + 5, gameObject->m_transform.position.y, gameObject->m_transform.position.z };
			frontVector = { 0,0,1 };

		}
		else if (i == 3)
		{
			geometryInstanceID = wallInstance4;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->m_transform.position.x - 5, gameObject->m_transform.position.y, gameObject->m_transform.position.z };
			frontVector = { 0,0,-1 };
		}
		else if (i == 4)
		{
			geometryInstanceID = wallInstance5;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y + 5, gameObject->m_transform.position.z };
			frontVector = { 1,0,0 };
			topVector = { 0,0,1 };

		}
		else if (i == 5)
		{
			geometryInstanceID = wallInstance6;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->m_transform.position.x, gameObject->m_transform.position.y - 5, gameObject->m_transform.position.z };
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

