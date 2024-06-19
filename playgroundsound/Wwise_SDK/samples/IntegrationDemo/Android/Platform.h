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
/// Contains platform-specific helpers for the Windows platform.

#pragma once

// WG-21707
// Soundbanks will be automatically deployed to your Android device as part of the APK, if you 
// place the generated soundbanks under $WWISESDK/samples/IntegrationDemo/Android/assets/GeneratedSoundBanks/Android
// NOTE: SOUND_BANK_PATH needs the trailing path separator.
#define SOUND_BANK_PATH "GeneratedSoundBanks/Android/"

// Defines for button name strings
#define UG_BUTTON1_NAME  "Enter"
#define UG_BUTTON2_NAME  "Space"
#define UG_BUTTON3_NAME  "Z"
#define UG_BUTTON4_NAME  "X"
#define UG_BUTTON5_NAME  "C"
#define UG_BUTTON6_NAME  "V"
#define UG_BUTTON7_NAME  "B"
#define UG_BUTTON8_NAME  "N"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"   
#define UG_BUTTON11_NAME "UNDEFINED"
#define UG_RIGHT_STICK	 ""
#define UG_LEFT_STICK	 "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define DIRECTIONAL_TYPE "A,W,D,S"

#define RESOURCES_BUTTON	UG_BUTTON11_NAME
#define HELP_BUTTON			UG_BUTTON7_NAME
#define ACTION_BUTTON		UG_BUTTON1_NAME
#define BACK_BUTTON			UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	60

#define __AK_OSCHAR_SNPRINTF snprintf

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#define OLD_INTDEMO_RENDER

#define AK_PLATFORM_SUPPORTS_MOTION
