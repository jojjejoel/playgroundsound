workspace_INTEGRATIONDEMO_ANDROID_PATH := $(call my-dir)

include $(workspace_INTEGRATIONDEMO_ANDROID_PATH)/../IntegrationDemo.mk

ifeq ($(PM5_CONFIG),debug_android_arm64-v8a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),debug_android_armeabi-v7a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),debug_android_x86)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),debug_android_x86_64)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),profile_android_arm64-v8a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),profile_android_armeabi-v7a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),profile_android_x86)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),profile_android_x86_64)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),release_android_arm64-v8a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),release_android_armeabi-v7a)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),release_android_x86)
  $(call import-module,android/native_app_glue)
endif

ifeq ($(PM5_CONFIG),release_android_x86_64)
  $(call import-module,android/native_app_glue)
endif