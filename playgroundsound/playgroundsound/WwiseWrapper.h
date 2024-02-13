#pragma once

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

class WwiseWrapper
{

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
	public: bool Init();

};

