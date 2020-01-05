# Pekka Kana 2 by Janne Kivilahti from Piste Gamez (2003-2007)
# https://pistegamez.net/game_pk2.html
#
# Makefile command:
# "make" - Creates Pekka Kana 2 binary
# "make clean" - Removes all objects, executables and dependencies

OPT = -g
#OPT = -O3

#CXX = g++
#CXXFLAGS += $(shell pkg-config sdl2 --cflags) $(shell pkg-config libzip --cflags) $(OPT) -std=gnu++17 -w
#LDFLAGS += $(shell pkg-config sdl2 --libs) $(shell pkg-config libzip --libs) -lSDL2_mixer -lSDL2_image

CXX = g++
CXXFLAGS += $(shell pkg-config sdl2 --cflags) $(OPT) -std=gnu++17 -Wall -Wno-sign-compare
LDFLAGS += $(shell pkg-config sdl2 --libs) -lSDL2_mixer -lSDL2_image

SRC_DIR = src/
BIN_DIR = bin/
BUILD_DIR = build/

PK2_SRC  = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)*/*.cpp)
PK2_OBJ := $(basename $(PK2_SRC))
PK2_OBJ := $(addsuffix .o, $(PK2_OBJ))
PK2_OBJ := $(addprefix $(BUILD_DIR), $(PK2_OBJ))

DEPENDENCIES := $(PK2_OBJ)
DEPENDENCIES := $(basename $(DEPENDENCIES))
DEPENDENCIES := $(addsuffix .d, $(DEPENDENCIES))

PK2_BIN = $(BIN_DIR)pekka-kana-2

pk2: $(PK2_BIN)

###########################
$(PK2_BIN): $(PK2_OBJ)
	@echo -Linking Pekka Kana 2
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $^ $(LDFLAGS) -o $@
###########################

###########################
-include $(DEPENDENCIES)

$(BUILD_DIR)%.o: %.cpp
	@echo -Compiling $<
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ -c $<
	@$(CXX) -MM -MT $@ -I$(SRC_DIR) $< > $(BUILD_DIR)$*.d
###########################

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: pk2 clean
