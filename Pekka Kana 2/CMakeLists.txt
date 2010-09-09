cmake_minimum_required(VERSION 2.6)

project(PK2)

add_definitions("-Wno-write-strings -g")
include_directories(../PisteEngineSDL/ ../PisteLanguage/)
link_directories(../PisteEngineSDL/)

set(SOURCES
PK2.cpp
PK2Map.cpp
PK2Sprite.cpp
)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

link_libraries (
	${SDL_LIBRARY}
	${SDLIMAGE_LIBRARY}
  ${SDLMIXER_LIBRARY}
	SDL_gfx
)

add_executable(PK2 ${SOURCES})
target_link_libraries(PK2 piste)

