cmake_minimum_required(VERSION 3.4.1)



add_library(
            mikmod
            SHARED
            IMPORTED)
set_target_properties(mikmod PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libmikmod.so)
add_library(
            smpeg2
            SHARED
            IMPORTED)
set_target_properties(smpeg2 PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libsmpeg2.so)
add_library(
            SDL2
            SHARED
            IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libSDL2.so)
add_library(
            SDL2main
            SHARED
            IMPORTED)
set_target_properties(SDL2main PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libSDL2main.so)
add_library(
            SDL2_image
            SHARED
            IMPORTED)
set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libSDL2_image.so)
add_library(
            SDL2_mixer
            SHARED
            IMPORTED)
set_target_properties(SDL2_mixer PROPERTIES IMPORTED_LOCATION
            ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}/libSDL2_mixer.so)

find_library(
             log-lib
             log)

add_definitions("-Wno-write-strings")

include_directories("../../engine")
include_directories("../../src")
include_directories("../../../../../../opt/android-sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/include/")
include_directories("../sdl/include")



set(
        ENGINE_SRC
        ../../engine/PisteEngine.cpp
        ../../engine/PisteDraw.cpp
        ../../engine/PisteFont.cpp
        ../../engine/PisteInput.cpp
        ../../engine/PisteLanguage.cpp
        ../../engine/PisteSound.cpp
        ../../engine/PisteUtils.cpp
    )
#add_library(
#             piste_engine
#             SHARED
#             ${ENGINE_SRC})

#set(
#        PK2_SRC
#        ../../src/pk2.cpp
#        ../../src/map.cpp
#        ../../src/sprite.cpp
#    )

#add_library(
#             pk2
#             SHARED
#             ${PK2_SRC})

add_library(
        ENGINE_TEST
        SHARED
        ../../src/enginetest.cpp)

target_link_libraries( ENGINE_TEST

                       mikmod
                       smpeg2
                       SDL2
                       SDL2main
                       SDL2_mixer
                       SDL2_image
                       #piste_engine
                       ${log-lib})