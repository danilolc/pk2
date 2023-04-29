# Pekka Kana 2 by Janne Kivilahti from Piste Gamez (2003-2007)
# https://pistegamez.net/game_pk2.html
#
# Makefile command:
# "make" - Creates Pekka Kana 2 binary
# "make clean" - Removes all objects, executables and dependencies

# Compiler:
CXX = emcc

# Optimization:
CXXFLAGS += -O2
#CXXFLAGS += -O3

#CXXFLAGS += -march=native

#CXXFLAGS += -fno-exceptions -fno-rtti
#LDFLAGS +=

# Warnings:
CXXFLAGS += -Wno-unused-command-line-argument

# Standart:
CXXFLAGS += --std=c++17

# SDL2:
#CXXFLAGS += $(shell pkg-config sdl2 --cflags)
#LDFLAGS += $(shell pkg-config sdl2 --libs) -lSDL2_mixer -lSDL2_image
CXXFLAGS += -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_MIXER=2 -sFORCE_FILESYSTEM 
CXXFLAGS += --preload-file res/ -sSDL2_IMAGE_FORMATS='["png", "bmp"]' -sUSE_MODPLUG=1
CXXFLAGS += -sSDL2_MIXER_FORMATS='["xm", "mod"]'
LDFLAGS += $(CXXFLAGS)

# LibZip (read episodes on zip files):
#CXXFLAGS += -DPK2_USE_ZIP $(shell pkg-config libzip --cflags)
#LDFLAGS += $(shell pkg-config libzip --libs)

# Portable (data is stored with resorces):
#CXXFLAGS += -DPK2_PORTABLE

# Commit hash
CXXFLAGS += -DCOMMIT_HASH='"$(shell git rev-parse --short HEAD)"'

# Directories:
SRC_DIR = src/
BIN_DIR = bin/
BUILD_DIR = build/

# Source files:
PK2_SRC  = *.cpp */*.cpp */*/*.cpp
PK2_SRC := $(addprefix $(SRC_DIR), $(PK2_SRC))
PK2_SRC := $(wildcard $(PK2_SRC))

# Object files:
PK2_OBJ := $(basename $(PK2_SRC))
PK2_OBJ := $(subst $(SRC_DIR), ,$(PK2_OBJ))
PK2_OBJ := $(addsuffix .o, $(PK2_OBJ))
PK2_OBJ := $(addprefix $(BUILD_DIR), $(PK2_OBJ))

# Dependency files:
DEPENDENCIES := $(PK2_OBJ)
DEPENDENCIES := $(basename $(DEPENDENCIES))
DEPENDENCIES := $(addsuffix .d, $(DEPENDENCIES))

# Binary output:
PK2_BIN = $(BIN_DIR)pekka-kana-2.html

pk2: $(PK2_BIN)

###########################
$(PK2_BIN): $(PK2_OBJ)
	@echo -Linking Pekka Kana 2
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $^ $(LDFLAGS) -o $@
###########################

###########################
-include $(DEPENDENCIES)

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -Compiling $<
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) -MM -MT $@ -I$(SRC_DIR) $< > $(BUILD_DIR)$*.d
###########################

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: pk2 clean
