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

#include "Wwise_IDs.h"

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

	g_lowLevelIO.SetBasePath(AKTEXT("..\\Wwise\\GeneratedSoundBanks\\Windows"));
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
	AK::SoundEngine::Term();
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}
	AK::Comm::Term();
	AK::MemoryMgr::Term();
	AK::MusicEngine::Term();
}

AKRESULT WwiseAPI::LoadBank(const AkUniqueID& bankID)
{
	return AK::SoundEngine::LoadBank(bankID);
}

void WwiseAPI::RenderAudio()
{
	AK::SoundEngine::RenderAudio();
}

std::vector<DiffractionPath> WwiseAPI::GetDiffraction(const AkGameObjectID& gameObjectID) {
	AkVector64 emitterPos;
	AkVector64 listenerPos;
	AkDiffractionPathInfo akDiffractionPaths[8];
	AkUInt32 numberOfPaths = 8;
	AK::SpatialAudio::QueryDiffractionPaths(gameObjectID, 0, listenerPos, emitterPos, akDiffractionPaths, numberOfPaths);

	int numLines = 0;

	std::vector<DiffractionPath> diffractionPaths;
	for (AkUInt32 path = 0; path < numberOfPaths; path++)
	{
		DiffractionPath diffractionPath;
		AkDiffractionPathInfo akDiffractionPath = akDiffractionPaths[path];
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.nodes[i].x = akDiffractionPath.nodes[i].X;
			diffractionPath.nodes[i].y = akDiffractionPath.nodes[i].Y;
			diffractionPath.nodes[i].z = -akDiffractionPath.nodes[i].Z;
		}
		diffractionPath.emitterPos.x = emitterPos.X;
		diffractionPath.emitterPos.y = emitterPos.Y;
		diffractionPath.emitterPos.z = emitterPos.Z;
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{
			diffractionPath.angles[i] = akDiffractionPath.angles[i];
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.portals[i] = akDiffractionPath.portals[i];
		}
		for (size_t i = 0; i < akDiffractionPath.nodeCount; i++)
		{

			diffractionPath.rooms[i] = akDiffractionPath.rooms[i];
		}
		diffractionPath.virtualPos.forward.x = -akDiffractionPath.virtualPos.OrientationFront().X;
		diffractionPath.virtualPos.forward.y = -akDiffractionPath.virtualPos.OrientationFront().Y;
		diffractionPath.virtualPos.forward.z = akDiffractionPath.virtualPos.OrientationFront().Z;
		diffractionPath.virtualPos.up.x = akDiffractionPath.virtualPos.OrientationTop().X;
		diffractionPath.virtualPos.up.y = akDiffractionPath.virtualPos.OrientationTop().Y;
		diffractionPath.virtualPos.up.z = -akDiffractionPath.virtualPos.OrientationTop().Z;
		diffractionPath.virtualPos.position.x = akDiffractionPath.virtualPos.Position().X;
		diffractionPath.virtualPos.position.y = akDiffractionPath.virtualPos.Position().Y;
		diffractionPath.virtualPos.position.z = -akDiffractionPath.virtualPos.Position().Z;
		diffractionPath.nodeCount = akDiffractionPath.nodeCount;
		diffractionPath.diffraction = akDiffractionPath.diffraction;
		diffractionPath.totLength = akDiffractionPath.totLength;
		diffractionPath.obstructionValue = akDiffractionPath.obstructionValue;
		diffractionPaths.emplace_back(diffractionPath);
	}
	return diffractionPaths;
}

void WwiseAPI::Log(std::string_view logMsg) {
	std::cout << logMsg << std::endl;
}
