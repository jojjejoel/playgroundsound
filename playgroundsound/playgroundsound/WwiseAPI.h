#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>
#include <SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>
#include <AK/SoundEngine/Common/AkCallback.h>
#include <AK/SpatialAudio/Common/AkSpatialAudioTypes.h>
#include <AK/SoundEngine/Common/AkQueryParameters.h>

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
	void RenderAudio();
	std::vector<DiffractionPath> GetDiffractionPaths(const AkGameObjectID& gameObjectID);
private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	void Log(std::string_view logMsg);
};

