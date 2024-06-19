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

#include "stdafx.h"
#include <android_native_app_glue.h>
#include <android/log.h>
#include "DemoOptionsUtil.h"
#include "../MenuSystem/ToggleControl.h"
#include "AkFileHelpers.h"
#include "IntegrationDemo.h"
#include "AndroidUtils.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>

const char* s_labelRound = "Round Frame To HW Size";
const char* s_labelVerbose = "Verbose Debug Logcat";
const char* s_labelLowLatency = "Use Low Latency Mode";

namespace demoOptionsUtil
{
	void PreInitWwise()
	{
		AK::Android::InitLLIO();
	}

	void PostInitWwise()
	{
		AK::Android::AddLLIOBasePaths();
	}

	AkUInt32 GetDefaultSpeakerConfigs(const char**& out_pNames, AkChannelConfig*& out_pConfigs)
	{
		static const char* names[] = { "Automatic", "Mono", "Stereo", "5.1" };
		const size_t uNumNames = sizeof(names) / sizeof(names[0]);

		int i = 0;
		static AkChannelConfig configs[uNumNames];
		configs[i++].SetStandard(0);
		configs[i++].SetStandard(AK_SPEAKER_SETUP_MONO);
		configs[i++].SetStandard(AK_SPEAKER_SETUP_STEREO);
		configs[i++].SetStandard(AK_SPEAKER_SETUP_5POINT1);

		out_pNames = names;
		out_pConfigs = configs;
		
		return i;
	}

	void InitPlatformSpecificControls(std::vector<Control*>& in_Controls, Page* in_pParent)
	{
		//Add platform specific controls
		AK::IAkGlobalPluginContext* pGlobalCtx = AK::SoundEngine::GetGlobalPluginContext();
		const AkPlatformInitSettings* pd_settings = pGlobalCtx->GetPlatformInitSettings();
		int idx = 0;
		char buf[20];

		ToggleControl* newToggle = new ToggleControl(*in_pParent);
		AddSampleRateOption(newToggle, pd_settings->uSampleRate);		
		in_Controls.push_back(newToggle);

		newToggle = new ToggleControl(*in_pParent);
		newToggle->SetLabel(s_labelAudioAPI);
		newToggle->AddOption("Automatic", (void*)AkAudioAPI_Default);
		newToggle->AddOption("AAudio", (void*)AkAudioAPI_AAudio);
		newToggle->AddOption("OpenSL ES", (void*)AkAudioAPI_OpenSL_ES);
		
		switch (pd_settings->eAudioAPI)
		{
		case AkAudioAPI_Default: idx = 0; break;
		case AkAudioAPI_AAudio: idx = 1; break;
		case AkAudioAPI_OpenSL_ES: idx = 2; break;
		};

		newToggle->SetSelectedIndex(idx);
		in_Controls.push_back(newToggle);

		newToggle = new ToggleControl(*in_pParent);
		newToggle->SetLabel(s_labelRound);
		newToggle->AddOption("No", (void*)0);
		newToggle->AddOption("Yes", (void*)1);
		newToggle->SetSelectedIndex(pd_settings->bRoundFrameSizeToHWSize);
		in_Controls.push_back(newToggle);

		newToggle = new ToggleControl(*in_pParent);
		newToggle->SetLabel(s_labelVerbose);
		newToggle->AddOption("No", (void*)0);
		newToggle->AddOption("Yes", (void*)1);
		newToggle->SetSelectedIndex(pd_settings->bVerboseSink);
		in_Controls.push_back(newToggle);
		
		newToggle = new ToggleControl(*in_pParent);
		newToggle->SetLabel(s_labelLowLatency);
		newToggle->AddOption("No", (void*)0);
		newToggle->AddOption("Yes", (void*)1);
		newToggle->SetSelectedIndex(pd_settings->bEnableLowLatency);
		in_Controls.push_back(newToggle);
	}

	void FillPlatformSpecificSettings(std::vector<Control*>& in_Controls, AkPlatformInitSettings* io_settings)
	{
		for (size_t i = 0; i < in_Controls.size(); i++)
		{
			if (in_Controls[i]->GetLabel() == s_labelRate)
				io_settings->uSampleRate = (AkUInt32)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
			else if (in_Controls[i]->GetLabel() == s_labelAudioAPI)
				io_settings->eAudioAPI = (AkAudioAPI)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
			else if (in_Controls[i]->GetLabel() == s_labelRound)
				io_settings->bRoundFrameSizeToHWSize = (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() != 0;
			else if (in_Controls[i]->GetLabel() == s_labelVerbose)
				io_settings->bVerboseSink = (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() != 0;
			else if (in_Controls[i]->GetLabel() == s_labelLowLatency)
				io_settings->bEnableLowLatency = (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() != 0;
		}
	}
		
	void UpdatePlatformSpecificControls(std::vector<Control*>& in_Controls)
	{

	}
}