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

// Platform.h
/// \file 
/// Contains platform-specific helpers for the iOS platform.

#pragma once

#ifndef __OBJC__
#include <AK/SoundEngine/Common/AkTypes.h>
#else
typedef char AkOSChar;
#endif
#include <MacTypes.h>

#define SOUND_BANK_PATH "../../../../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Mac/"

#define UG_BUTTON1_NAME  "Enter"
#define UG_BUTTON2_NAME  "Escape"
#define UG_BUTTON3_NAME  "Q"
#define UG_BUTTON4_NAME  "E"
#define UG_BUTTON5_NAME  "Shift"
#define UG_BUTTON6_NAME  "R"
#define UG_BUTTON7_NAME  "F1"
#define UG_BUTTON8_NAME  "BACK"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"  
#define UG_BUTTON11_NAME "UNDEFINED"
#define UG_RIGHT_STICK	 ""
#define UG_LEFT_STICK	 "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define DIRECTIONAL_TYPE "Arrow Keys"

#define RESOURCES_BUTTON	UG_BUTTON11_NAME
#define HELP_BUTTON			UG_BUTTON7_NAME
#define ACTION_BUTTON		UG_BUTTON1_NAME
#define BACK_BUTTON			UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95

#define __AK_OSCHAR_SNPRINTF snprintf

// Text colors
//									Red			Green		Blue		Alpha
#define GL_DEFAULT_DRAW_COLOUR			1,			1,			1,			1	// 0xFFFFFFFF	 
#define GL_SELECTED_DRAW_COLOUR		1,			1,			0,			1	// Yellow
#define GL_TITLE_DRAW_COLOUR			1,			1,			1,			1	// White
#define GL_ERROR_DRAW_COLOUR			1,			0,			0,			1	// Red

// Number of string texture that view will cache
#define TEXTURE_CACHE_COUNT 40

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#define INTEGRATIONDEMO_MICROPHONE

#define COREAUDIO_MACOSX_V10_6_AND_LATER
#define COREAUDIO_MACOSX_V10_5_AND_LATER

extern UInt32 g_uSamplesPerFrame;

#define OLD_INTDEMO_RENDER