#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

#include "GameObject.h"
#include "DiffractionPath.h"

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <map>

class WwiseAPI
{
public:
	bool Init();
	void DeInit();
	AKRESULT LoadBank(const AkUniqueID& bankID);
	float GetRTPCValueGameObject(const AkRtpcID& rtpcID, const AkGameObjectID& akGameObjectID);
	float GetRTPCValue(const AkRtpcID& rtpcID, const AkGameObjectID& akGameObjectID, const AK::SoundEngine::Query::RTPCValue_type& valueType, const AkPlayingID& playingID, const float& minValue, const float& maxValue);
	void SetRTPCValueGlobal(const AkRtpcID& rtpcID, const AkRtpcValue& rtpcValue);
	void SetRTPCValueGameObject(const AkRtpcID& rtpcID, const AkRtpcValue& rtpcValue, const AkGameObjectID& akGameObjectID);
	void RenderAudio();
	std::vector<DiffractionPath> GetDiffraction(const AkGameObjectID& gameObjectID);
	AKRESULT AddListener();
	AkPlayingID PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID);

	AKRESULT UpdateAllGameObjects();

	AKRESULT AddGeometry(const std::shared_ptr<GameObject>& gameObject);


	AKRESULT AddRoomGeometry(const std::shared_ptr<GameObject>& gameObject);

	static void EventCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

	void SetPlaybackSpeed(const float& playbackSpeed);

	void SetCallbackFunctionBeat(std::function<void()> function);
	void SetCallbackFunctionBar(std::function<void()> function);

	AKRESULT AddRoom();

	AKRESULT AddPortal(const GameObject& gameObject);

	AKRESULT SetGameObjectIsInRoom(const AkGameObjectID& gameObjectID, const unsigned int& roomID);
	AKRESULT RegisterGameObject(const GameObject& gameObject);

	AKRESULT UpdateGameObject(const GameObject& gameObject);
private:
	void SetRTPCValue(const AkRtpcID& rtpcID, const AkRtpcValue& rtpcValue, const AkGameObjectID& akGameObjectID);
	void GenerateWalls(const std::shared_ptr<GameObject>& gameObject, const AkRoomID& roomID,
		const AkGeometrySetID& wallSidesGeometryID,
		const AkGeometrySetID& wallCeilingFloorGeometryID,
		const AkGeometryInstanceID& wallInstance1,
		const AkGeometryInstanceID& wallInstance2,
		const AkGeometryInstanceID& wallInstance3,
		const AkGeometryInstanceID& wallInstance4,
		const AkGeometryInstanceID& wallInstance5,
		const AkGeometryInstanceID& wallInstance6);
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(std::string_view logMsg);

	std::function<void()> callbackFunctionBeat;
	std::function<void()> callbackFunctionBar;

	std::map<unsigned int, bool> registeredObjects;

};

