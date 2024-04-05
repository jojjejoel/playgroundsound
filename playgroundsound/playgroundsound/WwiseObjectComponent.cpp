#include "WwiseObjectComponent.h"
#include <AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "AkFilePackage.h"
#include "AkFilePackageLUT.h"
#include "AK/Comm/AkCommunication.h"
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include <AK/MusicEngine/Common/AkMusicEngine.h>

void WwiseObjectComponent::Init(GameObject* in_gameObject)
{
	akGameObjectID = in_gameObject->m_id;
	gameObjectName = in_gameObject->m_name.c_str();
	AK::SoundEngine::RegisterGameObj(akGameObjectID, gameObjectName);
}

void WwiseObjectComponent::Update(GameObject* in_gameObject)
{
	AkListenerPosition soundPosition;
	AkVector positionVector = { in_gameObject->m_transform.position.x, in_gameObject->m_transform.position.y ,-in_gameObject->m_transform.position.z };
	GoVector3 forwardNormalized = in_gameObject->m_transform.forward.Normalized();
	GoVector3 upNormalized = in_gameObject->m_transform.up.Normalized();
	AkVector orientationFront = { -forwardNormalized.x, -forwardNormalized.y, forwardNormalized.z };
	AkVector orientationTop = { upNormalized.x, upNormalized.y, -upNormalized.z };
	AkTransform transform;
	transform.Set(positionVector, orientationFront, orientationTop);
	soundPosition.Set(positionVector, orientationFront, orientationTop);

	/*if (gameObject.GetRoomID() != 0)
	{
		SetGameObjectIsInRoom(akGameObjectID, gameObject.GetRoomID());
	}
	else
	{
		SetGameObjectIsInRoom(akGameObjectID, AK::SpatialAudio::kOutdoorRoomID);
	}*/

	AKRESULT result = AK::SoundEngine::SetPosition(akGameObjectID, soundPosition);
}

void WwiseObjectComponent::PostEvent(const AkUniqueID& eventID)
{
	AK::SoundEngine::PostEvent(eventID, akGameObjectID);
}

void WwiseObjectComponent::RegisterAsListener()
{
	AK::SoundEngine::AddDefaultListener(akGameObjectID);
	AK::SpatialAudio::RegisterListener(akGameObjectID);
}

void WwiseObjectComponent::RegisterAsDistanceProbe(const AkGameObjectID& listenerID)
{
	AK::SoundEngine::SetDistanceProbe(static_cast<AkGameObjectID>(listenerID), static_cast<AkGameObjectID>(akGameObjectID));
}

void WwiseObjectComponent::SetRTPC(const AkRtpcID& rtpcID, const AkRtpcValue& rtpcValue)
{
	AKRESULT result = AK::SoundEngine::SetRTPCValue(rtpcID, rtpcValue, akGameObjectID);
}
