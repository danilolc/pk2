# This is the mingw makefile
# It must be included by a Makefile on a directory here
#
# It assumes that you have defined:
#
# CXX (the mingw c++ compiler)
# WINRES (the winres)
# BIN_SRC (the binary source to be copyed - see the copyfiles rule)
# INC_SRC (the include with SDL2 and Zip)
# LIB_SRC (the libs with SDL2 and Zip)

#Remove -DUSE_ZIP if you don't want or don't have zip
CXXFLAGS += -I$(INC_SRC) -I$(INC_SRC)SDL2/ -Dmain=SDL_main $(OPT) -std=gnu++17 -Wall -DPORTABLE -DUSE_ZIP
LDFLAGS += -static-libgcc -L$(LIB_SRC) -lmingw32 -mwindows -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_image -lzip -Wl,-Bstatic -lstdc++ -lpthread

SRC_DIR = ../../src/
RES_DIR = ../../res/
BIN_DIR = PK2/
LICENSES = $(BIN_DIR)licenses/
BUILD_DIR = build/

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
	@$(WINRES) ../version.rc  -O coff -o $(BUILD_DIR)version.res
	@$(WINRES) ../icon.rc  -O coff -o $(BUILD_DIR)icon.res
	@$(CXX) $^ $(BUILD_DIR)version.res $(BUILD_DIR)icon.res $(LDFLAGS) -o $@
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
	@mkdir -p $(LICENSES) >/dev/null

	@cp $(BIN_SRC)libogg-0.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.ogg-vorbis.txt $(LICENSES)

	@cp $(BIN_SRC)libmodplug-1.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.modplug.txt $(LICENSES)

	@cp $(BIN_SRC)libmpg123-0.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.mpg123.txt $(LICENSES)

	@cp $(BIN_SRC)libpng16-16.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.png.txt $(LICENSES)

	@cp $(BIN_SRC)zlib1.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.zlib.txt $(LICENSES)

	@cp $(BIN_SRC)libzip.dll $(BIN_DIR)
	@cp $(BIN_SRC)LICENSE.libzip.txt $(LICENSES)
	
	@cp $(BIN_SRC)SDL2.dll $(BIN_DIR)
	@cp $(BIN_SRC)SDL2_image.dll $(BIN_DIR)
	@cp $(BIN_SRC)SDL2_mixer.dll $(BIN_DIR)

	@echo -Copying resources
	@cp -r $(RES_DIR)/* $(BIN_DIR)


.PHONY: pk2 clean copyfiles