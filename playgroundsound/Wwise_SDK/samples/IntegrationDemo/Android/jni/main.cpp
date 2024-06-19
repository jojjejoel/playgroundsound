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

// FreetypeGraphicRenderer.cpp
/// \file
/// Integration demo main.cpp implementation

#include <stdlib.h>
#include "AndroidUtils.h"
#include "AkFilePackageLowLevelIOBlocking.h"

// To see debug output of the Integraiton Demo:
// adb logcat ActivityManager:I IntegrationDemo:D *:S

// To see debug ouptut from the android_native_app_glue:
// adb logcat ActivityManager:I threaded_app *:S

#include <AK/Tools/Android/AkJNIThread.h>
#include <DemoOptionsUtil.h>
#include "IntegrationDemo.h"
#include "Drawing.h"
#include "InputMgr.h"
#include "../FreetypeRenderer/FreetypeGraphicRenderer.h"

// Top 3/4th of the window
extern FreetypeGraphicRenderer* g_pFreetypeRenderer;

static bool g_isAppRunning = false;

static bool g_bAppIsInitialized = false;

static void CopyRenderBufferToWindow( ANativeWindow_Buffer* in_pBuffer, FreetypeGraphicRenderer * in_pRenderer, int in_iY = 0)
{
	size_t iWidth = in_pRenderer->GetWindowWidth();
	const uint16_t* bufferToCopy = in_pRenderer->GetWindowBuffer();

	//The screen may have turned as well.  For now, limit the copy to the minimum size.
	size_t minW = AkMin(iWidth, in_pBuffer->width);
	size_t minH = AkMin(in_pRenderer->GetWindowHeight(), in_pBuffer->height);

	// Need to take into account the stride of the app buffer on galaxy nexus.
	uint16_t * bits = (uint16_t *) in_pBuffer->bits + in_pBuffer->stride * in_iY;
	for ( int i = 0; i < minH; ++i )
	{
		memcpy(bits, bufferToCopy, minW * sizeof( uint16_t ) );
		bits += in_pBuffer->stride;
		bufferToCopy += iWidth;
	}
}

static void PostFrame( ANativeWindow_Buffer* in_pBuffer )
{
	if(g_pFreetypeRenderer == NULL )
		return;

	CopyRenderBufferToWindow( in_pBuffer, g_pFreetypeRenderer );
}

static void TermApp(struct android_app* app)
{
	if(g_bAppIsInitialized)
	{
		g_bAppIsInitialized = false;
		IntegrationDemo::Instance().Term();	
	}
}

