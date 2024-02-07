#include "WwiseWrapper.h"
#include <iostream>

bool WwiseWrapper::Init()
{
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) == AK_Success)
	{
		std::cout << "Memory Manager initialized" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Memory Manager failed to initialize" << std::endl;
		return false;
	}
}
