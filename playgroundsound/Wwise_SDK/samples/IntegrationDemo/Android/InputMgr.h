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

// InputMgr.h
/// \file 
/// Contains the declaration of the Android Input Manager class.

#pragma once

#include <vector>
#include "android_native_app_glue.h"


class UniversalInput;

/// Android-Specific Input Manager class that handles all input devices and relays input
/// data to the Menu system.
class InputMgr
{
public:

	/// InputMgr class destructor
	~InputMgr();

	/// Initializes the Input Manager object.
	/// \return True if the Input Manager has been properly initialized and false otherwise.
	bool Init( 
		void* in_pParam,						///< - Extra parameter, in this case for the application's main window handle
		AkOSChar* in_szErrorBuffer,				///< - Buffer where error details will be written (if the function returns false)
		unsigned int in_unErrorBufferCharCount	///< - Number of characters available in in_szErrorBuffer, including terminating NULL character
		);

	/// Accessor method for the UniversalInput object.
	/// \return A pointer to the UniversalInput object being used by the Input Manager.
	UniversalInput* UniversalInputAdapter() const;

	/// Updates the input from all devices and relays the new input states to the Menu system.
	/// \remark This method should be called every frame before the Menu system's Update call.
	void Update();

	/// Ends the Input Manager class and releases all used resources.
	void Release();
	
	/// Flush all Inputs
	void Flush();

	static int32_t HandleInput(struct android_app* app, AInputEvent* event);

private:
	/// Pointer to the Universal Input object to interface with the Menu System
	/// \sa UniversalInput
	UniversalInput* m_pUInput;

};
