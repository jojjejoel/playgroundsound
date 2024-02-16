#include "WwiseAPI.h"
#include <iostream>

//// Memory management
#include <AK/SoundEngine/Common/AkMemoryMgr.h>
#include <AK/SoundEngine/Common/AkModule.h>
//
//// Streaming
#include <AK/SoundEngine/Common/IAkStreamMgr.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AkFilePackageLowLevelIOBlocking.h>


//// File packaging
#include "AkFilePackage.h"
#include "AkFilePackageLUT.h"


#include "Wwise_IDs.h"

#define BANKNAME_INIT L"Init.bnk"
#define BANKNAME_MAIN L"Main.bnk"

bool WwiseAPI::Init()
{


	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) == AK_Success)
	{
		std::cout << "Memory Manager initialized" << std::endl;
	}
	else
	{
		std::cout << "Memory Manager failed to initialize" << std::endl;
		return false;
	}

	AkStreamMgrSettings streamSettings;
	AK::StreamMgr::GetDefaultSettings(streamSettings);

	if (!AK::StreamMgr::Create(streamSettings))
	{
		std::cout << "Could not create the Streaming Manager" << std::endl;
	}
	else
	{
		std::cout << "Streaming Manager created!" << std::endl;
	}


	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	if (g_lowLevelIO.Init(deviceSettings) == AK_Success)
	{
		std::cout << "Created the streaming device and low level I/O system!" << std::endl;
	}
	else
	{
		std::cout << "Could not create streaming device and low level I/O system!" << std::endl;
		return false;
	}


	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);
	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) == AK_Success)
	{
		std::cout << "Sound engine initialized!" << std::endl;
	}
	else
	{
		std::cout << "Could not initialize sound engine!" << std::endl;
		return false;
	}


	g_lowLevelIO.SetBasePath(AKTEXT("C:\\Users\\Joel.Schultz\\Documents\\GitHub\\playgroundsound\\playgroundsound\\Wwise\\GeneratedSoundBanks\\Windows"));
	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

	AkBankID initBankID;
	LoadBank(BANKNAME_INIT, initBankID);
	if (initBankID == AK_INVALID_BANK_ID)
	{
		Log(L"Failed to load init bank");
	}

	AkBankID mainBankID;
	LoadBank(BANKNAME_MAIN, mainBankID);
	if (initBankID == AK_INVALID_BANK_ID)
	{
		Log(L"Failed to load main bank");
	}


	AkGameObjectID gameObjectIDTest = 1;
	AkGameObjectID listenerGameObject = 2;

	if (AK::SoundEngine::RegisterGameObj(gameObjectIDTest, "Test") == AK_Success)
	{
		std::cout << "GameObject registered" << std::endl;
	}

	if (AK::SoundEngine::RegisterGameObj(listenerGameObject, "Test") == AK_Success)
	{
		std::cout << "GameObject registered" << std::endl;
	}

	if (AK::SoundEngine::AddDefaultListener(listenerGameObject) == AK_Success)
	{
		std::cout << "Listener added" << std::endl;
	}

	AkPlayingID playingID = AK::SoundEngine::PostEvent(AK::EVENTS::PLAY_TEST, gameObjectIDTest);
	if (playingID == AK_Success)
	{
		std::cout << "Posted event!" << std::endl;
	}
	else
	{
		std::cout << "Post event FAILED" << std::endl;
	}
	int a = 0;
	AK::SoundEngine::RenderAudio();

	return true;
}

void WwiseAPI::LoadBank(const wchar_t* bankName, AkBankID& bankID)
{
	AK::SoundEngine::LoadBank(bankName, bankID);
}

void WwiseAPI::Log(const wchar_t* logMsg) {
	std::cout << logMsg << std::endl;
}
