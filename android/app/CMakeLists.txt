cmake_minimum_required(VERSION 3.4.1)

add_definitions("-Wno-write-strings")

include_directories("../../engine ../../src")

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

#add_library(
#             sdl2
#             SHARED
#             src/main/cpp/native-lib.cpp)

add_library(
             pk2
             SHARED
             ${PK2_SRC})

find_library(
             log-lib
             log)

target_link_libraries( pk2
                       piste_engine
                       ${log-lib})