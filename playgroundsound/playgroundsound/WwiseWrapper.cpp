#include "WwiseWrapper.h"
#include <iostream>

bool WwiseWrapper::Init()
{

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

	AkBankID bankID;

	if (AK::SoundEngine::LoadBank(BANKNAME_INIT, bankID) != AK_Success)
	{
		std::cout << "ERROR: Could not load Init Bank" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Init bank loaded" << std::endl;
	}

	if (AK::SoundEngine::LoadBank(BANKNAME_MAIN, bankID) != AK_Success)
	{
		std::cout << "ERROR: Could not load Main Bank" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Main bank loaded" << std::endl;
	}

	return true;
}
