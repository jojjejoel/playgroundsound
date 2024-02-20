#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AkFilePackageLowLevelIOBlocking.h>

#include "AK/SpatialAudio/Common/AkSpatialAudioTypes.h"
#include "GameObject.h"

#include "DiffractionPath.h"
#include <string>
#include <memory>

class WwiseAPI
{
public:
	bool Init();
	void DeInit();
	AKRESULT LoadBank(const AkUniqueID& bankID);
	void RenderAudio();
	std::vector<DiffractionPath> GetDiffraction(const AkGameObjectID& gameObjectID);
	AKRESULT AddListener(const AkGameObjectID& listenerID, const AkGameObjectID& emitterID, std::string_view listenerGameObjectName, const AkGameObjectID& distanceProbeID, std::string_view distanceProbeName);
	AkPlayingID PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID, std::string_view gameObjectName);
	AKRESULT UpdateGameObject(const AkGameObjectID& gameObjectID, const GameObject& gameObject);

	AKRESULT AddGeometry(const std::vector<std::shared_ptr<GameObject>>& gameObjects);

	AKRESULT AddRoom(const GameObject& gameObject);


private:
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(std::string_view logMsg);
	AKRESULT RegisterGameObject(const AkGameObjectID& gameObjectID, std::string_view gameObjectName);

	std::vector<AkGameObjectID> akGameObjects;
};

