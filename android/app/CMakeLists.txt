cmake_minimum_required(VERSION 3.4.1)

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

set(
        PK2_SRC
        ../../src/pk2.cpp
        ../../src/map.cpp
        ../../src/sprite.cpp
    )

add_library(
             piste_engine
             SHARED
             ${ENGINE_SRC})

add_library(
             pk2
             SHARED
             ${PK2_SRC})






add_library(
            mikmod
            SHARED
            IMPORTED)
add_library(
            smpeg2
            SHARED
            IMPORTED)
add_library(
            SDL2
            SHARED
            IMPORTED)
add_library(
            SDL2_image
            SHARED
            IMPORTED)
add_library(
            SDL2_mixer
            SHARED
            IMPORTED)

find_library(
             log-lib
             log)

target_link_libraries( pk2
                       piste_engine
                       mikmod
                       smpeg2
                       SDL2
                       SDL2_mixer
                       SDL2_image
                       ${log-lib})