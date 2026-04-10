//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "levelclass.hpp"
#include "exceptions.hpp"
#include <sstream>

#include "system.hpp"

#include "engine/PDraw.hpp"
#include "engine/PLog.hpp"
#include "engine/PJson.hpp"
#include "engine/PFilesystem.hpp"

#include <cinttypes>
#include <cstring>
#include <cmath>
#include <array>


#define PK2MAP_MAP_WIDTH  256
#define PK2MAP_MAP_HEIGHT 224
#define PK2MAP_MAP_SIZE   PK2MAP_MAP_WIDTH * PK2MAP_MAP_HEIGHT
#define PK2MAP_MAP_MAX_PROTOTYPES 100


void LevelClass::setTilesAnimations(int degree, int anim, u32 aika1, u32 aika2, u32 aika3) {

	arrows_block_degree = degree;
	block_animation_frame = anim;
	button1_timer = aika1;
	button2_timer = aika2;
	button3_timer = aika3;

}

LevelClass::LevelClass(){

}

void LevelClass::clearSectors(){
	for(LevelSector*& sector:this->sectors){
		if(sector!=nullptr){
			delete sector;
			sector = nullptr;
		}
	}
	this->sectors.clear();
}

void LevelClass::clear(){

	this->clearSectors();	

	for(Tileset*& tileset:this->mTilesets){
		if(tileset!=nullptr){
			delete tileset;
			tileset = nullptr;
		}
	}
	this->mTilesets.clear();

	for(Background*& background: this->mBackgrounds){
		if(background!=nullptr){
			delete background;
			background = nullptr;
		}
	}
	this->mBackgrounds.clear();

	for(std::pair<int, std::string>&p : this->mGfxTextures){
		PDraw::image_delete(p.first);
	}

	this->mGfxTextures.clear();
}

void LevelClass::load(PFile::Path path, bool headerOnly) {
	try{
		char version[5];
		PFile::RW file = path.GetRW2("r");

		file.read(version, 4);
		version[4] = '\0';

		file.close();

		PLog::Write(PLog::DEBUG, "PK2", "Loading %s, version %s", path.c_str(), version);

		/**
		 * @brief 
		 * New GE level format.
		 * 1.4 intentionally skipped as it was used in PK2 Community Edition
		 */
		if(strcmp(version, "1.5")==0){
			this->loadVersion15(path, headerOnly);
		}
		/**
		 * @brief 
		 * Legacy PK2 level format.
		 */
		else if (strcmp(version,"1.3")==0) {
			this->loadVersion13(path, headerOnly);
		}

		else if (strcmp(version,"1.4")==0 ){
			/**
			 * @brief 
			 * TO DO
			 * Support PK2 Community Edition level format
			 */

			throw PExcept::PException("PK2 CE levels not supported yet!");
		}

		else{
			std::ostringstream os;
			os<<"Unsupported level format: \""<<version<<"\"";
			throw PExcept::PException(os.str());
		}

	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::ERR,"PK2",e.what());
		throw PExcept::FileNotFoundException(path.c_str(), PExcept::MISSING_LEVEL);
	}

	for(LevelSector*sector: this->sectors){
		sector->calculateColors();
		sector->calculateEdges();
	}

	//TODO - redesign it!
	this->calculateBlockTypes();
}


void LevelClass::readTiles(PFile::RW& file,
        u8 compression,
        u32 level_width,
        std::size_t level_size,
        u8* tiles){

	switch (compression)
	{
	case TILES_COMPRESSION_NONE:{
		file.read(tiles, level_size);
	}
	break;
	case TILES_OFFSET_LEGACY:{
		u32 width = 0, height = 0;
		u32 offset_x = 0, offset_y = 0;

		file.readLegacyStrU32(offset_x);
		file.readLegacyStrU32(offset_y);
		file.readLegacyStrU32(width);
		file.readLegacyStrU32(height);

		for (u32 y = offset_y; y <= offset_y + height; y++) {
			u32 x_start = offset_x + y * level_width;
			/**
			 * @brief 
			 * To prevent a memory leak
			 */
			if(x_start>=0 && x_start + width < level_size){
				file.read(&tiles[x_start], width + 1);
			}
			else{
				throw std::runtime_error("Malformed level file!");
			}
		}
	}
	break;

	case TILES_OFFSET_NEW:{
		u32 width = 0, height = 0;
		u32 offset_x = 0, offset_y = 0;

		file.read(offset_x);
		file.read(offset_y);
		file.read(width);
		file.read(height);

		for (u32 y = offset_y; y <= offset_y + height; y++) {
			u32 x_start = offset_x + y * level_width;
			/**
			 * @brief 
			 * To prevent a memory leak
			 */
			if(x_start>=0 && x_start + width <level_size){
				file.read(&tiles[x_start], width + 1);
			}
			else{
				throw std::runtime_error("Malformed level file!");
			}
		}
	}
	break;

	default:
		std::ostringstream os;
		os<<"Tiles compression: "<<compression<<" not supported";
		throw PExcept::PException(os.str());
	}
}


