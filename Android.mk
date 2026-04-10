SRC_DIR := $(call my-dir)/src

include $(CLEAR_VARS)
LOCAL_MODULE := PK2

LOCAL_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
LOCAL_SRC_FILES += $(SRC_DIR)/engine/render/PSdl.cpp


# TODO
# Make these paths portable

LOCAL_C_INCLUDES := $(SRC_DIR) \
       /home/saturnin/android/SDK/SDL2_image-2.8.10/include \
       /home/saturnin/android/SDK/SDL2-2.32.10/include \
       /home/saturnin/android/SDK/SDL2_mixer-2.8.1/include \
       /home/saturnin/android/libzip/$(APP_ABI)/include \
       $(PROJECT_ROOT)/lua/src

LOCAL_CPP_FEATURES += exceptions
LOCAL_SHARED_LIBRARIES := LIBCPP SDL2 SDL2_image SDL2_mixer libzip lua
LOCAL_STL := c++_shared
LOCAL_CPPFLAGS += -std=c++17 -DPK2_USE_ZIP -DPK2_USE_LUA -DPK2_VERSION=\"1.5.1 Greta\"
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)