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

#include "AudioSessionUtils.h"

const char* s_labelCategory = "Session Category:";
const char* s_labelOptionsMixOthers = "Mix Others:";
const char* s_labelOptionsDuck = "Duck Others:";
const char* s_labelOptionsBluetooth = "Allow Bluetooth:";
const char* s_labelOptionsSpeaker = "Default to Speaker:";
const char* s_labelMode = "Session Mode: ";
const char* s_labelActiveOptions = "Notify on Deactivation: ";
const char* s_labelBGAudioOptions = "Background audio: ";

void InitializeAudioSessionControls(std::vector<Control*>& in_Controls, Page* in_pParent, const AkPlatformInitSettings * pfSettings)
{
	int idx = 0;

	ToggleControl* newToggle;
	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelCategory);
	newToggle->AddOption("Ambient", (void*)AkAudioSessionCategoryAmbient);
	newToggle->AddOption("SoloAmbient", (void*)AkAudioSessionCategorySoloAmbient);
	newToggle->AddOption("PlayAndRecord", (void*)AkAudioSessionCategoryPlayAndRecord);
	newToggle->AddOption("Playback", (void*)AkAudioSessionCategoryPlayback);

	switch (pfSettings->audioSession.eCategory)
	{
	case AkAudioSessionCategoryAmbient: idx = 0; break;
	case AkAudioSessionCategorySoloAmbient: idx = 1; break;
	case AkAudioSessionCategoryPlayAndRecord: idx = 2; break;
	case AkAudioSessionCategoryPlayback: idx = 3; break;
	};

	newToggle->SetSelectedIndex(idx);
	in_Controls.push_back(newToggle);

	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelOptionsMixOthers);
	newToggle->AddOption("No", (void*)0);
	newToggle->AddOption("Yes", (void*)1);
	newToggle->SetSelectedIndex((pfSettings->audioSession.eCategoryOptions & AkAudioSessionCategoryOptionMixWithOthers) != 0);
	in_Controls.push_back(newToggle);

	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelOptionsDuck);
	newToggle->AddOption("No", (void*)0);
	newToggle->AddOption("Yes", (void*)1);
	newToggle->SetSelectedIndex((pfSettings->audioSession.eCategoryOptions & AkAudioSessionCategoryOptionDuckOthers) != 0);
	in_Controls.push_back(newToggle);

	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelOptionsBluetooth);
	newToggle->AddOption("No", (void*)0);
	newToggle->AddOption("Yes", (void*)1);
	newToggle->SetSelectedIndex((pfSettings->audioSession.eCategoryOptions & AkAudioSessionCategoryOptionAllowBluetooth) != 0);
	in_Controls.push_back(newToggle);

	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelOptionsSpeaker);
	newToggle->AddOption("No", (void*)0);
	newToggle->AddOption("Yes", (void*)1);
	newToggle->SetSelectedIndex((int)((pfSettings->audioSession.eCategoryOptions & AkAudioSessionCategoryOptionDefaultToSpeaker) != 0));
	in_Controls.push_back(newToggle);

	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelMode);
	newToggle->AddOption("Default", (void*)AkAudioSessionModeDefault);
	newToggle->AddOption("Voice Chat", (void*)AkAudioSessionModeVoiceChat);
	newToggle->AddOption("Game Chat", (void*)AkAudioSessionModeGameChat);
	newToggle->AddOption("Video Recording", (void*)AkAudioSessionModeVideoRecording);
	newToggle->AddOption("Measurement", (void*)AkAudioSessionModeMeasurement);
	newToggle->AddOption("Movie Playback", (void*)AkAudioSessionModeMoviePlayback);
	newToggle->AddOption("Video Chat", (void*)AkAudioSessionModeVideoChat);

	newToggle->SetSelectedIndex((int)pfSettings->audioSession.eMode);
	in_Controls.push_back(newToggle);
	
	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelActiveOptions);
	newToggle->AddOption("No", (void*)0);
	newToggle->AddOption("Yes", (void*)1);
	newToggle->SetSelectedIndex((int)pfSettings->audioSession.eSetActivateOptions);
	in_Controls.push_back(newToggle);
	
	newToggle = new ToggleControl(*in_pParent);
	newToggle->SetLabel(s_labelBGAudioOptions);
	newToggle->AddOption("Yes, for PlayAndRecord and Playback", (void*)0);
	newToggle->AddOption("No", (void*)1);
	newToggle->SetSelectedIndex((int)pfSettings->audioSession.eAudioSessionBehavior);
	in_Controls.push_back(newToggle);
}

void FillAudioSessionSettings(std::vector<Control*>& in_Controls, AkPlatformInitSettings* io_settings)
{
	AkUIntPtr uOptions = 0;
	for (size_t i = 0; i < in_Controls.size(); i++)
	{
		if (in_Controls[i]->GetLabel() == s_labelCategory)
			io_settings->audioSession.eCategory = (AkAudioSessionCategory)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
		else if (in_Controls[i]->GetLabel() == s_labelOptionsMixOthers)
			uOptions |= (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() ? AkAudioSessionCategoryOptionMixWithOthers : 0;
		else if (in_Controls[i]->GetLabel() == s_labelOptionsDuck)
			uOptions |= (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() ? AkAudioSessionCategoryOptionDuckOthers : 0;
		else if (in_Controls[i]->GetLabel() == s_labelOptionsBluetooth)
			uOptions |= (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() ? AkAudioSessionCategoryOptionAllowBluetooth | AkAudioSessionCategoryOptionAllowBluetoothA2DP : 0;
		else if (in_Controls[i]->GetLabel() == s_labelOptionsSpeaker)
			uOptions |= (AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue() ? AkAudioSessionCategoryOptionDefaultToSpeaker : 0;
		else if (in_Controls[i]->GetLabel() == s_labelMode)
			io_settings->audioSession.eMode = (AkAudioSessionMode)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
		else if (in_Controls[i]->GetLabel() == s_labelActiveOptions)
			io_settings->audioSession.eSetActivateOptions = (AkAudioSessionSetActiveOptions)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
		else if (in_Controls[i]->GetLabel() == s_labelBGAudioOptions)
			io_settings->audioSession.eAudioSessionBehavior = (AkAudioSessionBehaviorOptions)(AkUIntPtr)((ToggleControl*)in_Controls[i])->SelectedValue();
	}

	io_settings->audioSession.eCategoryOptions = (AkAudioSessionCategoryOptions)uOptions;
}

static bool g_bAudioSessionInterrupted = false;
static bool g_bBackgroundMusicMuted = false;

bool IsAudioSessionInterrupted()
{
	return g_bAudioSessionInterrupted;
}

bool IsBackgroundMusicMuted()
{
	return g_bBackgroundMusicMuted;
}

void OnAudioSessionInterruption(bool in_bEnterInterruption,	void* in_pCookie)
{
	g_bAudioSessionInterrupted = in_bEnterInterruption;
}

AKRESULT OnBackgroundMusicChange(bool bMuted, void* in_pCookie)
{
	g_bBackgroundMusicMuted = bMuted;
	return AK_Success;
}
