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

#include <MacTypes.h>

#ifndef __OBJC__
#include <AK/SoundEngine/Common/AkTypes.h>
#else
typedef char AkOSChar;
#endif

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

#define SOUND_BANK_PATH g_szBasePath
#define WRITABLE_PATH g_szDocumentsPath
#define AK_IOS_MAX_BANK_PATH 1024
#define AK_IOS_MAX_DOCUMENTS_PATH 1024
extern char g_szBasePath[AK_IOS_MAX_BANK_PATH];
extern char g_szDocumentsPath[AK_IOS_MAX_DOCUMENTS_PATH];

// Defines for button name strings
#define UG_BUTTON1_NAME  "Tap"
#define UG_BUTTON2_NAME  "Menu"
#define UG_BUTTON3_NAME  "UNDEFINED"
#define UG_BUTTON4_NAME  "UNDEFINED"
#define UG_BUTTON5_NAME  "UNDEFINED"
#define UG_BUTTON6_NAME  "UNDEFINED"
#define UG_BUTTON7_NAME  "Play/Pause"
#define UG_BUTTON8_NAME  "UNDEFINED"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"  
#define UG_BUTTON11_NAME "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define UG_RIGHT_STICK	 "Touchpad"
#define UG_LEFT_STICK	 "Touchpad"
#define DIRECTIONAL_TYPE "Swiping gesture"
#define RESOURCES_BUTTON	UG_BUTTON11_NAME
#define HELP_BUTTON			UG_BUTTON7_NAME
#define ACTION_BUTTON		UG_BUTTON1_NAME
#define BACK_BUTTON			UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95

#define __AK_OSCHAR_SNPRINTF snprintf

// Defines specific to iOS UI
// Interface colors

//									Red			Green		Blue		Alpha
#define JOYSTICK_BACKGROUND_COLOR	0.04296875,	0.37890625,	0.640625,	1
#define JOYSTICK_COLOR				0,			0.703125,	0.328125,	1
#define	GAMEPAD_TEXT_COLOR			0.140625,	0.09765625,	0.6953125,	1

//									Red			Green		Blue		Alpha
#define GL_DEFAULT_DRAW_COLOUR			1,			1,			1,			1	// 0xFFFFFFFF	 
#define GL_SELECTED_DRAW_COLOUR		1,			1,			0,			1	// Yellow
#define GL_TITLE_DRAW_COLOUR			1,			1,			1,			1	// White
#define GL_ERROR_DRAW_COLOUR			1,			0,			0,			1	// Red

// Number of string texture that view will cache
#define TEXTURE_CACHE_COUNT 40

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#if ! TARGET_IPHONE_SIMULATOR
	#define INTEGRATIONDEMO_MICROPHONE
#endif // #if ! TARGET_IPHONE_SIMULATOR

#define AUDIO_SAMPLE_RATE 32000

extern UInt32 g_uSamplesPerFrame;

extern bool g_bEnableMicDemo;

#define OLD_INTDEMO_RENDER

#define AK_PLATFORM_HAS_HOOKS
