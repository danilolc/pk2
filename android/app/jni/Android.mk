SRC_DIR := $(call my-dir)/../../../src

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)

LOCAL_MODULE := PK2
LOCAL_C_INCLUDES := $(SRC_DIR)
LOCAL_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
LOCAL_SRC_FILES += $(SRC_DIR)/engine/render/PSdl.cpp

LOCAL_CPP_FEATURES += exceptions
LOCAL_SHARED_LIBRARIES := LIBCPP SDL2 SDL2_image SDL2_mixer libzip lua
LOCAL_STL := c++_shared
LOCAL_CPPFLAGS += -std=c++17 -DPK2_USE_ZIP -DPK2_USE_LUA -DPK2_VERSION=\"v0.209\"
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
