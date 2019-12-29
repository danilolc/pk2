# Pekka Kana 2 by Janne Kivilahti from Piste Gamez (2003-2007)
# https://pistegamez.net/game_pk2.html
#
# This public release and rewritten is governed by a BSD-2-clause license.
#
# Makefile command:
# "make" - Creates Pekka Kana 2 binary
# "make clean" - Removes all objects, executables and dependencies

OPT = -g
#OPT = -O3

CXX = g++
CXXFLAGS += $(shell pkg-config --cflags sdl2) $(OPT) -std=gnu++17
LDFLAGS += -lSDL2 -lSDL2_mixer -lSDL2_image

# Defines directories
SRC_DIR = src/
BIN_DIR = bin/
BUILD_DIR = build/

PK2_SRC  = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)*/*.cpp)
PK2_OBJ := $(basename $(PK2_SRC))
PK2_OBJ := $(addsuffix .o, $(PK2_OBJ))
PK2_OBJ := $(addprefix $(BUILD_DIR), $(PK2_OBJ))

# Defines the destination of each binary file
PK2_BIN = $(BIN_DIR)pekka-kana-2

DEPENDENCIES := $(PK2_OBJ)
DEPENDENCIES := $(basename $(DEPENDENCIES))
DEPENDENCIES := $(addsuffix .d, $(DEPENDENCIES))

pk2: $(PK2_BIN)

###########################
# Rules for generate the binaries using the object files
$(PK2_BIN) : $(PK2_OBJ)
	@echo -Linking Pekka Kana 2
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $^ $(LDFLAGS) -o $@
###########################

###########################
# Rules for generate any *.o file
-include $(DEPENDENCIES)

$(BUILD_DIR)%.o : %.cpp
	@echo -Compiling $<
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ -c $<
	@$(CXX) -MM -MT $@ -I$(SRC_DIR) $< > $(BUILD_DIR)$*.d
###########################

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: pk2 test clean
