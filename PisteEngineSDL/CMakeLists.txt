cmake_minimum_required(VERSION 2.6)

project(PisteEngineSDL)

add_definitions("-Wno-write-strings -g")
include_directories("../PisteLanguage/")

set(SOURCES
Game.cpp
PisteDraw.cpp
PisteFont.cpp
PisteSFX.cpp
PisteSound.cpp
PisteInput.cpp
PisteLog.cpp
PisteWait.cpp
../PisteLanguage/PisteLanguage.cpp
)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

add_library(piste STATIC ${SOURCES})

link_libraries (
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
  ${SDLMIXER_LIBRARY}
	SDL_gfx
)

add_executable(Game Game.cpp)
target_link_libraries(Game piste)

