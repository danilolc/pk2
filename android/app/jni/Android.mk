PK2_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)
SRC_DIR := $(PK2_PATH)/../../../src
LOCAL_MODULE := PK2
LOCAL_C_INCLUDES := $(SRC_DIR)
LOCAL_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
LOCAL_CPPFLAGS += -std=gnu++17
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer hidapi mpg123
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog
include $(BUILD_SHARED_LIBRARY)
