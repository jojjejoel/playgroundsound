/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#include "AndroidUtils.h"
#include "IntegrationDemo.h"
#include "AkFileHelpersAndroid.h"

struct android_app* g_AkAndroidApp = nullptr;

void AK::Android::SetGlobalApplication(struct android_app* in_pApplication)
{
    g_AkAndroidApp = in_pApplication;
}

void AK::Android::InitLLIO()
{
	if ( IntegrationDemo::Instance().GetLowLevelIOHandler()->Init(g_AkAndroidApp->activity->vm, g_AkAndroidApp->activity->clazz) != AK_Success )
		LOGI("Cannot initialize Android IO\n");
}

void AK::Android::AddLLIOBasePaths()
{
	// We should add some writable paths for the AkRecorder plug-in and other debug utilities.

	// Add the internal application cache path (which is not easily accessible from a PC)
	IntegrationDemo::Instance().GetLowLevelIOHandler()->AddBasePath( g_AkAndroidApp->activity->internalDataPath );

	// Also add the external cache path if available (which is easily accessible from a PC)
	// We add it AFTER the internal one so it takes precedence.
	static char tmpDirPath[1024];
	if (CAkFileHelpersAndroid::GetWritablePath(CAkFileHelpersAndroid::ExternalCacheDir, g_AkAndroidApp->activity->vm, g_AkAndroidApp->activity->clazz, tmpDirPath, 1024) == AK_Success)
	{
		IntegrationDemo::Instance().GetLowLevelIOHandler()->AddBasePath(tmpDirPath);
	}
}
