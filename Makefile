#Make File
#-----------------
#"make" - Creates PK2 binary
#"make pk2le" - Creates PK2 Level Editor
#"make pk2sc" - Creates PK2 Sprite Creator
#"make clean" - Removes all objects, executables and dependencies
#-----------------

include config.txt

SDLD = /usr/include/SDL2/

.PHONY: pk2 pk2le pk2sc clean makedirs

pk2: makedirs $(PK2_BIN)

pk2le: makedirs $(PK2_LE_BIN)

pk2sc: makedirs $(PK2_SC_BIN)

###########################
#Rules for generate the binaries using the object files
$(PK2_BIN): $(PK2_OBJ) $(PK2_SPRITE_OBJ) $(PK2_MAP_OBJ) $(ENGINE_OBJ)
	@echo -Linking PK2
	@$(CPP) $^ $(LFLAGS) -o $@
	@cp $@ $(RES_DIR)$(GAME_NAME).exe

$(PK2_LE_BIN): $(PK2_LE_OBJ) $(PK2_SPRITE_OBJ) $(PK2_MAP_OBJ) $(ENGINE_OBJ)
	@echo -Linking PK2_LE
	@$(CPP) $^ $(LFLAGS) -o $@

#TODO - Don't compile with $(ENGINE_OBJ) and $(LFLAGS)
$(PK2_SC_BIN): $(PK2_SC_OBJ) $(PK2_SPRITE_OBJ) $(ENGINE_OBJ)
	@echo -Linking PK2_SC
	@$(CPP) $^ $(LFLAGS) -o $@
###########################

###########################
#Rules for generate any *.o file
-include $(DEPENDENCIES)

build/%.o : engine/%.cpp
	@echo -Some dependence of $@ was changed, updating
	@$(CPP) $(CFLAGS) -I$(SDLD) -I$(ENGINE_DIR) -o $@ -c $<
	@$(CPP) -MM -MT $@ -I$(SDLD) -I$(ENGINE_DIR) $< > $(BUILD_DIR)$*.d

build/%.o : src/%.cpp
	@echo -Some dependence of $@ was changed, updating
	@$(CPP) $(CFLAGS) -I$(SDLD) -I$(SRC_DIR) -I$(ENGINE_DIR) -o $@ -c $<
	@$(CPP) -MM -MT $@ -I$(SDLD) -I$(SRC_DIR) -I$(ENGINE_DIR) $< > $(BUILD_DIR)$*.d
###########################

clean:
	@rm -f -r $(BIN_DIR)
	@rm -f -r $(BUILD_DIR)

makedirs:
	@mkdir -p $(BIN_DIR) >/dev/null
	@mkdir -p $(BUILD_DIR) >/dev/null
