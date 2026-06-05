#pragma once

// Must define these BEFORE including Wwise SDK headers that pull in Windows.h
#define NOMINMAX
#define NOGDI
#define NOUSER

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

#include "DiffractionPath.h"

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>

class WwiseAPI
{
public:
	void Init();
	void DeInit();
	AKRESULT LoadBank(const AkUniqueID& bankID);
	void RenderAudio();
	std::vector<DiffractionPath> GetDiffractionPaths(const AkGameObjectID& gameObjectID);
private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(std::string_view logMsg);

	// Helper methods for vector conversion
	GO_Vector3 ConvertAkVectorToGO(const AkVector64& akVec) const;
	GO_Vector3 ConvertAkVectorToGOWithZFlip(const AkVector64& akVec) const;
	void ConvertPathNodes(const AkDiffractionPathInfo& akPath, DiffractionPath& path) const;
	void ConvertVirtualPosition(const AkDiffractionPathInfo& akPath, DiffractionPath& path) const;
};

