LOCAL_PATH := $(call my-dir)

sdl_libs=$(LOCAL_PATH)/SDL2/build/android/lib

ENG_DIR := $(LOCAL_PATH)/../../../engine
SRC_DIR := $(LOCAL_PATH)/../../../src

include $(CLEAR_VARS)
LOCAL_MODULE := hidapi
LOCAL_SRC_FILES := $(sdl_libs)/$(TARGET_ARCH_ABI)/libhidapi.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := SDL2
LOCAL_SRC_FILES := $(sdl_libs)/$(TARGET_ARCH_ABI)/libSDL2.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_image
LOCAL_SRC_FILES := $(sdl_libs)/$(TARGET_ARCH_ABI)/libSDL2_image.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := mpg123
LOCAL_SRC_FILES := $(sdl_libs)/$(TARGET_ARCH_ABI)/libmpg123.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := SDL2_mixer
LOCAL_SRC_FILES := $(sdl_libs)/$(TARGET_ARCH_ABI)/libSDL2_mixer.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := PisteEngine
LOCAL_C_INCLUDES := $(LOCAL_PATH)/SDL2/include $(LOCAL_PATH)/SDL2_image $(LOCAL_PATH)/SDL2_mixer
LOCAL_SRC_FILES := $(wildcard $(ENG_DIR)/*.cpp)
LOCAL_CPPFLAGS += -std=c++17 -Wno-write-strings
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer hidapi mpg123
include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := PK2
LOCAL_C_INCLUDES := $(ENG_DIR) $(SRC_DIR)
LOCAL_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
LOCAL_CPPFLAGS += -std=gnu++17 -Wno-write-strings
LOCAL_STATIC_LIBRARIES := PisteEngine
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
