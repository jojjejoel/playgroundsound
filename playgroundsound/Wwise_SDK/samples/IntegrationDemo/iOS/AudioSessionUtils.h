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

#include <vector>
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "Page.h"

void InitializeAudioSessionControls(std::vector<Control*>& in_Controls, Page* in_pParent, const AkPlatformInitSettings * pfSettings);
void FillAudioSessionSettings(std::vector<Control*>& in_Controls, AkPlatformInitSettings* io_settings);
bool IsAudioSessionInterrupted();
bool IsBackgroundMusicMuted();

void OnAudioSessionInterruption(bool in_bEnterInterruption,	void* in_pCookie);
AKRESULT OnBackgroundMusicChange(bool in_bBackgroundMusicMuted, void* in_pCookie);
