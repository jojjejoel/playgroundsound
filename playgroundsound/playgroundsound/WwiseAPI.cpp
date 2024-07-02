#include "WwiseAPI.h"

#include <iostream>

#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <SoundEngine/Common/AkFilePackage.h>
#include "SoundEngine/Common/AkFilePackageLUT.h"
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

        for (size_t i = 0; i < akDiffractionPath.nodeCount; i++) {
			const AkVector64& akNode = akDiffractionPath.nodes[i];
			GO_Vector3& convertedNode = diffractionPath.nodes[i];

            convertedNode.x = static_cast<float>(akNode.X);
            convertedNode.y = static_cast<float>(akNode.Y);
            convertedNode.z = static_cast<float>(-akNode.Z);

            diffractionPath.angles[i] = akDiffractionPath.angles[i];
            diffractionPath.portals[i] = static_cast<int>(akDiffractionPath.portals[i]);
            diffractionPath.rooms[i] = static_cast<int>(akDiffractionPath.rooms[i]);
        }

        diffractionPath.emitterPos.x = static_cast<float>(emitterPos.X);
        diffractionPath.emitterPos.y = static_cast<float>(emitterPos.Y);
        diffractionPath.emitterPos.z = static_cast<float>(emitterPos.Z);

		const AkVector& akOrientationFront = akDiffractionPath.virtualPos.OrientationFront();
        diffractionPath.virtualPos.forward.x = -akOrientationFront.X;
        diffractionPath.virtualPos.forward.y = -akOrientationFront.Y;
        diffractionPath.virtualPos.forward.z = akOrientationFront.Z;

		const AkVector& akOrientationTop = akDiffractionPath.virtualPos.OrientationTop();
        diffractionPath.virtualPos.up.x = akOrientationTop.X;
        diffractionPath.virtualPos.up.y = akOrientationTop.Y;
        diffractionPath.virtualPos.up.z = -akOrientationTop.Z;

		const AkVector64 akPosition = akDiffractionPath.virtualPos.Position();
        diffractionPath.virtualPos.position.x = static_cast<float>(akPosition.X);
        diffractionPath.virtualPos.position.y = static_cast<float>(akPosition.Y);
        diffractionPath.virtualPos.position.z = static_cast<float>(-akPosition.Z);

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
