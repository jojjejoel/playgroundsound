#include "WwiseAPI.h"

#include <iostream>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include "AkFilePackage.h"
#include "AkFilePackageLUT.h"
#include "AK/Comm/AkCommunication.h"
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"
#include "AK/Plugin/AkRoomVerbFXFactory.h"
#include <AK/Plugin/AkMeterFXFactory.h>
#include <AK/Plugin/AkGainFXFactory.h>
#include <AK/Plugin/AkParametricEQFXFactory.h>
#include <AK/Plugin/AkVorbisDecoderFactory.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>

#include "Wwise_Project/GeneratedSoundBanks/Wwise_IDs.h"

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

	AkMusicSettings musicInitSettings;
	AK::MusicEngine::GetDefaultInitSettings(musicInitSettings);
	AK::MusicEngine::Init(&musicInitSettings);

	g_lowLevelIO.SetBasePath(AKTEXT("Wwise_Project\\GeneratedSoundBanks\\Windows"));
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
	AK::SoundEngine::StopAll();
	#ifndef AK_OPTIMIZED 

		AK::Comm::Term();

	#endif // !AK_OPTIMIZED

	AK::MusicEngine::Term();
	AK::SoundEngine::Term();
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}
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

GO_Vector3 WwiseAPI::ConvertAkVectorToGO(const AkVector64& akVec) const
{
	return GO_Vector3{ static_cast<float>(akVec.X), static_cast<float>(akVec.Y), static_cast<float>(akVec.Z) };
}

GO_Vector3 WwiseAPI::ConvertAkVectorToGOWithZFlip(const AkVector64& akVec) const
{
	return GO_Vector3{ static_cast<float>(akVec.X), static_cast<float>(akVec.Y), static_cast<float>(-akVec.Z) };
}

void WwiseAPI::ConvertPathNodes(const AkDiffractionPathInfo& akPath, DiffractionPath& path) const
{
	for (size_t i = 0; i < akPath.nodeCount; i++) {
		path.nodes[i] = ConvertAkVectorToGOWithZFlip(akPath.nodes[i]);
		path.angles[i] = akPath.angles[i];
		path.portals[i] = static_cast<int>(akPath.portals[i]);
		path.rooms[i] = static_cast<int>(akPath.rooms[i]);
	}
}

void WwiseAPI::ConvertVirtualPosition(const AkDiffractionPathInfo& akPath, DiffractionPath& path) const
{
	const AkVector& akOrientationFront = akPath.virtualPos.OrientationFront();
	path.virtualPos.forward = GO_Vector3{ -akOrientationFront.X, -akOrientationFront.Y, akOrientationFront.Z };

	const AkVector& akOrientationTop = akPath.virtualPos.OrientationTop();
	path.virtualPos.up = GO_Vector3{ akOrientationTop.X, akOrientationTop.Y, -akOrientationTop.Z };

	path.virtualPos.position = ConvertAkVectorToGOWithZFlip(akPath.virtualPos.Position());
}

std::vector<DiffractionPath> WwiseAPI::GetDiffractionPaths(const AkGameObjectID& gameObjectID) {
    AkVector64 emitterPos;
    AkVector64 listenerPos;
    AkDiffractionPathInfo akDiffractionPaths[8];
    AkUInt32 numberOfPaths = 8;
    AK::SpatialAudio::QueryDiffractionPaths(gameObjectID, 0, listenerPos, emitterPos, akDiffractionPaths, numberOfPaths);

    std::vector<DiffractionPath> diffractionPaths;
    diffractionPaths.reserve(numberOfPaths);

    for (AkUInt32 path = 0; path < numberOfPaths; path++) {
        const AkDiffractionPathInfo& akDiffractionPath = akDiffractionPaths[path];
        DiffractionPath diffractionPath;

        ConvertPathNodes(akDiffractionPath, diffractionPath);
        ConvertVirtualPosition(akDiffractionPath, diffractionPath);

        diffractionPath.emitterPos = ConvertAkVectorToGO(emitterPos);
        diffractionPath.nodeCount = akDiffractionPath.nodeCount;
        diffractionPath.diffraction = akDiffractionPath.diffraction;
        diffractionPath.totLength = akDiffractionPath.totLength;
        diffractionPath.obstructionValue = akDiffractionPath.obstructionValue;

        diffractionPaths.emplace_back(std::move(diffractionPath));
    }
    return diffractionPaths;
}

void WwiseAPI::Log(std::string_view logMsg) {
	std::cout << logMsg << std::endl;
}
