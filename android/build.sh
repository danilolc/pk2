#!/bin/bash

LASTDIR=$(cwd)
BASEDIR=$(dirname "$0")
cd $BASEDIR
#export ANDROID_SDK=/opt/android-sdk
#export ANDROID_NDK=$ANDROID_SDK/ndk-bundle

$ANDROID_NDK/ndk-build
retval=$?
if [ "$retval" == 0 ]
then
	echo "Libraries OK"
else
	exit $retval
fi



./gradlew build
retval=$?
if [ "$retval" == 0 ]
then
	echo "APK Ok"
else
	exit $retval
fi



jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -storepass pekkakana -keypass pekkakana -keystore key.keystore build/outputs/apk/android-debug-unsigned.apk pekka_kana
retval=$?
if [ "$retval" == 0 ]
then
	echo "Signed Ok"
else
	exit $retval
fi
mv build/outputs/apk/android-debug-unsigned.apk build/outputs/apk/pk2-debug.apk



$ANDROID_SDK/platform-tools/adb install -r build/outputs/apk/pk2-debug.apk
if [ "$retval" == 0 ]
then
	echo "Installed Ok"
else
	exit $retval
fi


$ANDROID_SDK/platform-tools/adb shell monkey -p org.pgnapps.pk2 -c android.intent.category.LAUNCHER 1

cd $LASTDIR
