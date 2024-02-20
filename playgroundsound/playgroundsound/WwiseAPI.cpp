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

	for (AkUInt32 path = 0; path < numberOfPaths; path++)
	{
		DiffractionPath diffractionPath;
		AkDiffractionPathInfo akDiffractionPath = akDiffractionPaths[path];
		for (size_t i = 0; i < akDiffractionPath.kMaxNodes; i++)
		{
			diffractionPath.nodes[i].x = akDiffractionPath.nodes[i].X;
			diffractionPath.nodes[i].y = akDiffractionPath.nodes[i].Y;
			diffractionPath.nodes[i].z = -akDiffractionPath.nodes[i].Z;
		}
		diffractionPath.emitterPos.x = akDiffractionPath.emitterPos.X;
		diffractionPath.emitterPos.y = akDiffractionPath.emitterPos.Y;
		diffractionPath.emitterPos.z = akDiffractionPath.emitterPos.Z;
		for (size_t i = 0; i < akDiffractionPath.kMaxNodes; i++)
		{
			diffractionPath.angles[i] = akDiffractionPath.angles[i];
		}
		for (size_t i = 0; i < akDiffractionPath.kMaxNodes; i++)
		{

		diffractionPath.portals[i] = akDiffractionPath.portals[i];
		}
		for (size_t i = 0; i < akDiffractionPath.kMaxNodes; i++)
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

AKRESULT WwiseAPI::AddListener(const AkGameObjectID& listenerID, const AkGameObjectID& emitterID, std::string_view listenerName, const AkGameObjectID& distanceProbeID, std::string_view distanceProbeName) {
	RegisterGameObject(listenerID, listenerName);
	RegisterGameObject(distanceProbeID, distanceProbeName);

	static const int kNumLstnrsForEm = 1;
	static const AkGameObjectID aLstnrsForEmitter[kNumLstnrsForEm] = { listenerID };
	AK::SoundEngine::SetListeners(emitterID, aLstnrsForEmitter, kNumLstnrsForEm);
	AK::SoundEngine::SetListeners(listenerID, aLstnrsForEmitter, kNumLstnrsForEm);


	AK::SoundEngine::SetDistanceProbe(listenerID, distanceProbeID);
	AK::SpatialAudio::RegisterListener(listenerID);
	return AK_Success;
}

AkPlayingID WwiseAPI::PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID, std::string_view gameObjectName)
{
	RegisterGameObject(gameObjectID, gameObjectName);
	return AK::SoundEngine::PostEvent(eventID, gameObjectID);
}

AKRESULT WwiseAPI::UpdateGameObject(const AkGameObjectID& akGameObjectID, const GameObject& gameObject)
{
	AkListenerPosition soundPosition;
	AkVector positionVector = { gameObject.GetPosition().x, gameObject.GetPosition().y ,-gameObject.GetPosition().z};
	GoVector3 forwardNormalized = gameObject.GetNormalizedForward();
	GoVector3 upNormalized = gameObject.GetNormalizedUp();
	AkVector orientationFront = { forwardNormalized.x, forwardNormalized.y, forwardNormalized.z};
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

AKRESULT WwiseAPI::AddRoom(const GameObject& gameObject) {
	AkGeometrySetID geometryID = 100;
	AkRoomParams paramsRoom;
	AkRoomID roomID = 200;
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
	paramsRoom.ReverbAuxBus = AK::SoundEngine::GetIDFromString("Room");
	paramsRoom.GeometryInstanceID = geometryID;

	AK::SpatialAudio::SetRoom(roomID, paramsRoom, "Room Object");
	return AK_Success;
}

AKRESULT WwiseAPI::AddGeometry(const std::vector<std::shared_ptr<GameObject>>& gameObjects) {

	int idIncrease = 0;
	for (const auto& gameObject : gameObjects)
	{
		AkGeometrySetID geometrySetID = 100 + idIncrease;
		AkGeometrySetID geometrySetInstanceID = 200 + idIncrease;
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
		surfaces[0].transmissionLoss = 0.9f;
		AkPlacementNew(&surfaces[1]) AkAcousticSurface();
		surfaces[1].strName = "Inside";
		surfaces[1].textureID = AK::SoundEngine::GetIDFromString("Drywall");
		surfaces[1].transmissionLoss = 0.9f;
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

		AK::SpatialAudio::SetGeometry(geometrySetID, geom);
		AkGeometryInstanceParams instanceParams;
		instanceParams.GeometrySetID = geometrySetID;
		AK::SpatialAudio::SetGeometryInstance(geometrySetInstanceID, instanceParams);
		idIncrease++;
	}
	
	return AK_Success;
}