workspace_SAMPLEPLUGINSANDROID_PATH := $(call my-dir)

include $(workspace_SAMPLEPLUGINSANDROID_PATH)/AkAudioInput/Sources/AudioEngineFX/Android/AkAudioInputSourceAndroid.mk
include $(workspace_SAMPLEPLUGINSANDROID_PATH)/AkDelay/Sources/AudioEngineFX/Android/AkDelayFXAndroid.mk
include $(workspace_SAMPLEPLUGINSANDROID_PATH)/AkSineTone/Sources/AudioEngineFX/Android/AkSineSourceAndroid.mk
include $(workspace_SAMPLEPLUGINSANDROID_PATH)/AkToneGenerator/Sources/AudioEngineFX/Android/AkToneSourceAndroid.mk