static double getDefaultSlipperinessFactor(int weather){
	switch (weather)
	{
	case WEATHER_RAIN:
	case WEATHER_RAIN_LEAVES:
		return 1.03;

	case WEATHER_SNOW:
		return 1.01;
	
	default:
		break;
	}
	return 1.04;
}

void LevelClass::loadVersion13(PFile::Path path, bool headerOnly){
	PFile::RW file = path.GetRW2("r");
	file.read(version,      sizeof(version));

	std::string tileset_name, background_name, music_name;
	int weather = 0;
	int scrolling = 0;

	file.readLegacyStr13Chars(tileset_name);	
	file.readLegacyStr13Chars(background_name);
	file.readLegacyStr13Chars(music_name);


	char name_buffer[40];
	file.read(name_buffer, sizeof(name_buffer));
	name_buffer[39] = '\0';

	/**
	 * @brief 
	 * ???
	 * What's the purpose of this??
	 */
	for (int i = 38; i > 0 && (name_buffer[i] == (char)0xCD); i--)
		name_buffer[i] = 0;

	this->name = name_buffer;

	file.readLegacyStr40Chars(author);

	file.readLegacyStrInt(this->level_number);
	file.readLegacyStrInt(weather);
	file.readLegacyStrU32(this->button1_time);
	file.readLegacyStrU32(this->button2_time);
	file.readLegacyStrU32(this->button3_time);
	file.readLegacyStrInt(this->map_time);
	file.readLegacyStrInt(this->extra);
	file.readLegacyStrInt(scrolling);
	file.readLegacyStrInt(this->player_sprite_index);
	file.readLegacyStrInt(this->icon_x);
	file.readLegacyStrInt(this->icon_y);
	file.readLegacyStrInt(this->icon_id);

	/**
	 * @brief
	 * Loading only header for the map screen, it could stop now.
	 * There's no reason to load level tiles in that case.
	 */
	if(headerOnly){
		file.close();
		return;
	}

	LevelSector* sector = new LevelSector(PK2MAP_MAP_WIDTH, PK2MAP_MAP_HEIGHT);
	this->sectors.push_back(sector);

	sector->background = this->mLoadBackground(background_name);
	sector->tileset1 = this->mLoadTileset(tileset_name);
	sector->music_name = music_name;

	sector->background->scrolling = scrolling;
	sector->weather = weather;
	sector->slipperiness_factor = getDefaultSlipperinessFactor(weather);
	sector->name = "legacy sector";

	// sprite prototypes

	u32 prototypesNumber = 0;
	file.readLegacyStrU32(prototypesNumber);
	if(prototypesNumber>PK2MAP_MAP_MAX_PROTOTYPES){
		std::ostringstream os;
		os<<"Too many level sprite prototypes: "<<prototypesNumber<<std::endl;
		os<<PK2MAP_MAP_MAX_PROTOTYPES<<" is the limit in the \"1.3\" level format";
		file.close();
		throw std::runtime_error(os.str());
	}

	char prototype_names_legacy[PK2MAP_MAP_MAX_PROTOTYPES][13] = {""};
	
	file.read(prototype_names_legacy, sizeof(prototype_names_legacy[0]) * prototypesNumber);

	this->sprite_prototype_names.resize(prototypesNumber);
	for(u32 i=0;i<prototypesNumber;++i){
		char * prototype_name = prototype_names_legacy[i];
		prototype_name[12] = '\0';
		this->sprite_prototype_names[i] = prototype_name;
	}

	// background_tiles

	readTiles(file, TILES_OFFSET_LEGACY,
		PK2MAP_MAP_WIDTH,
		PK2MAP_MAP_SIZE,
		sector->background_tiles);

	// foreground_tiles
	readTiles(file, TILES_OFFSET_LEGACY,
		PK2MAP_MAP_WIDTH,
		PK2MAP_MAP_SIZE,
		sector->foreground_tiles);

	// sprite_tiles
	readTiles(file, TILES_OFFSET_LEGACY,
		PK2MAP_MAP_WIDTH,
		PK2MAP_MAP_SIZE,
		sector->sprite_tiles);
	

	file.close();
}


