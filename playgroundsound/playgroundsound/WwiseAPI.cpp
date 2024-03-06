#include "WwiseAPI.h"
#include <iostream>

//// Memory management
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
//
//// Streaming
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

//// File packaging
#include "AkFilePackage.h"
#include "AkFilePackageLUT.h"

// Communication
#include "AK/Comm/AkCommunication.h"

#include "AK/SpatialAudio/Common/AkSpatialAudio.h"


//Plugins
#include "AK/Plugin/AkRoomVerbFXFactory.h"
#include <AK/Plugin/AkMeterFXFactory.h>
#include <AK/Plugin/AkGainFXFactory.h>
#include <AK/Plugin/AkParametricEQFXFactory.h>


//Get RTPC (Query)
#include <AK/SoundEngine/Common/AkQueryParameters.h>


//IDs
#include "Wwise_IDs.h"


#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include "AkInstanceIDs.h"


bool WwiseAPI::Init()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);
	AK::MemoryMgr::Init(&memSettings);

	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	AK::StreamMgr::Create(streamSettings);

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	g_lowLevelIO.Init(deviceSettings);


	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);

	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	AK::SoundEngine::Init(&initSettings, &platformInitSettings);

	AkSpatialAudioInitSettings spatialAudioInitSettings;
	AK::SpatialAudio::Init(spatialAudioInitSettings);

	AkMusicSettings musicInitSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicInitSettings);
	AK::MusicEngine::Init(&musicInitSettings);

	g_lowLevelIO.SetBasePath(AKTEXT("..\\Wwise\\GeneratedSoundBanks\\Windows"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

#ifndef AK_OPTIMIZED

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	AK::Comm::Init(commSettings);

#endif // !AK_OPTIMIZED


	return true;
}

void WwiseAPI::DeInit()
{
	AK::SoundEngine::Term();
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}
	AK::Comm::Term();
	AK::MemoryMgr::Term();
}

AKRESULT WwiseAPI::LoadBank(const AkUniqueID& bankID)
{
	return AK::SoundEngine::LoadBank(bankID);
}

float WwiseAPI::GetGameParamValue(const AkUniqueID& rtpcID, const AkGameObjectID& akGameObjectID) {

	AkRtpcValue rtpcValue;
	AK::SoundEngine::Query::RTPCValue_type valueType = AK::SoundEngine::Query::RTPCValue_Global;
	AKRESULT result = AK::SoundEngine::Query::GetRTPCValue(rtpcID, akGameObjectID, AK_INVALID_PLAYING_ID, rtpcValue, valueType);

	float minRTPCValue = -36;
	float maxRTPCValue = -16;

	float normalizedValue = (rtpcValue - (minRTPCValue)) / (maxRTPCValue - (minRTPCValue));
	return normalizedValue;
}

void WwiseAPI::RenderAudio()
{
	AK::SoundEngine::RenderAudio();
}

std::vector<DiffractionPath> WwiseAPI::GetDiffraction(const AkGameObjectID& gameObjectID) {
	AkVector64 emitterPos;
	AkVector64 listenerPos;
	AkDiffractionPathInfo akDiffractionPaths[8];
	AkUInt32 numberOfPaths = 8;
	AK::SpatialAudio::QueryDiffractionPaths(gameObjectID, 0, listenerPos, emitterPos, akDiffractionPaths, numberOfPaths);

	int numLines = 0;

	std::vector<DiffractionPath> diffractionPaths;
	for (AkUInt32 path = 0; path < numberOfPaths; path++)
	{
		DiffractionPath diffractionPath;
		AkDiffractionPathInfo akDiffractionPath = akDiffractionPaths[path];
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.nodes[i].x = akDiffractionPath.nodes[i].X;
			diffractionPath.nodes[i].y = akDiffractionPath.nodes[i].Y;
			diffractionPath.nodes[i].z = -akDiffractionPath.nodes[i].Z;
		}
		diffractionPath.emitterPos.x = emitterPos.X;
		diffractionPath.emitterPos.y = emitterPos.Y;
		diffractionPath.emitterPos.z = emitterPos.Z;
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.angles[i] = akDiffractionPath.angles[i];
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.portals[i] = akDiffractionPath.portals[i];
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.rooms[i] = akDiffractionPath.rooms[i];
		}
		diffractionPath.virtualPos.forward.x = -akDiffractionPath.virtualPos.OrientationFront().X;
		diffractionPath.virtualPos.forward.y = -akDiffractionPath.virtualPos.OrientationFront().Y;
		diffractionPath.virtualPos.forward.z = akDiffractionPath.virtualPos.OrientationFront().Z;
		diffractionPath.virtualPos.up.x = akDiffractionPath.virtualPos.OrientationTop().X;
		diffractionPath.virtualPos.up.y = akDiffractionPath.virtualPos.OrientationTop().Y;
		diffractionPath.virtualPos.up.z = -akDiffractionPath.virtualPos.OrientationTop().Z;
		diffractionPath.virtualPos.position.x = akDiffractionPath.virtualPos.Position().X;
		diffractionPath.virtualPos.position.y = akDiffractionPath.virtualPos.Position().Y;
		diffractionPath.virtualPos.position.z = -akDiffractionPath.virtualPos.Position().Z;
		diffractionPath.nodeCount = akDiffractionPath.nodeCount;
		diffractionPath.diffraction = akDiffractionPath.diffraction;
		diffractionPath.totLength = akDiffractionPath.totLength;
		diffractionPath.obstructionValue = akDiffractionPath.obstructionValue;
		diffractionPaths.emplace_back(diffractionPath);
	}
	return diffractionPaths;
}


