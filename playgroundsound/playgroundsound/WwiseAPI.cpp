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


const AkRoomID ROOM = 200;
const AkPortalID PORTAL0 = 300;
const AkPortalID PORTAL1 = 301;
const AkGeometrySetID GEOMETRY_ROOM = 400;
const AkGeometrySetID GEOMETRY_ROOM_INSTANCE = 401;

const AkGameObjectID musicObjectID = 1;
const AkGameObjectID listenerObjectID = 2;
const AkGameObjectID distanceProbeObjectID = 3;


const AkGeometrySetID GEOMETRY_WALL_1 = 1000;
const AkGeometrySetID GEOMETRY_WALL_2 = 1001;
const AkGeometrySetID GEOMETRY_WALL_3 = 1002;
const AkGeometrySetID GEOMETRY_WALL_4 = 1003;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_1 = 2000;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_2 = 2001;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_3 = 2002;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_4 = 2003;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_5 = 2004;
const AkGeometrySetID GEOMETRY_WALL_INSTANCE_6 = 2005;

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
	if (numberOfPaths > 0)
	{
		int b = 1;
	}
	if (numberOfPaths > 1)
	{
		int a = 1;
	}
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
		diffractionPaths.push_back(diffractionPath);
	}
	return diffractionPaths;

}


void WwiseAPI::Log(std::string_view logMsg) {
	std::cout << logMsg << std::endl;
}

AKRESULT WwiseAPI::RegisterGameObject(const AkGameObjectID& gameObjectID, std::string_view gameObjectName)
{
	akGameObjects.push_back(gameObjectID);
	return AK::SoundEngine::RegisterGameObj(gameObjectID, gameObjectName.data());
}

AKRESULT WwiseAPI::SetPlayerIsInRoom(const bool& isInRoom)
{
	AkRoomID currentRoom = isInRoom ? ROOM : AK::SpatialAudio::kOutdoorRoomID;
	return AK::SpatialAudio::SetGameObjectInRoom(listenerObjectID, currentRoom);
}

AKRESULT WwiseAPI::AddListener() {

	RegisterGameObject(musicObjectID, "Music");
	RegisterGameObject(listenerObjectID, "Listener");
	RegisterGameObject(distanceProbeObjectID, "DistanceProbe");

	static const int kNumLstnrsForEm = 1;
	static const AkGameObjectID aLstnrsForEmitter[kNumLstnrsForEm] = { listenerObjectID };
	//AK::SoundEngine::AddDefaultListener(listenerObjectID);
	AK::SoundEngine::SetListeners(musicObjectID, aLstnrsForEmitter, kNumLstnrsForEm);
	//AK::SoundEngine::SetListeners(listenerObjectID, aLstnrsForEmitter, kNumLstnrsForEm);
	//AK::SoundEngine::SetListeners(ROOM, aLstnrsForEmitter, kNumLstnrsForEm);
	//AK::SoundEngine::SetListeners(AK::SpatialAudio::kOutdoorRoomID, aLstnrsForEmitter, kNumLstnrsForEm);
	//AK::SoundEngine::SetListeners(distanceProbeObjectID, aLstnrsForEmitter, kNumLstnrsForEm);

	//AK::SpatialAudio::SetGameObjectInRoom(listenerObjectID, AK::SpatialAudio::kOutdoorRoomID);
	AK::SoundEngine::SetDistanceProbe(listenerObjectID, distanceProbeObjectID);
	AK::SpatialAudio::RegisterListener(listenerObjectID);
	return AK_Success;
}

AkPlayingID WwiseAPI::PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID)
{
	//AK::SpatialAudio::SetGameObjectInRoom(gameObjectID, ROOM);
	return AK::SoundEngine::PostEvent(eventID, gameObjectID);
}

AKRESULT WwiseAPI::UpdateListenerGO(const GameObject& listenerGameObject)
{
	return UpdateGameObject(listenerObjectID, listenerGameObject);
}

AKRESULT WwiseAPI::UpdateDistanceProbeGO(const GameObject& distanceProbeGameObject)
{
	return UpdateGameObject(distanceProbeObjectID, distanceProbeGameObject);
}

AKRESULT WwiseAPI::UpdateEmitterGO(const GameObject& emitterGameObject)
{
	return UpdateGameObject(musicObjectID, emitterGameObject);
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

	if (result != AK_Success)
	{
		Log("Failed to set position. Forward: {" + std::to_string(forwardNormalized.x) + ", " + std::to_string(forwardNormalized.y) + ", " + std::to_string(forwardNormalized.z) + " }");
	}
	return result;
}


