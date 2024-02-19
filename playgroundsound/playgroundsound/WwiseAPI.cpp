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

void WwiseAPI::Log(std::string_view logMsg) {
	std::cout << logMsg << std::endl;
}

AKRESULT WwiseAPI::RegisterGameObject(const AkGameObjectID& gameObjectID, std::string_view gameObjectName)
{
	return AK::SoundEngine::RegisterGameObj(gameObjectID, gameObjectName.data());
}

AKRESULT WwiseAPI::AddListener(const AkGameObjectID& listenerID, std::string_view listenerName, const AkGameObjectID& distanceProbeID, std::string_view distanceProbeName) {
	RegisterGameObject(listenerID, listenerName);
	RegisterGameObject(distanceProbeID, distanceProbeName);
	AK::SoundEngine::AddDefaultListener(listenerID);
	//AK::SoundEngine::liste
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
	AkVector positionVector = { gameObject.position.x, gameObject.position.y ,-gameObject.position.z };
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

AKRESULT WwiseAPI::SetStaticObject(const GameObject& gameObject) {
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
