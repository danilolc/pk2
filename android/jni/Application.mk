APP_ABI := armeabi-v7a #arm64-v8a x86 x86_64
APP_STL := gnustl_static
APP_PLATFORM := android-14
APP_MODULES := SDL2 SDL2_image SDL2_mixer PisteEngine PK2
APP_BUILD_SCRIPT := Android.mk
APP_CPPFLAGS += -std=c++11 -Wno-write-strings -O2