void WwiseAPI::Log(std::string_view logMsg) {
	std::cout << logMsg << std::endl;
}

AKRESULT WwiseAPI::RegisterGameObject(const AkGameObjectID& gameObjectID, std::string_view gameObjectName)
{
	akGameObjects.emplace_back(gameObjectID);
	return AK::SoundEngine::RegisterGameObj(gameObjectID, gameObjectName.data());
}

AKRESULT WwiseAPI::SetPlayerIsInRoom(const bool& isInRoom)
{
	AkRoomID currentRoom = isInRoom ? IDs::ROOM : AK::SpatialAudio::kOutdoorRoomID;
	AK::SpatialAudio::SetGameObjectInRoom(IDs::distanceProbeObjectID, currentRoom);
	return AK::SpatialAudio::SetGameObjectInRoom(IDs::listenerObjectID, currentRoom);
}

AKRESULT WwiseAPI::AddListener() {

	RegisterGameObject(IDs::musicObjectID, "Music");
	RegisterGameObject(IDs::listenerObjectID, "Listener");
	RegisterGameObject(IDs::distanceProbeObjectID, "DistanceProbe");

	AK::SoundEngine::AddDefaultListener(IDs::listenerObjectID);
	AK::SoundEngine::SetDistanceProbe(IDs::listenerObjectID, IDs::distanceProbeObjectID);

	AK::SpatialAudio::RegisterListener(IDs::listenerObjectID);
	//AK::SoundEngine::PostEvent("Car_Engine_Loop", IDs::distanceProbeObjectID);
	return AK_Success;
}

AkPlayingID WwiseAPI::PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID)
{
	AK::SpatialAudio::SetGameObjectInRoom(gameObjectID, IDs::ROOM);
	return AK::SoundEngine::PostEvent(eventID, gameObjectID, AK_MusicSyncBeat | AK_MusicSyncBar, &WwiseAPI::EventCallback, this);
}

AKRESULT WwiseAPI::UpdateGameObject(const AkGameObjectID& akGameObjectID, const GameObject& gameObject)
{
	AkListenerPosition soundPosition;
	AkVector positionVector = { gameObject.GetPosition().x, gameObject.GetPosition().y ,gameObject.GetPosition().z };
	GoVector3 forwardNormalized = gameObject.GetNormalizedForward();
	GoVector3 upNormalized = gameObject.GetNormalizedUp();
	AkVector orientationFront = { forwardNormalized.x, forwardNormalized.y, forwardNormalized.z };
	AkVector orientationTop = { upNormalized.x, upNormalized.y, upNormalized.z };
	AkTransform transform;
	transform.Set(positionVector, orientationFront, orientationTop);
	soundPosition.Set(positionVector, orientationFront, orientationTop);

	AKRESULT result = AK::SoundEngine::SetPosition(akGameObjectID, soundPosition);
	return result;
}


AKRESULT WwiseAPI::AddRoom() {
	AKRESULT result;

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
	paramsRoom.GeometryInstanceID = IDs::GEOMETRY_ROOM_INSTANCE;

	result = AK::SpatialAudio::SetRoom(IDs::ROOM, paramsRoom, "Inside");

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
	return result;

}

