//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @file open_browser.cpp
 * Opening browser utility by Saturnin 
 */
#include "open_browser.hpp"
#include "engine/PLog.hpp"

const char* URL_DISCORD = "https://discord.gg/kqDJfYX";
const char* URL_GITHUB= "https://github.com/SaturninTheAlien/pk2_greta";
const char* URL_PROBOARDS ="https://pistegamez.proboards.com";
const char* URL_PISTEGAMEZ = "https://pistegamez.net";
const char* URL_MAKYUNI = "https://sites.google.com/view/pekka-kana-fanpage/pekka-kana-2";

#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __ANDROID__
#include <jni.h>
#include <SDL.h>
#endif


void OpenBrowser(const std::string& url){
    
    #ifndef __ANDROID__

    std::ostringstream command;

    #if defined(_WIN32)
    command<<"start ";
    #elif defined(__APPLE__)
    command<<"open ";
    #else
    command<<"xdg-open ";
    #endif
    command<<url;

    int ret = system(command.str().c_str());
    if(ret!=0){
        PLog::Write(PLog::WARN, "Cannot open a browser, code: %i", ret);
    }

    #else //Android

    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));
    jmethodID method_id = env->GetMethodID(clazz, "openBrowser", "(Ljava/lang/String;)V");
    if(method_id==0){
        throw std::runtime_error("JNI: Method \"openBrowser\" not found!");
    }

    jstring param1 = env->NewStringUTF(url.c_str());
    env->CallObjectMethod(activity, method_id, param1);
    env->DeleteLocalRef(param1);

    #endif
}