AKRESULT WwiseAPI::AddRoom() {
	AKRESULT result;

	AkRoomParams paramsRoom;
	// Let's orient our rooms towards the top of the screen. 
	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;
	paramsRoom.TransmissionLoss = 0.9f;	// Let's have a bit of sound transmitted through walls when all portals are closed.
	paramsRoom.RoomGameObj_KeepRegistered = true;	// We intend to use the room's game object to post events (see documentation of AkRoomParams::RoomGameObj_KeepRegistered).
	paramsRoom.RoomGameObj_AuxSendLevelToSelf = 0.25f;	// Since we will be playing an ambience ("Play_Ambience_Quad", below), on this room's game object, we here route some of it to the room's auxiliary bus to add some of its reverb.
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Inside");
	paramsRoom.GeometryInstanceID = GEOMETRY_ROOM;	// We associate the geometry to the room in order to compute the room spread. 
	// If the geometry is not found (in "Portals Demo"), the room bounding box is calculated from the portals combined extent.

	result = AK::SpatialAudio::SetRoom(ROOM, paramsRoom, "Inside");

	paramsRoom.Front.X = 0.f;
	paramsRoom.Front.Y = 0.f;
	paramsRoom.Front.Z = 1.f;
	paramsRoom.Up.X = 0.f;
	paramsRoom.Up.Y = 1.f;
	paramsRoom.Up.Z = 0.f;

	paramsRoom.TransmissionLoss = 0.9f;
	paramsRoom.RoomGameObj_KeepRegistered = false;
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Outside");
	paramsRoom.GeometryInstanceID = AkGeometryInstanceID(); // Invalid ID - no geometry for outside.
	result = AK::SpatialAudio::SetRoom(AK::SpatialAudio::kOutdoorRoomID, paramsRoom, "Outside");
	return result;

}

AKRESULT WwiseAPI::AddPortals(const GameObject& gameObject1, const GameObject& gameObject2) {
	AKRESULT result;
	//
	// Register portals to SpatialAudio.
	//

	AkPortalParams paramsPortal;

	GoVector3 position = gameObject1.GetPosition();
	paramsPortal.Transform.SetPosition(position.x, position.y, -position.z);

	GoVector3 forward = gameObject1.GetNormalizedForward();
	GoVector3 up = gameObject1.GetNormalizedUp();
	paramsPortal.Transform.SetOrientation({ forward.x, forward.y, -forward.z }, { up.x, up.y, up.z });
	// Portal extent. Defines the dimensions of the portal relative to its center; all components must be positive numbers. The local X and Y dimensions (side and top) are used in diffraction calculations, 
	// whereas the Z dimension (front) defines a depth value which is used to implement smooth transitions between rooms. It is recommended that users experiment with different portal depths to find a value 
	// that results in appropriately smooth transitions between rooms.
	// Important: divide height and width by 2, because Extent expresses dimensions relative to the center (like a radius).
	paramsPortal.Extent.halfWidth = gameObject1.GetTransform().scale.x / 2.f;
	paramsPortal.Extent.halfHeight = gameObject1.GetTransform().scale.y / 2.f;
	paramsPortal.Extent.halfDepth = gameObject1.GetTransform().scale.z / 2.f;

	// Whether or not the portal is active/enabled. For example, this parameter may be used to simulate open/closed doors.
	paramsPortal.bEnabled = true;	// Open if bit 0 of our portal open state m_portalsOpen is set.
	// ID of the room that the portal connects to, in the direction of the Front vector.
	paramsPortal.FrontRoom = AK::SpatialAudio::kOutdoorRoomID;
	// ID of room that that portal connects, in the direction opposite to the Front vector. 
	paramsPortal.BackRoom = ROOM;

	result = AK::SpatialAudio::SetPortal(PORTAL0, paramsPortal, "Portal One");


	//
	// Portal 1 (Outside->ROOM, like a wide vertical tube, bottom-left).
	//

	//GoVector3 position2 = gameObject2.GetPosition();
	//paramsPortal.Transform.SetPosition(position2.x, position2.y, position2.z);
	//// Points up towards ROOM.
	//// Up vector: This is a 2D game with Y pointing towards the player, and so should the local Y.
	//GoVector3 forward2 = gameObject2.GetForward();
	//GoVector3 up2 = gameObject2.GetUp();
	//paramsPortal.Transform.SetOrientation({ -forward2.x, -forward2.y, forward2.z }, { up2.x, up2.y, -up2.z });
	//// Portal extent. Defines the dimensions of the portal relative to its center; all components must be positive numbers. The local X and Y dimensions (side and top) are used in diffraction calculations, 
	//// whereas the Z dimension (front) defines a depth value which is used to implement smooth transitions between rooms. It is recommended that users experiment with different portal depths to find a value 
	//// that results in appropriately smooth transitions between rooms.
	//// Important: divide width and height by 2, because Extent expresses dimensions relative to the center (like a radius).
	//paramsPortal.Extent.halfWidth = gameObject2.GetTransform().scale.x / 2.f;
	//paramsPortal.Extent.halfHeight = gameObject2.GetTransform().scale.y / 2.f;
	//paramsPortal.Extent.halfDepth = gameObject2.GetTransform().scale.z / 2.f;
	///// Whether or not the portal is active/enabled. For example, this parameter may be used to simulate open/closed doors.
	//paramsPortal.bEnabled = true;	// Open if bit 1 of our portal open state m_portalsOpen is set.
	//// ID of the room that the portal connects to, in the direction of the Front vector.
	//paramsPortal.FrontRoom = ROOM;
	//// ID of room that that portal connects, in the direction opposite to the Front vector. 
	//paramsPortal.BackRoom = AK::SpatialAudio::kOutdoorRoomID;

	////result = AK::SpatialAudio::SetPortal(PORTAL1, paramsPortal, "Outside->ROOM, vertical");
	return AK_Success;
}