AKRESULT WwiseAPI::AddPortals(const GameObject& gameObject1, const GameObject& gameObject2) {
	AKRESULT result;

	AkPortalParams paramsPortal;

	GoVector3 position = gameObject1.GetPosition();
	paramsPortal.Transform.SetPosition(position.x, position.y, -position.z);

	GoVector3 forward = gameObject1.GetNormalizedForward();
	GoVector3 up = gameObject1.GetNormalizedUp();
	paramsPortal.Transform.SetOrientation({ forward.x, forward.y, -forward.z }, { up.x, up.y, up.z });
	paramsPortal.Extent.halfWidth = gameObject1.GetTransform().scale.x / 2.f;
	paramsPortal.Extent.halfHeight = gameObject1.GetTransform().scale.y / 2.f;
	paramsPortal.Extent.halfDepth = gameObject1.GetTransform().scale.z / 2.f;

	paramsPortal.bEnabled = true;
	paramsPortal.FrontRoom = AK::SpatialAudio::kOutdoorRoomID;
	paramsPortal.BackRoom = IDs::ROOM;

	result = AK::SpatialAudio::SetPortal(IDs::PORTAL0, paramsPortal, "Portal One");
	return AK_Success;
}

AKRESULT WwiseAPI::AddGeometry(const std::shared_ptr<GameObject>& gameObject) {
	AKRESULT result;
	AkGeometryParams geomWallsInside;
	geomWallsInside.NumVertices = gameObject->mesh.vertices.size();
	std::vector<AkVertex> vertices;
	for (size_t i = 0; i < geomWallsInside.NumVertices; i++)
	{
		AkVertex vertex;
		vertex.X = gameObject->mesh.vertices[i].x;
		vertex.Y = gameObject->mesh.vertices[i].y;
		vertex.Z = gameObject->mesh.vertices[i].z;
		vertices.emplace_back(vertex);
	}

	geomWallsInside.Vertices = vertices.data();

	geomWallsInside.NumSurfaces = 2;
	AkAcousticSurface surfaces[2];
	AkPlacementNew(&surfaces[0]) AkAcousticSurface();
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 1.0f;
	AkPlacementNew(&surfaces[1]) AkAcousticSurface();
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 1.0f;
	geomWallsInside.Surfaces = surfaces;
	geomWallsInside.NumTriangles = gameObject->triangles.size();

	std::vector<AkTriangle> akTriangles;

	for (size_t i = 0; i < geomWallsInside.NumTriangles; i++)
	{
		AkTriangle akTriangle;
		akTriangle.point0 = gameObject->triangles[i].point0;
		akTriangle.point1 = gameObject->triangles[i].point1;
		akTriangle.point2 = gameObject->triangles[i].point2;
		akTriangle.surface = 0;
		akTriangles.emplace_back(akTriangle);
	}

	geomWallsInside.Triangles = akTriangles.data();

	geomWallsInside.EnableDiffraction = true;
	geomWallsInside.EnableDiffractionOnBoundaryEdges = true;
	geomWallsInside.EnableTriangles = true;


	result = AK::SpatialAudio::SetGeometry(IDs::GEOMETRY_WALL_SIDES, geomWallsInside);

	AkGeometryParams geomRoofCeilingInside = geomWallsInside;
	geomRoofCeilingInside.EnableDiffractionOnBoundaryEdges = false;
	result = AK::SpatialAudio::SetGeometry(IDs::GEOMETRY_WALL_CEILINGFLOOR, geomRoofCeilingInside);


	GenerateWalls(gameObject, IDs::ROOM, IDs::GEOMETRY_WALL_SIDES, IDs::GEOMETRY_WALL_CEILINGFLOOR,
		IDs::GEOMETRY_WALL_INSTANCE_1,
		IDs::GEOMETRY_WALL_INSTANCE_2,
		IDs::GEOMETRY_WALL_INSTANCE_3,
		IDs::GEOMETRY_WALL_INSTANCE_4,
		IDs::GEOMETRY_WALL_INSTANCE_5,
		IDs::GEOMETRY_WALL_INSTANCE_6);

	GenerateWalls(gameObject, AK::SpatialAudio::kOutdoorRoomID, IDs::GEOMETRY_WALL_SIDES, IDs::GEOMETRY_WALL_CEILINGFLOOR,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_1,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_2,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_3,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_4,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_5,
		IDs::GEOMETRY_WALL_INSTANCE_OUTSIDE_6);


	return AK_Success;
}

