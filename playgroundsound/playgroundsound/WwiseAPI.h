#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <AkFilePackageLowLevelIOBlocking.h>

#include "AK/SpatialAudio/Common/AkSpatialAudioTypes.h"
#include "GameObject.h"

#include "DiffractionPath.h"
#include <string>
#include <memory>
#include <vector>

class WwiseAPI
{
public:
	bool Init();
	void DeInit();
	AKRESULT LoadBank(const AkUniqueID& bankID);
	const float GetRTPCGlobal(const AkUniqueID& rtpcID);
	void RenderAudio();
	std::vector<DiffractionPath> GetDiffraction(const AkGameObjectID& gameObjectID);
	AKRESULT AddListener();
	AkPlayingID PostEvent(const AkUniqueID& eventID, const AkGameObjectID& gameObjectID);
	AKRESULT UpdateListenerGO(const GameObject& listenerGameObject);
	AKRESULT UpdateDistanceProbeGO(const GameObject& distanceProbeGameObject);

	AKRESULT UpdateEmitterGO(const GameObject& emitterGameObject);

	AKRESULT AddGeometry(const std::shared_ptr<GameObject>& gameObject);


	AKRESULT AddRoomGeometry(const std::shared_ptr<GameObject>& gameObject);

	void MusicCallback(AkCallbackType in_eType, AkCallbackInfo* in_pCallbackInfo);

	AKRESULT AddRoom();

	AKRESULT AddPortals(const GameObject& gameObject, const GameObject& gameObject2);

	AKRESULT SetPlayerIsInRoom(const bool& isInRoom);

private:
	void GenerateWalls(const std::shared_ptr<GameObject>& gameObject, const AkRoomID& roomID,
		const AkGeometrySetID& wallSidesGeometryID,
		const AkGeometrySetID& wallCeilingFloorGeometryID,
		const AkGeometryInstanceID& wallInstance1,
		const AkGeometryInstanceID& wallInstance2,
		const AkGeometryInstanceID& wallInstance3,
		const AkGeometryInstanceID& wallInstance4,
		const AkGeometryInstanceID& wallInstance5,
		const AkGeometryInstanceID& wallInstance6);
	AKRESULT RegisterGameObject(const AkGameObjectID& gameObjectID, std::string_view gameObjectName);
	AKRESULT UpdateGameObject(const AkGameObjectID& gameObjectID, const GameObject& gameObject);
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(std::string_view logMsg);

	std::vector<AkGameObjectID> akGameObjects;

	float musicVolume = 0;
};