void LevelClass::loadVersion15(PFile::Path path, bool headerOnly){

	using namespace PJson;
	PFile::RW file = path.GetRW2("r");

	file.read(version,      sizeof(version));

	u32 sectors_number = 1; //placeholder;

	u32 compression = TILES_COMPRESSION_NONE;

	//Read level header
	{
		const nlohmann::json j = file.readCBOR();
		jsonReadString(j, "name", this->name);
		jsonReadString(j, "author", this->author);
		jsonReadInt(j, "number", this->level_number);
		jsonReadInt(j, "icon_x", this->icon_x);
		jsonReadInt(j, "icon_y", this->icon_y);
		jsonReadInt(j, "icon_id", this->icon_id);

		jsonReadU32(j, "compression", compression);

		/**
		 * @brief
		 * If loading a level only for the map entry, finish here.
		 */
		if(headerOnly)return;

		jsonReadU32(j, "sectors", sectors_number);

		if(j.contains("sprite_prototypes")){
			this->sprite_prototype_names = j["sprite_prototypes"].get<std::vector<std::string>>();
		}

		jsonReadInt(j, "player_index", this->player_sprite_index);
		jsonReadInt(j, "time", this->map_time);
		jsonReadString(j, "lua_script", this->lua_script);
		jsonReadInt(j, "game_mode", this->game_mode);
	}

	if(sectors_number==0){
		throw PExcept::PException("At least one level sector is required!");
	}

	this->sectors.resize(sectors_number);
	for(u32 i=0;i<sectors_number;++i){
		u32 width = 0; // placeholder
		u32 height = 0; //placeholder

		// Read sector header
		const nlohmann::json j = file.readCBOR();
		jsonReadU32(j, "width", width);
		jsonReadU32(j, "height", height);

		LevelSector*& sector = this->sectors[i];
		sector = new LevelSector(width, height);

		/**
		 * @brief 
		 * Tileset
		 */
		sector->tileset1 = this->mLoadTileset(j["tileset"].get<std::string>());

		/**
		 * @brief 
		 * Tileset bg
		 */

		if(j.contains("tileset_bg") && j["tileset_bg"].is_string()){
			std::string tileset_bg_name = j["tileset_bg"].get<std::string>();
			if(!tileset_bg_name.empty()){
				sector->tileset2 = this->mLoadTileset(tileset_bg_name);
			}
		}

		/**
		 * @brief 
		 * background
		 */
		sector->background = this->mLoadBackground(j["background"].get<std::string>());


		jsonReadString(j, "music", sector->music_name);

		jsonReadInt(j, "scrolling", sector->background->scrolling);
		jsonReadInt(j, "weather", sector->weather);

		jsonReadInt(j, "splash_color", sector->splash_color);
		jsonReadInt(j, "fire_color_1", sector->fire_color_1);
		jsonReadInt(j, "fire_color_2", sector->fire_color_2);

		if(j.contains("slipperiness")){
			sector->slipperiness_factor = j["slipperiness"].get<double>();
		}
		else{
			sector->slipperiness_factor = getDefaultSlipperinessFactor(sector->weather);
		}

		jsonReadString(j, "gfx", sector->gfxTextureName);
		if(sector->gfxTextureName!=""){
			sector->gfxTexture = this->mLoadGfxTexture(sector->gfxTextureName);
		}

		jsonReadInt(j, "rain_color", sector->rain_color);
		jsonReadString(j, "name", sector->name);


		// Background tiles
		readTiles(file, compression, sector->getWidth(), sector->size(), sector->background_tiles);

		// Foreground tiles
		readTiles(file, compression, sector->getWidth(), sector->size(), sector->foreground_tiles);

		// Sprite tiles
		readTiles(file, compression, sector->getWidth(), sector->size(), sector->sprite_tiles);
	}
	file.close();
}


