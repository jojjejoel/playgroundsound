#include "WwiseRoomComponent.h"

#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "RenderComponent.h"
#include "..\GameObject\GoMesh.h"

WwiseRoomComponent::~WwiseRoomComponent()
{
	AK::SpatialAudio::RemoveRoom(ROOM);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_ROOM_INSTANCE);

	AK::SpatialAudio::RemoveGeometry(GEOMETRY_WALL_SIDES);
	AK::SpatialAudio::RemoveGeometry(GEOMETRY_WALL_CEILINGFLOOR);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_1);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_2);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_3);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_4);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_5);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_6);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_1);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_2);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_3);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_4);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_5);
	AK::SpatialAudio::RemoveGeometryInstance(GEOMETRY_WALL_INSTANCE_OUTSIDE_6);
}

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
		akTriangles.push_back({
			(AkVertIdx)triangle.point0,
			(AkVertIdx)triangle.point1,
			(AkVertIdx)triangle.point2,
			(AkSurfIdx)0 });
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

	GenerateWalls(in_gameObject, AK::SpatialAudio::kOutdoorRoomID, GEOMETRY_WALL_SIDES, GEOMETRY_WALL_CEILINGFLOOR,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_1, GEOMETRY_WALL_INSTANCE_OUTSIDE_2, GEOMETRY_WALL_INSTANCE_OUTSIDE_3,
		GEOMETRY_WALL_INSTANCE_OUTSIDE_4, GEOMETRY_WALL_INSTANCE_OUTSIDE_5, GEOMETRY_WALL_INSTANCE_OUTSIDE_6);
}

void WwiseRoomComponent::Update(GameObject* in_gameObject)
{
}

void WwiseRoomComponent::InitRoomGeometry(GameObject* roomObj) {
	AkGeometryParams geom;

	// Set vertices
	auto& renderComponent = roomObj->GetComponent<RenderComponent>();
	const auto& verticesData = renderComponent.GetVertices();
	std::vector<AkVertex> vertices(verticesData.size());
	for (size_t i = 0; i < verticesData.size(); ++i) {
		vertices[i] = { verticesData[i].x, verticesData[i].y, verticesData[i].z };
	}
	geom.NumVertices = static_cast<AkVertIdx>(vertices.size());
	geom.Vertices = vertices.data();

	// Set surfaces
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

	// Set triangles
	const auto& trianglesData = renderComponent.GetTriangles();
	std::vector<AkTriangle> akTriangles(trianglesData.size());
	for (size_t i = 0; i < trianglesData.size(); ++i) {
		akTriangles[i] = {
			(AkVertIdx)trianglesData[i].point0,
			(AkVertIdx)trianglesData[i].point1,
			(AkVertIdx)trianglesData[i].point2,
			(AkSurfIdx)0 };
	}
	geom.NumTriangles = static_cast<AkTriIdx>(akTriangles.size());
	geom.Triangles = akTriangles.data();

	// Additional geometry parameters
	geom.EnableDiffraction = true;
	geom.EnableDiffractionOnBoundaryEdges = false;
	geom.EnableTriangles = true;

	// Set geometry
	AK::SpatialAudio::SetGeometry(GEOMETRY_ROOM, geom);

	// Set geometry instance
	AkGeometryInstanceParams instanceParams;
	instanceParams.GeometrySetID = GEOMETRY_ROOM;
	instanceParams.RoomID = ROOM;
	AK::SpatialAudio::SetGeometryInstance(GEOMETRY_ROOM_INSTANCE, instanceParams);

	// Set room parameters
	AkRoomParams paramsRoom;
	paramsRoom.Front = { 0.f, 0.0f, 1.0f };
	paramsRoom.Up = { 0.f, 1.0f, 0.0f };
	paramsRoom.TransmissionLoss = 0.f;
	paramsRoom.RoomGameObj_KeepRegistered = true;

	// Set outdoor room
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Outside");
	paramsRoom.GeometryInstanceID = AkGeometryInstanceID();
	AK::SpatialAudio::SetRoom(AK::SpatialAudio::kOutdoorRoomID, paramsRoom, "Outside");

	// Set indoor room
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Inside");
	paramsRoom.GeometryInstanceID = GEOMETRY_ROOM_INSTANCE;
	AK::SpatialAudio::SetRoom(ROOM, paramsRoom, "Inside");
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

