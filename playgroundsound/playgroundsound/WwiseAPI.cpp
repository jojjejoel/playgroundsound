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



bool WwiseAPI::Init()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) == AK_Success)
	{
		std::cout << "Memory Manager initialized" << std::endl;
	}
	else
	{
		std::cout << "Memory Manager failed to initialize" << std::endl;
		return false;
	}

	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	if (!AK::StreamMgr::Create(streamSettings))
	{
		std::cout << "Could not create the Streaming Manager" << std::endl;
	}
	else
	{
		std::cout << "Streaming Manager created!" << std::endl;
	}


	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) == AK_Success)
	{
		std::cout << "Created the streaming device and low level I/O system!" << std::endl;
	}
	else
	{
		std::cout << "Could not create streaming device and low level I/O system!" << std::endl;
		return false;
	}


	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) == AK_Success)
	{
		std::cout << "Sound engine initialized!" << std::endl;
	}
	else
	{
		std::cout << "Could not initialize sound engine!" << std::endl;
		return false;
	}


	g_lowLevelIO.SetBasePath(AKTEXT("C:\\Users\\Joel.Schultz\\Documents\\GitHub\\playgroundsound\\playgroundsound\\Wwise\\GeneratedSoundBanks\\Windows"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

#ifndef AK_OPTIMIZED

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		Log("Communication Init failed.");
	}

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

void WwiseAPI::Log(const std::string& logMsg) {
	std::cout << logMsg << std::endl;
}

AKRESULT WwiseAPI::RegisterGameObject(const AkGameObjectID& gameObjectID, const char* gameObjectName)
{
	return AK::SoundEngine::RegisterGameObj(gameObjectID, gameObjectName);
}

AKRESULT WwiseAPI::AddListener(const AkGameObjectID& gameObjectID, const char* gameObjectName) {
	RegisterGameObject(gameObjectID, gameObjectName);
	AK::SoundEngine::AddDefaultListener(gameObjectID);
	return AK_Success;
}

AkPlayingID WwiseAPI::PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID, const char* gameObjectName)
{
	RegisterGameObject(gameObjectID, gameObjectName);
	return AK::SoundEngine::PostEvent(eventID, gameObjectID);
}

AKRESULT WwiseAPI::UpdateGameObject(const AkGameObjectID& akGameObjectID, const GameObject& gameObject)
{
	AkSoundPosition soundPosition;
	AkVector64 positionVector = { gameObject.position.x, gameObject.position.y ,gameObject.position.z };
	GoVector3 forwardNormalized = gameObject.GetNormalizedForward();
	GoVector3 upNormalized = gameObject.GetNormalizedUp();
	AkVector orientationFront = { forwardNormalized.x, forwardNormalized.y, forwardNormalized.z};
	AkVector orientationTop = { upNormalized.x, upNormalized.y, upNormalized.z };
	/*AkVector orientationFront = { 1,0,0 };
	AkVector orientationTop = { 0,1,0 };*/
	soundPosition.Set(positionVector, orientationFront, orientationTop);

	AKRESULT result = AK::SoundEngine::SetPosition(akGameObjectID, soundPosition);
	if (result == AK_Success)
	{
		Log("Position set. Forward: {" + std::to_string(forwardNormalized.x) + ", " + std::to_string(forwardNormalized.y) + ", " + std::to_string(forwardNormalized.z) + " }");
	}
	else
	{
		Log("Failed to set position. Forward: {" + std::to_string(forwardNormalized.x) + ", " + std::to_string(forwardNormalized.y) + ", " + std::to_string(forwardNormalized.z) + " }");
	}
	return result;
}