extern void DisplayKeyboard(android_app* app, bool pShow)
{
    // Attaches the current thread to the JVM.
    jint lResult;
    jint lFlags = 0;

    JavaVM* lJavaVM = app->activity->vm;
    JNIEnv* lJNIEnv = app->activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name = "NativeThread";
    lJavaVMAttachArgs.group = NULL;

    lResult=lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);
    if (lResult == JNI_ERR)
        return;

    // Retrieves NativeActivity.
    jobject lNativeActivity = app->activity->clazz;
    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE.
    jclass ClassContext = lJNIEnv->FindClass("android/content/Context");
    jfieldID FieldINPUT_METHOD_SERVICE = lJNIEnv->GetStaticFieldID(ClassContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
    jobject INPUT_METHOD_SERVICE = lJNIEnv->GetStaticObjectField(ClassContext, FieldINPUT_METHOD_SERVICE);
//    jniCheck(INPUT_METHOD_SERVICE);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE).
    jclass ClassInputMethodManager = lJNIEnv->FindClass( "android/view/inputmethod/InputMethodManager");
    jmethodID MethodGetSystemService = lJNIEnv->GetMethodID( ClassNativeActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod( lNativeActivity, MethodGetSystemService, INPUT_METHOD_SERVICE);

    // Runs getWindow().getDecorView().
    jmethodID MethodGetWindow = lJNIEnv->GetMethodID( ClassNativeActivity, "getWindow", "()Landroid/view/Window;");
    jobject lWindow = lJNIEnv->CallObjectMethod(lNativeActivity, MethodGetWindow);
    jclass ClassWindow = lJNIEnv->FindClass( "android/view/Window");
    jmethodID MethodGetDecorView = lJNIEnv->GetMethodID( ClassWindow, "getDecorView", "()Landroid/view/View;");
    jobject lDecorView = lJNIEnv->CallObjectMethod(lWindow, MethodGetDecorView);

    if (pShow) 
	{
        // Runs lInputMethodManager.showSoftInput(...).
        jmethodID MethodShowSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
        jboolean lResult = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodShowSoftInput, lDecorView, lFlags);
    } 
	else 
	{
        // Runs lWindow.getViewToken()
        jclass ClassView = lJNIEnv->FindClass( "android/view/View");
        jmethodID MethodGetWindowToken = lJNIEnv->GetMethodID( ClassView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject lBinder = lJNIEnv->CallObjectMethod(lDecorView, MethodGetWindowToken);

        // lInputMethodManager.hideSoftInput(...).
        jmethodID MethodHideSoftInput = lJNIEnv->GetMethodID( ClassInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
        jboolean lRes = lJNIEnv->CallBooleanMethod( lInputMethodManager, MethodHideSoftInput, lBinder, lFlags);
    }

    // Finished with the JVM.
    lJavaVM->DetachCurrentThread();
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
   switch (cmd)
   {
		case APP_CMD_START:
			break;
		case APP_CMD_STOP:
			break;
		case APP_CMD_PAUSE:			
			IntegrationDemo::Instance().PauseAllSounds();
			break;
		case APP_CMD_RESUME:			
			IntegrationDemo::Instance().ResumeAllSounds();
			break;
		case APP_CMD_SAVE_STATE:
			break;
		case APP_CMD_INIT_WINDOW:
		{
			AK::Android::SetGlobalApplication(app);
			ANativeWindow_setBuffersGeometry( app->window, 0, 0, WINDOW_FORMAT_RGB_565 );

			// Need to do this in order to get valid width and height back from
			// ANativeWindow_getWidth ANativeWindow_getHeight
			ANativeWindow_Buffer buffer;
			ANativeWindow_lock(app->window, &buffer, NULL);
			ANativeWindow_unlockAndPost(app->window);

			int32_t width = buffer.width;
			int32_t height = buffer.height;
			int32_t stride = buffer.stride;

			if (g_isAppRunning) {
				LOGI("App is running. Skip initialization.");
				break;
			}
			
			LOGI("App window is %d x %d (stride %d)", width, height, stride );			
			
			char errBuffer[256];

			//Change default values of AkPlatformInitSettings before the call to Init.
			//All other settings can also be changed in similar way
			AkPlatformInitSettings& platformInitSettings = IntegrationDemo::Instance().GetPlatformInitSettings();
			platformInitSettings.pJavaVM = app->activity->vm;
			platformInitSettings.jActivity = app->activity->clazz;

			AK::Android::InitLLIO();

			if( !IntegrationDemo::Instance().Init( NULL, errBuffer, 256,width, height) )
			{
				LOGI("%s", errBuffer);
			}
			else
			{
				g_isAppRunning = true;
				g_bAppIsInitialized = true;
				AK::Android::AddLLIOBasePaths();
			}
		}
			break;
		case APP_CMD_TERM_WINDOW:
			break;
		case APP_CMD_DESTROY:
			TermApp(app);
			g_isAppRunning = false;
			exit(0); // ensure process is not reused when restarting app.
			break;

		case APP_CMD_LOST_FOCUS:			
			IntegrationDemo::Instance().PauseAllSounds();
			break;
		case APP_CMD_GAINED_FOCUS:			
			IntegrationDemo::Instance().ResumeAllSounds();		
			break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state)
{
    // Set callback function for command and input event
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = InputMgr::HandleInput;
    state->userData = IntegrationDemo::Instance().GetInputMgr();

    // loop waiting for stuff to do.
    while (1)
    {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
            // Process this event.
            if (source != NULL)
                source->process(state, source);

            // Check if we are exiting.
            if (state->destroyRequested != 0)
                return;
        }

        // A timeout has occurred during waiting for events.
        // Here you can process your periodic activities, e.g. call your redraw routine.

		if( g_bAppIsInitialized && state->window && !IntegrationDemo::Instance().IsGamePaused())
        {			
			ANativeWindow_Buffer buffer;
			int32_t lock_result = ANativeWindow_lock( state->window, &buffer, NULL);

			IntegrationDemo::Instance().StartFrame();

			if ( ! IntegrationDemo::Instance().Update() )
			{
				LOGI("Application term request event loop ");
				ANativeActivity_finish(state->activity);
			}

			IntegrationDemo::Instance().Render();
			if (lock_result == 0)
			{
				PostFrame( &buffer );
				ANativeWindow_unlockAndPost(state->window );
			}
			IntegrationDemo::Instance().EndFrame();

			// This need to be done on android since the softkeyboard
			// doesn't support button down and button up (both events are sent at the same time)
			IntegrationDemo::Instance().GetInputMgr()->Flush();
        }
    }
}
