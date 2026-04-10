# Pekka Kana 2 by Janne Kivilahti from Piste Gamez (2003-2007)
# and the Piste Gamez community.
#
# https://pistegamez.net/game_pk2.html
#
# Makefile command:
# "make" - creates Pekka Kana 2 binary
# "make clean" - removes all objects, executables and dependencies

# "sudo make install" - installs PK2 into Linux/Unix FHS
# "sudo make uninstall" - removes PK2 from Linux/Unix FHS

INSTALL_BIN_DIR = /usr/local/games/
INSTALL_RES_DIR = /usr/local/share/games/pekka-kana-2/


INSTALL_APP_SHORTCUT_DIR = /usr/local/share/applications/
INSTALL_APP_ICON_DIR = /usr/local/share/icons/hicolor/64x64/apps/


# Compiler:
CXX = c++

# Optimization:
ifdef DEBUG
$(info ->Debugging symbols enabled) 
    CXXFLAGS += -g
else
$(info ->Release mode)
    CXXFLAGS += -O3
endif

# Warnings:
CXXFLAGS += -Wall

# Standart:
CXXFLAGS += --std=c++17 -fPIC


# SDL2, libzip and lua
CXXFLAGS += -DPK2_USE_ZIP -DPK2_USE_LUA $(shell pkg-config sdl2 libzip lua --cflags)
LDFLAGS += $(shell pkg-config sdl2 libzip lua --libs) -lSDL2_mixer -lSDL2_image

# # Version string
# PK2_VERSION = $(shell git log -1 --pretty=format:"%s" | grep -o 'v[0-9]\+\.[0-9]\+')
# ifeq ($(PK2_VERSION),)
# 	PK2_VERSION = "Unknown_version"
# endif

PK2_VERSION = "1.5.1 Greta"

# Portable (data is stored with resorces):
CXXFLAGS += -DPK2_PORTABLE -DPK2_VERSION=\"$(PK2_VERSION)\"

# Commit hash
# CXXFLAGS += -DCOMMIT_HASH='"$(shell git rev-parse --short HEAD)"'

#Compile command CXX and CXXFLAGS
COMPILE_COMMAND = $(CXX) $(CXXFLAGS)

# Directories:
SRC_DIR = src/
BIN_DIR = bin/
BUILD_DIR = build/
RES_DIR = res/

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
PK2_BIN = $(BIN_DIR)pekka-kana-2
all: pk2

pk2: $(PK2_BIN)

###########################
$(PK2_BIN): $(PK2_OBJ)
	@echo -Linking Pekka Kana 2
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $^ $(LDFLAGS) -o $@


###########################
-include $(DEPENDENCIES)

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -Compiling $<
	@mkdir -p $(dir $@) >/dev/null
	@$(COMPILE_COMMAND) -I$(SRC_DIR) -o $@ -c $<
	@$(COMPILE_COMMAND) -MM -MT $@ -I$(SRC_DIR) $< > $(BUILD_DIR)$*.d
###########################

install:
	install -d $(INSTALL_BIN_DIR)
	install -m 755 $(PK2_BIN) $(INSTALL_BIN_DIR)pekka-kana-2

	install -d $(INSTALL_RES_DIR)
	rsync -a --exclude='data' --chmod=F644,D755 $(RES_DIR) $(INSTALL_RES_DIR)

	install -d $(INSTALL_APP_ICON_DIR)
	install -m 644  misc/icon_64x64.png $(INSTALL_APP_ICON_DIR)pekka-kana-2.png

	install -d $(INSTALL_APP_SHORTCUT_DIR)
	install -m 644  misc/linux/pk2_local.desktop $(INSTALL_APP_SHORTCUT_DIR)pekka-kana-2.desktop


uninstall:
	rm $(INSTALL_BIN_DIR)pekka-kana-2
	rm -r $(INSTALL_RES_DIR)
	rm $(INSTALL_APP_ICON_DIR)pekka-kana-2.png
	rm $(INSTALL_APP_SHORTCUT_DIR)pekka-kana-2.desktop

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

test:
	@echo $(CXXFLAGS)

.PHONY: pk2 clean all test install uninstall
