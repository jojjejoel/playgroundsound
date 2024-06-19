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

#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#include <AK/Tools/Android/AkJNIThread.h>

class CAkFileHelpersAndroid
{
public:

	/// Possible storage locations for Android
	enum AppStorageLocation
	{
		InternalFilesDir, // Same as android.content.Context.getFilesDir (primary internal storage for persistent files)
		InternalCacheDir, // Same as android.content.Context.getCacheDir (primary internal storage for temporary files)
		ExternalCacheDir, // Same as android.content.Context.getExternalCacheDir (primary external storage for temporary files)
	};
	

	static const char * GetJNIStorageLocationMethod(AppStorageLocation in_eLocation)
	{
		switch (in_eLocation)
		{
			case InternalFilesDir:
				return "getFilesDir";
			case InternalCacheDir:
				return "getCacheDir";
			case ExternalCacheDir:
				return "getExternalCacheDir";
			default:
				AKASSERT(!"Unexpected value for AppStorageLocation!");
				return nullptr;
		}
	}

	/// Calls JNI APIs to retrieve the path returned by the Context object (see AppStorageLocation documentation)
	static AKRESULT GetWritablePath(AppStorageLocation in_eLocation, JavaVM* in_lJavaVM, jobject in_jActivity, AkOSChar* out_path, size_t path_size)
	{
		if (out_path == NULL)
			return AK_InsufficientMemory;

		out_path[0] = '\0';

		// This utility will release the JNI environment automatically
		AKPLATFORM::CAkJNIThread jThread(in_lJavaVM);
		if(!jThread.isAttached())
			return AK_Fail;

		JNIEnv* lJNIEnv = jThread.getEnv();

		// Resolve the Java classes needed to call the APIs
		jclass Context = lJNIEnv->FindClass("android/content/Context");
		jclass File = lJNIEnv->FindClass("java/io/File");
		if (Context == NULL || File == NULL)
			return AK_Fail;

		// Resolve the Java methods needed to call the APIs
		jmethodID GetWriteablePath = lJNIEnv->GetMethodID(Context, GetJNIStorageLocationMethod(in_eLocation), "()Ljava/io/File;");
		jmethodID GetAbsolutePath = lJNIEnv->GetMethodID(File, "getAbsolutePath", "()Ljava/lang/String;");
		if (GetWriteablePath == NULL || GetAbsolutePath == NULL)
			return AK_Fail;

		// Get the File object representing the cache path
		jobject WriteableDir = lJNIEnv->CallObjectMethod(in_jActivity, GetWriteablePath);
		if (WriteableDir == NULL) return AK_Fail;

		// Get the Java string representing the absolute path to the cache path
		jstring WriteablePath = (jstring)lJNIEnv->CallObjectMethod(WriteableDir, GetAbsolutePath);
		if (WriteablePath == NULL) return AK_Fail;

		// Convert Java string to C-style string and copy it locally
		const char *cStr = lJNIEnv->GetStringUTFChars(WriteablePath, NULL);
		if (cStr == NULL) return AK_Fail;
		AKPLATFORM::SafeStrCpy(out_path, cStr, path_size);
		lJNIEnv->ReleaseStringUTFChars(WriteablePath, cStr);

		return AK_Success;
	}
};