AKRESULT WwiseAPI::AddGeometry(const std::shared_ptr<GameObject>& gameObject, int wallIndex) {
	AKRESULT result;
	/*for (const auto& gameObject : gameObjects)
	{*/
	AkGeometryParams geom;
	geom.NumVertices = gameObject->mesh.vertices.size();
	std::vector<AkVertex> vertices;
	for (size_t i = 0; i < geom.NumVertices; i++)
	{
		AkVertex vertex;
		vertex.X = gameObject->mesh.vertices[i].x;
		vertex.Y = gameObject->mesh.vertices[i].y;
		vertex.Z = gameObject->mesh.vertices[i].z;
		vertices.push_back(vertex);
	}

	geom.Vertices = vertices.data();

	geom.NumSurfaces = 2;
	AkAcousticSurface surfaces[2];
	AkPlacementNew(&surfaces[0]) AkAcousticSurface();
	surfaces[0].strName = "Outside";
	surfaces[0].textureID = AK::SoundEngine::GetIDFromString("Brick");
	surfaces[0].transmissionLoss = 0.0f;
	AkPlacementNew(&surfaces[1]) AkAcousticSurface();
	surfaces[1].strName = "Inside";
	surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
	surfaces[1].transmissionLoss = 0.0f;
	geom.Surfaces = surfaces;
	geom.NumTriangles = gameObject->triangles.size();

	std::vector<AkTriangle> akTriangles;

	for (size_t i = 0; i < geom.NumTriangles; i++)
	{
		AkTriangle akTriangle;
		akTriangle.point0 = gameObject->triangles[i].point0;
		akTriangle.point1 = gameObject->triangles[i].point1;
		akTriangle.point2 = gameObject->triangles[i].point2;
		akTriangles.push_back(akTriangle);
	}

	geom.Triangles = akTriangles.data();

	geom.EnableDiffraction = true;
	geom.EnableDiffractionOnBoundaryEdges = true;
	geom.EnableTriangles = true;


	AkGeometrySetID geometryID;
	AkGeometrySetID geometryInstanceID;
	
	result = AK::SpatialAudio::SetGeometry(GEOMETRY_WALL_1, geom);
	//}

	for (size_t i = 0; i < 6; i++)
	{
		AkGeometryInstanceParams instanceParams;
		instanceParams.Scale = { 1.01,1.01,1.01 };
		AkWorldTransform worldTransform;
		AkVector topVector = { 0,1,0 };
		AkVector frontVector = { 0,0,1 };
		AkVector64 position; 
		
		if (i == 0)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_1;
			geometryID = GEOMETRY_WALL_1;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z + 5 };
			frontVector = { 1,0,0 };
		}
		else if (i == 1)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_2;
			geometryID = GEOMETRY_WALL_2;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y, gameObject->GetPosition().z - 5 };
			frontVector = { -1,0,0 };

		}
		else if (i == 2)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_3;
			geometryID = GEOMETRY_WALL_3;
			position = { gameObject->GetPosition().x + 5, gameObject->GetPosition().y, gameObject->GetPosition().z };
			frontVector = { 0,0,1 };

		}
		/*else if (i == 3)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_4;
			geometryID = GEOMETRY_WALL_4;
			position = { gameObject->GetPosition().x - 5, gameObject->GetPosition().y, gameObject->GetPosition().z };
			frontVector = { 0,0,-1 };

		}*/
		else if (i == 4)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_5;
			geometryID = GEOMETRY_WALL_4;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y + 5, gameObject->GetPosition().z };
			frontVector = { 1,0,0 };
			topVector = { 0,0,1 };

		}
		else if (i == 5)
		{
			geometryInstanceID = GEOMETRY_WALL_INSTANCE_6;
			geometryID = GEOMETRY_WALL_4;
			position = { gameObject->GetPosition().x, gameObject->GetPosition().y - 5, gameObject->GetPosition().z };

			frontVector = { -1,0,0 };
			topVector = { 0,0,1 };

		}

		worldTransform.Set(position, frontVector, topVector);
		instanceParams.PositionAndOrientation = worldTransform;
		instanceParams.GeometrySetID = GEOMETRY_WALL_1;
		//instanceParams.RoomID = ROOM;

		result = AK::SpatialAudio::SetGeometryInstance(geometryInstanceID, instanceParams);

	}

	return AK_Success;
}