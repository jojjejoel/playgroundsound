#include "WwiseObjectComponent.h"
#include <AK/SoundEngine/Common/AkQueryParameters.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "AK/Comm/AkCommunication.h"
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "..\GameObject\GameObject.h"
#include <AK/SoundEngine/Common/AkCallback.h>

WwiseObjectComponent::~WwiseObjectComponent()
{
	AK::SoundEngine::StopAll(akGameObjectID);
}

void WwiseObjectComponent::Init(GameObject* in_gameObject)
{
	akGameObjectID = in_gameObject->m_id;
	gameObjectName = in_gameObject->m_name.c_str();
	currentRoomID = static_cast<unsigned int>(AK::SpatialAudio::kOutdoorRoomID);
	AK::SoundEngine::RegisterGameObj(akGameObjectID, gameObjectName);
}

void WwiseObjectComponent::Update(GameObject* in_gameObject)
{
	m_position = in_gameObject->m_transform.position;
	AkListenerPosition soundPosition;
	AkVector positionVector = { in_gameObject->m_transform.position.x, in_gameObject->m_transform.position.y ,-in_gameObject->m_transform.position.z };
	GO_Vector3 forwardNormalized = in_gameObject->m_transform.forward.Normalized();
	GO_Vector3 upNormalized = in_gameObject->m_transform.up.Normalized();
	AkVector orientationFront = { -forwardNormalized.x, -forwardNormalized.y, forwardNormalized.z };
	AkVector orientationTop = { upNormalized.x, upNormalized.y, -upNormalized.z };
	AkTransform transform;
	transform.Set(positionVector, orientationFront, orientationTop);
	soundPosition.Set(positionVector, orientationFront, orientationTop);

	AKRESULT result = AK::SoundEngine::SetPosition(akGameObjectID, soundPosition);
}

void WwiseObjectComponent::PostEvent(const unsigned int& eventID)
{
	AK::SoundEngine::PostEvent(eventID, akGameObjectID);
}


static void MusicCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo)
{
	if (in_eType & AK_MusicSyncAll)
	{
		WwiseObjectComponent* wwiseObjectComponent = (WwiseObjectComponent*)in_pCallbackInfo->pCookie;
		AkMusicSyncCallbackInfo* akMusicSyncCallbackInfo = static_cast<AkMusicSyncCallbackInfo*>(in_pCallbackInfo);
		float barDuration = akMusicSyncCallbackInfo->segmentInfo.fBarDuration;
		switch (in_eType)
		{
			case AK_MusicSyncBeat:
				wwiseObjectComponent->callbackFuntionBeat();
				break;
			case AK_MusicSyncBar:
				wwiseObjectComponent->callbackFuntionBar(barDuration);
				break;
			default:
				break;
		}
	}
}

void WwiseObjectComponent::PostMusicEvent(const unsigned int& eventID, std::function<void(float)> in_callbackFuncBar, std::function<void()> in_callbackFuncBeat)
{
	callbackFuntionBar = in_callbackFuncBar;
	callbackFuntionBeat = in_callbackFuncBeat;
	AK::SoundEngine::PostEvent(eventID, akGameObjectID, AK_MusicSyncAll, &MusicCallback, this);
}

void WwiseObjectComponent::RegisterAsListener()
{
	AK::SoundEngine::AddDefaultListener(akGameObjectID);
	AK::SpatialAudio::RegisterListener(akGameObjectID);
}

void WwiseObjectComponent::RegisterAsDistanceProbe(const unsigned int& listenerID)
{
	AK::SoundEngine::SetDistanceProbe(listenerID, akGameObjectID);
}

void WwiseObjectComponent::SetRTPC(const unsigned int& rtpcID, const float& rtpcValue)
{
	AKRESULT result = AK::SoundEngine::SetRTPCValue(rtpcID, rtpcValue, akGameObjectID);
}

void WwiseObjectComponent::SetRoomID(const unsigned int& in_akRoomID)
{
	if (currentRoomID != in_akRoomID)
	{
		currentRoomID = in_akRoomID;
		AK::SpatialAudio::SetGameObjectInRoom(akGameObjectID, currentRoomID);
	}
}
const GO_Vector3& WwiseObjectComponent::GetPosition()
{
	return m_position;
}

float WwiseObjectComponent::GetGameParamValueGlobal(const unsigned int& rtpcID)
{
	AkRtpcValue rtpcValue;
	AK::SoundEngine::Query::RTPCValue_type valueType = AK::SoundEngine::Query::RTPCValue_Global;
	AKRESULT result = AK::SoundEngine::Query::GetRTPCValue(rtpcID, akGameObjectID, AK_INVALID_PLAYING_ID, rtpcValue, valueType);

	float minRTPCValue = -20;
	float maxRTPCValue = -4;

	float normalizedValue = (rtpcValue - (minRTPCValue)) / (maxRTPCValue - (minRTPCValue));

	return normalizedValue;
}
