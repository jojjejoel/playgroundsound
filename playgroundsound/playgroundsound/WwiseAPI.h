#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AkFilePackageLowLevelIOBlocking.h>

#include "GameObject.h"

#include <string>

class WwiseAPI
{
public:
	bool Init();
	void DeInit();
	AKRESULT LoadBank(const AkUniqueID& bankID);
	void RenderAudio();
	AKRESULT AddListener(const AkGameObjectID& listenerID, const std::string& listenerGameObjectName, const AkGameObjectID& distanceProbeID);
	AkPlayingID PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID, const char* gameObjectName);
	AKRESULT UpdateGameObject(const AkGameObjectID& gameObjectID, const GameObject& gameObject);


private:
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(const std::string& logMsg);
	AKRESULT RegisterGameObject(const AkGameObjectID& gameObjectID, const std::string& gameObjectName);
};

