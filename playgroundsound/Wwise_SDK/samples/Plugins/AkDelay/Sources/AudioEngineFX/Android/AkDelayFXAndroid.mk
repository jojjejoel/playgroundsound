LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := AkDelayFX
LOCAL_MODULE_FILENAME := libAkDelayFX

LOCAL_SRC_FILES := ../AkDelayFX.cpp ../AkDelayFXDSP.cpp ../AkDelayFXParams.cpp

ifeq ($(PM5_CONFIG),debug_android_armeabi-v7a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -D_DEBUG -mfloat-abi=softfp -mfpu=neon -ffast-math -fvisibility=hidden -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -D_DEBUG -ffast-math -fvisibility=hidden -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_arm64-v8a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -D_DEBUG -ffast-math -fvisibility=hidden -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),debug_android_x86_64)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -D_DEBUG -ffast-math -fvisibility=hidden -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_android_armeabi-v7a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -mfloat-abi=softfp -mfpu=neon -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_android_x86)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_android_arm64-v8a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_android_x86_64)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_enableasserts_android_armeabi-v7a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_ENABLE_ASSERTS -mfloat-abi=softfp -mfpu=neon -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_enableasserts_android_x86)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_ENABLE_ASSERTS -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_enableasserts_android_arm64-v8a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_ENABLE_ASSERTS -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),profile_enableasserts_android_x86_64)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_ENABLE_ASSERTS -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -fno-omit-frame-pointer -g -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  cmd-strip :=
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_armeabi-v7a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_OPTIMIZED -mfloat-abi=softfp -mfpu=neon -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_x86)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_OPTIMIZED -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_arm64-v8a)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_OPTIMIZED -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_STATIC_LIBRARY)
endif

ifeq ($(PM5_CONFIG),release_android_x86_64)
  LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. $(LOCAL_PATH)/. $(LOCAL_PATH)/../../../../../../include $(LOCAL_PATH)/../../Common/Android $(LOCAL_PATH)/../../Common $(LOCAL_PATH)/../../../Common
  LOCAL_CFLAGS := -Werror -DNDEBUG -DAK_OPTIMIZED -ffast-math -fvisibility=hidden -ffunction-sections -fdata-sections -O3
  LOCAL_CPPFLAGS := -Wno-conversion-null -Wno-deprecated-declarations -fvisibility=hidden -fno-rtti -fno-exceptions -std=c++14
  LOCAL_CONLYFLAGS := -std=c11
  include $(BUILD_STATIC_LIBRARY)
endif


all: $(LOCAL_INSTALLED)