// TO DO
void LevelClass::saveVersion15(PFile::Path path)const{

	PFile::RW file = path.GetRW2("w");
	char version[5] = "1.5";
	file.write(version, sizeof(version));

	//Write header
	{
		nlohmann::json j;
		j["name"] = this->name;
		j["author"] = this->author;
		j["number"] = this->level_number;
		j["icon_id"] = this->icon_id;
		j["icon_x"] = this->icon_x;
		j["icon_y"] = this->icon_y;

		j["sprite_prototypes"] = this->sprite_prototype_names;
		j["player_index"] = this->player_sprite_index;

		j["sectors"] = u32(this->sectors.size());
		j["time"] = this->map_time;
		j["lua_script"] = this->lua_script;
		j["game_mode"] = this->game_mode;

		j["compression"] = TILES_COMPRESSION_NONE;

		file.writeCBOR(j);
	}


	for(LevelSector* sector:this->sectors){
		nlohmann::json j;

		j["width"] = sector->getWidth();
		j["height"]= sector->getHeight();		

		j["tileset"] = sector->tileset1->name;
		if(sector->tileset2!=nullptr){
			j["tileset_bg"] = sector->tileset2->name;
		}

		
		j["music"] = sector->music_name;
		j["background"] = sector->background->name;
		j["scrolling"] = sector->background->scrolling;
		j["weather"] = sector->weather;

		j["splash_color"] = sector->splash_color;
		j["fire_color_1"] = sector->fire_color_1;
		j["fire_color_2"] = sector->fire_color_2;

		j["name"] = sector->name;
		j["gfx"] = sector->gfxTextureName;
		j["rain_color"] = sector->rain_color;

		file.writeCBOR(j);

		//background tiles
		file.write(sector->background_tiles, sizeof(u8) * sector->size());

		//foreground tiles
		file.write(sector->foreground_tiles, sizeof(u8) * sector->size());

		//sprite tiles
		file.write(sector->sprite_tiles, sizeof(u8) * sector->size());
	}

	file.close();
}


void LevelClass::calculateBlockTypes(){
	PK2BLOCK block;

	for (int i=0;i<150;i++){
		block = this->block_types[i];

		block.left  = 0;
		block.right  = 0;//32
		block.top	   = 0;
		block.bottom    = 0;//32

		block.id  = i;

		if ((i < 80 || i > 139) && i != 255){
			block.right_side	= BLOCK_WALL;
			block.left_side	= BLOCK_WALL;
			block.top_side		= BLOCK_WALL;
			block.bottom_side		= BLOCK_WALL;

			// Erikoislattiat

			if (i > 139){
				block.right_side	= BLOCK_BACKGROUND;
				block.left_side	= BLOCK_BACKGROUND;
				block.top_side		= BLOCK_BACKGROUND;
				block.bottom_side		= BLOCK_BACKGROUND;
			}

			// L�pik�velt�v� lattia

			if (i == BLOCK_BARRIER_DOWN){
				block.right_side	= BLOCK_BACKGROUND;
				block.top_side		= BLOCK_BACKGROUND;
				block.bottom_side		= BLOCK_WALL;
				block.left_side	= BLOCK_BACKGROUND;
				block.bottom -= 27;
			}

			// M�et

			if (i > 49 && i < 60){
				block.right_side	= BLOCK_BACKGROUND;
				block.top_side		= BLOCK_WALL;
				block.bottom_side		= BLOCK_WALL;
				block.left_side	= BLOCK_BACKGROUND;
				block.bottom += 1;
			}

			// Kytkimet

			if (i >= BLOCK_BUTTON1 && i <= BLOCK_BUTTON3){
				block.right_side	= BLOCK_WALL;
				block.top_side		= BLOCK_WALL;
				block.bottom_side		= BLOCK_WALL;
				block.left_side	= BLOCK_WALL;
			}
		}
		else{
			block.right_side	= BLOCK_BACKGROUND;
			block.left_side	= BLOCK_BACKGROUND;
			block.top_side		= BLOCK_BACKGROUND;
			block.bottom_side		= BLOCK_BACKGROUND;
		}

		if (i > 131 && i < 140)
			block.water = true;
		else
			block.water = false;

		this->block_types[i] = block;
	}
}

