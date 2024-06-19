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

#include "PlatformHooks.h"
#include "IntegrationDemo.h"
#include "AudioSessionUtils.h"

namespace AK
{
	namespace IntegrationDemo
	{
		class iOSHooks : public PlatformHooks
		{
		public:
			virtual void PreInit(::IntegrationDemo* pDemo)
			{
				pDemo->GetPlatformInitSettings().audioCallbacks.interruptionCallback = OnAudioSessionInterruption;
				pDemo->GetInitSettings().BGMCallback = OnBackgroundMusicChange;
			}
		private:
			AkThreadProperties arThreadProperties[AK_MAX_WORKER_THREADS];
		};

		static iOSHooks g_PlatformHooks;

		PlatformHooks* PlatformHooks::Get()
		{
			return &g_PlatformHooks;
		}
	}
}
