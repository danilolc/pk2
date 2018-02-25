ndk-build



#keystore
#alias: pekka_kana
#pass: pekkakana
jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore key.keystore build/outputs/apk/pk2_android-debug-unsigned.apk pekka_kana
mv build/outputs/apk/pk2_android-debug-unsigned.apk build/outputs/apk/pk2_android-debug.apk