void LevelClass::moveBlocks(u32 button1, u32 button2, u32 button3){
	this->block_types[BLOCK_LIFT_HORI].left = (int)cos_table(degree);
	this->block_types[BLOCK_LIFT_HORI].right = (int)cos_table(degree);

	this->block_types[BLOCK_LIFT_VERT].bottom = (int)sin_table(degree);
	this->block_types[BLOCK_LIFT_VERT].top = (int)sin_table(degree);

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (button1 > 0) {
		kytkin1_y = 64;

		if (button1 < 64)
			kytkin1_y = button1;

		if (button1 > this->button1_time - 64)
			kytkin1_y = this->button1_time - button1;
	}

	if (button2 > 0) {
		kytkin2_y = 64;

		if (button2 < 64)
			kytkin2_y = button2;

		if (button2 > this->button2_time - 64)
			kytkin2_y = this->button2_time - button2;
	}

	if (button3 > 0) {
		kytkin3_x = 64;

		if (button3 < 64)
			kytkin3_x = button3;

		if (button3 > this->button3_time - 64)
			kytkin3_x = this->button3_time - button3;
	}

	kytkin1_y /= 2;
	kytkin2_y /= 2;
	kytkin3_x /= 2;

	this->block_types[BLOCK_BUTTON1].bottom = kytkin1_y;
	this->block_types[BLOCK_BUTTON1].top = kytkin1_y;

	this->block_types[BLOCK_BUTTON2_UP].bottom = -kytkin2_y;
	this->block_types[BLOCK_BUTTON2_UP].top = -kytkin2_y;

	this->block_types[BLOCK_BUTTON2_DOWN].bottom = kytkin2_y;
	this->block_types[BLOCK_BUTTON2_DOWN].top = kytkin2_y;

	this->block_types[BLOCK_BUTTON2].bottom = kytkin2_y;
	this->block_types[BLOCK_BUTTON2].top = kytkin2_y;

	this->block_types[BLOCK_BUTTON3_RIGHT].right = kytkin3_x;
	this->block_types[BLOCK_BUTTON3_RIGHT].left = kytkin3_x;
	this->block_types[BLOCK_BUTTON3_RIGHT].id = BLOCK_LIFT_HORI;

	this->block_types[BLOCK_BUTTON3_LEFT].right = -kytkin3_x;
	this->block_types[BLOCK_BUTTON3_LEFT].left = -kytkin3_x;
	this->block_types[BLOCK_BUTTON3_LEFT].id = BLOCK_LIFT_HORI;

	this->block_types[BLOCK_BUTTON3].bottom = kytkin3_x;
	this->block_types[BLOCK_BUTTON3].top = kytkin3_x;
}

void LevelClass::drawForegroundTiles(int camera_x, int camera_y, LevelSector* sector){


	int button1_timer_y = 0;
	int button2_timer_y = 0;
	int button3_timer_y = 0;

	if (button1_timer > 0){
		button1_timer_y = 64;

		if (button1_timer < 64)
			button1_timer_y = button1_timer;

		if (button1_timer > button1_time - 64)
			button1_timer_y = button1_time - button1_timer;
	}

	if (button2_timer > 0){
		button2_timer_y = 64;

		if (button2_timer < 64)
			button2_timer_y = button2_timer;

		if (button2_timer > button2_time - 64)
			button2_timer_y = button2_time - button2_timer;
	}

	if (button3_timer > 0){
		button3_timer_y = 64;

		if (button3_timer < 64)
			button3_timer_y = button3_timer;

		if (button3_timer > button3_time - 64)
			button3_timer_y = button3_time - button3_timer;
	}

	sector->drawForegroundTiles(camera_x, camera_y, this->block_animation_frame,
	this->arrows_block_degree, button1_timer_y, button2_timer_y, button3_timer_y);

	sector->animateTilesets(tiles_animation_timer, button1_timer);
	/**
	 * @brief 
	 * Blinking pixels,
	 * TO DO
	 * Move this to GameClass
	 */
	if(tiles_animation_timer%4 == 0){
		PDraw::rotate_palette(224,239);
	}
	
	this->tiles_animation_timer = 1 + this->tiles_animation_timer % 320;
}


Tileset* LevelClass::mLoadTileset(const std::string& tilesetName){
	for(Tileset* tileset : this->mTilesets){
		if(tileset->name == tilesetName){
			return tileset;
		}
	}
	Tileset* tileset = new Tileset();
	this->mTilesets.push_back(tileset);

	tileset->name = tilesetName;
	tileset->loadImage(tilesetName);

	return tileset;
}

Background* LevelClass::mLoadBackground(const std::string& backgroundName){
	for(Background* background : this->mBackgrounds){
		if(background->name == backgroundName){
			return background;
		}
	}

	Background* background = new Background();
	this->mBackgrounds.push_back(background);

	background->name = backgroundName;
	background->load(backgroundName);

	return background;
}

int LevelClass::mLoadGfxTexture(const std::string& name){
	for(const std::pair<int, std::string>& pair: this->mGfxTextures){
		if(pair.second==name){
			return pair.first;
		}
	}

	std::optional<PFile::Path> p = PFilesystem::FindAsset(name, PFilesystem::GFX_DIR);
	if(p.has_value()){
		int res = PDraw::image_load(*p);
		if(res>=0){
			this->mGfxTextures.push_back(std::make_pair(res, name));
			return res;
		}
		else{
			std::ostringstream os;
			os<<"Unable to load GFX texture: \""<<name<<"\"";
			throw std::runtime_error(os.str());
		}
	}
	throw PExcept::FileNotFoundException(this->name, PExcept::MISSING_GFX);
}