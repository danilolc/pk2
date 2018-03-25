#!/bin/bash
#export ANDROID_SDK=/opt/android-sdk
#export ANDROID_NDK=$ANDROID_SDK/ndk-bundle

BASEDIR=$(dirname "$0")
cd $BASEDIR

TYPE="debug"

for var in "$@"
do
    if [ "$var" == "release" ]
	then
		TYPE="release"
	fi
done

echo Building $TYPE
$ANDROID_NDK/ndk-build
retval=$?
if [ "$retval" == 0 ]
then
	echo "Libraries OK"
else
	exit $retval
fi

arg=
if [ "$TYPE" == "debug" ]
then
	arg=-d
fi

./gradlew build $arg
retval=$?
if [ "$retval" == 0 ]
then
	echo "APK Ok"
else
	exit $retval
fi

mv build/outputs/apk/$TYPE/android-$TYPE.apk build/outputs/apk/$TYPE/pk2-$TYPE.apk

$ANDROID_SDK/platform-tools/adb install -r build/outputs/apk/$TYPE/pk2-$TYPE.apk
if [ "$retval" == 0 ]
then
	echo "Installed Ok"
else
	exit $retval
fi


$ANDROID_SDK/platform-tools/adb shell monkey -p org.pgnapps.pk2 -c android.intent.category.LAUNCHER 1