void WwiseAPI::GenerateWalls(const std::shared_ptr<GameObject>& gameObject, const AkRoomID& roomID,
	const AkGeometrySetID& wallSidesGeometryID,
	const AkGeometrySetID& wallCeilingFloorGeometryID,
	const AkGeometryInstanceID& wallInstance1,
	const AkGeometryInstanceID& wallInstance2,
	const AkGeometryInstanceID& wallInstance3,
	const AkGeometryInstanceID& wallInstance4,
	const AkGeometryInstanceID& wallInstance5,
	const AkGeometryInstanceID& wallInstance6)
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
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z + 2.5f };
			frontVector = { 1,0,0 };
		}
		else if (i == 1)
		{
			geometryInstanceID = wallInstance2;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z - 2.5f };
			frontVector = { -1,0,0 };

		}
		else if (i == 2)
		{
			geometryInstanceID = wallInstance3;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->GetPosition().x + 2.5f, gameObject->GetPosition().y, gameObject->GetPosition().z };
			frontVector = { 0,0,1 };

		}
		else if (i == 3)
		{
			geometryInstanceID = wallInstance4;
			akGeometrySetID = wallSidesGeometryID;
			position = { gameObject->GetPosition().x - 2.5, gameObject->GetPosition().y, gameObject->GetPosition().z };
			frontVector = { 0,0,-1 };
		}
		else if (i == 4)
		{
			geometryInstanceID = wallInstance5;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y + 2.5f, gameObject->GetPosition().z };
			frontVector = { 1,0,0 };
			topVector = { 0,0,1 };

		}
		else if (i == 5)
		{
			geometryInstanceID = wallInstance6;
			akGeometrySetID = wallCeilingFloorGeometryID;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y - 2.5f, gameObject->GetPosition().z };
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

AKRESULT WwiseAPI::AddRoomGeometry(const std::shared_ptr<GameObject>& gameObject) {
	AKRESULT result;
	AkGeometryParams geom;
	geom.NumVertices = gameObject->mesh.vertices.size();
	std::vector<AkVertex> vertices;
	for (size_t i = 0; i < geom.NumVertices; i++)
	{
		AkVertex vertex;
		vertex.X = gameObject->mesh.vertices[i].x;
		vertex.Y = gameObject->mesh.vertices[i].y;
		vertex.Z = gameObject->mesh.vertices[i].z;
		vertices.emplace_back(vertex);
	}

	geom.Vertices = vertices.data();

	geom.NumSurfaces = 2;
	AkAcousticSurface surfaces[2];
	AkPlacementNew(&surfaces[0]) AkAcousticSurface();
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 0.5f;
	AkPlacementNew(&surfaces[1]) AkAcousticSurface();
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 0.5f;
	geom.Surfaces = surfaces;
	geom.NumTriangles = gameObject->triangles.size();

	std::vector<AkTriangle> akTriangles;

	for (size_t i = 0; i < geom.NumTriangles; i++)
	{
		AkTriangle akTriangle;
		akTriangle.point0 = gameObject->triangles[i].point0;
		akTriangle.point1 = gameObject->triangles[i].point1;
		akTriangle.point2 = gameObject->triangles[i].point2;
		akTriangle.surface = 0;
		akTriangles.emplace_back(akTriangle);
	}

	geom.Triangles = akTriangles.data();

	geom.EnableDiffraction = true;
	geom.EnableDiffractionOnBoundaryEdges = false;
	geom.EnableTriangles = true;

	result = AK::SpatialAudio::SetGeometry(IDs::GEOMETRY_ROOM, geom);

	AkGeometryInstanceParams instanceParams;

	instanceParams.GeometrySetID = IDs::GEOMETRY_ROOM;
	instanceParams.RoomID = IDs::ROOM;

	result = AK::SpatialAudio::SetGeometryInstance(IDs::GEOMETRY_ROOM_INSTANCE, instanceParams);

	return AK_Success;
}

void WwiseAPI::EventCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	WwiseAPI* wwiseAPI = (WwiseAPI*)in_pCallbackInfo->pCookie;
	switch (in_eType)
	{
	case AK_MusicSyncBeat:
	wwiseAPI->callbackFunctionBeat();
		break;
	case AK_MusicSyncBar:
	wwiseAPI->callbackFunctionBar();
		break;
	default:
		break;
	}
}

void WwiseAPI::SetCallbackFunctionBeat(std::function<void()> function) {
	callbackFunctionBeat = function;
}

void WwiseAPI::SetCallbackFunctionBar(std::function<void()> function)
{
	callbackFunctionBar = function;
}
