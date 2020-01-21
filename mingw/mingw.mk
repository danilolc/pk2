#
#
#

OPT = -O2

CXX = $(PLAT)-w64-mingw32-g++
BIN_SRC = /usr/local/$(PLAT)-w64-mingw32/bin/

SDL_CONF = $(BIN_SRC)/sdl2-config

CXXFLAGS += $(shell $(SDL_CONF) --cflags) $(OPT) -std=gnu++17 -Wall -Wno-sign-compare -DNO_ZIP
LDFLAGS += -static-libgcc -static-libstdc++ $(shell $(SDL_CONF) --libs) -lSDL2_mixer -lSDL2_image

SRC_DIR = ../../src/
RES_DIR = ../../res/
BIN_DIR = PK2/
BUILD_DIR = build/

RC_FILE = ../app.rc
RES_FILE = $(BUILD_DIR)app.res

PK2_SRC  = $(wildcard $(SRC_DIR)*.cpp) $(wildcard $(SRC_DIR)*/*.cpp)
PK2_OBJ := $(basename $(PK2_SRC))
PK2_OBJ := $(subst $(SRC_DIR), ,$(PK2_OBJ))
PK2_OBJ := $(addsuffix .o, $(PK2_OBJ))
PK2_OBJ := $(addprefix $(BUILD_DIR), $(PK2_OBJ))

DEPENDENCIES := $(PK2_OBJ)
DEPENDENCIES := $(basename $(DEPENDENCIES))
DEPENDENCIES := $(addsuffix .d, $(DEPENDENCIES))

PK2_BIN = $(BIN_DIR)PK2.exe

pk2: $(PK2_BIN) copyfiles

###########################
$(PK2_BIN): $(PK2_OBJ)
	@echo -Linking Pekka Kana 2
	@mkdir -p $(dir $@) >/dev/null
	@$(PLAT)-w64-mingw32-windres $(RC_FILE) -O coff -o $(RES_FILE)
	@$(CXX) $^ $(RES_FILE) $(LDFLAGS) -o $@
###########################

###########################
-include $(DEPENDENCIES)

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	@echo -Compiling $<
	@mkdir -p $(dir $@) >/dev/null
	@$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -o $@ -c $<
	@$(CXX) -MM -MT $@ -I$(SRC_DIR) $< > $(BUILD_DIR)$*.d
###########################

clean:
	@rm -rf $(BIN_DIR)
	@rm -rf $(BUILD_DIR)

copyfiles:
	@echo -Copying libraries
	@mkdir -p $(BIN_DIR) >/dev/null
	
	#@cp $(BIN_SRC)libFLAC-8.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.FLAC.txt $(BIN_DIR)

	@cp $(BIN_SRC)libogg-0.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.ogg-vorbis.txt $(BIN_DIR)
	
	#@cp $(BIN_SRC)libjpeg-9.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.jpeg.txt $(BIN_DIR)
	
	#@cp $(BIN_SRC)libopus-0.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.opus.txt $(BIN_DIR)

	#@cp $(BIN_SRC)libopusfile-0.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.opusfile.txt $(BIN_DIR)

	#@cp $(BIN_SRC)libvorbis-0.dll $(BIN_DIR)

	#@cp $(BIN_SRC)libvorbisfile-3.dll $(BIN_DIR)

	@cp $(BIN_SRC)libmodplug-1.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.modplug.txt $(BIN_DIR)

	@cp $(BIN_SRC)libmpg123-0.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.mpg123.txt $(BIN_DIR)

	@cp $(BIN_SRC)libpng16-16.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.png.txt $(BIN_DIR)

	#@cp $(BIN_SRC)libtiff-5.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.tiff.txt $(BIN_DIR)

	#@cp $(BIN_SRC)libwebp-7.dll $(BIN_DIR)
	#@cp $(BIN_SRC)LICENSE.webp.txt $(BIN_DIR)

	@cp $(BIN_SRC)zlib1.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.zlib.txt $(BIN_DIR)

	@cp $(BIN_SRC)SDL2.dll $(BIN_DIR)
	@cp $(BIN_SRC)SDL2_image.dll $(BIN_DIR)
	@cp $(BIN_SRC)SDL2_mixer.dll $(BIN_DIR)

	@echo -Copying resources
	@cp -r $(RES_DIR)/* $(BIN_DIR)


.PHONY: pk2 clean copyfiles