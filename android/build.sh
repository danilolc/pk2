ndk-build \
  NDK_PROJECT_PATH=null \
  NDK_OUT=$obj \
  NDK_LIBS_OUT=$lib \
  APP_BUILD_SCRIPT=Android.mk \
  APP_ABI="armeabi-v7a arm64-v8a x86 x86_64" \
  APP_PLATFORM=android-14 \
  APP_MODULES="SDL2 SDL2_main SDL2_image SDL2_mixer PK2" \
  $ndk_args

#keystore
#alias: pekka_kana
#pass: pekkakana
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore key.keystore build/outputs/apk/pk2_android-debug-unsigned.apk pekka_kana
mv build/outputs/apk/pk2_android-debug-unsigned.apk build/outputs/apk/pk2_android-debug.apk
