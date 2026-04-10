//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "levelsector.hpp"
#include "engine/PDraw.hpp"
#include "system.hpp"
#include "gfx/effect.hpp"
#include "engine/PString.hpp"
#include "engine/PSound.hpp"
#include "engine/PLog.hpp"
#include "engine/PFilesystem.hpp"
#include "episode/episodeclass.hpp"


LevelSector::LevelSector(std::size_t width, std::size_t height):
sprites(this), mWidth(width), mHeight(height){
    this->mSize = width * height;
    if(this->mSize>0){
        this->background_tiles = new u8[this->mSize];
        this->foreground_tiles = new u8[this->mSize];
        this->sprite_tiles = new u8[this->mSize];
		this->edges = new bool[this->mSize];

		memset(this->background_tiles, u8(255), this->mSize);
		memset(this->foreground_tiles, u8(255), this->mSize);
		memset(this->sprite_tiles, u8(255), this->mSize);
		memset(this->edges, false, this->mSize);
    }

	this->gfxTexture = global_gfx_texture;
}

LevelSector::~LevelSector(){
    this->mSize = 0;
    if(this->background_tiles!=nullptr){
        delete[] this->background_tiles;
        this->background_tiles = nullptr;
    }

    if(this->foreground_tiles!=nullptr){
        delete[] this->foreground_tiles;
        this->foreground_tiles = nullptr;
    }

    if(this->sprite_tiles!=nullptr){
        delete[] this->sprite_tiles;
        this->sprite_tiles = nullptr;
    }

	if(this->edges!=nullptr){
		delete[] this->edges;
		this->edges = nullptr;
	}
}

void LevelSector::calculateColors(){
	if(this->splash_color==-1){
		this->splash_color = this->tileset1->calculateSplashColor();
	}
}

void LevelSector::calculateEdges(){
	memset(this->edges, false, this->mSize);
	u8 tile1, tile2, tile3;
	for (u32 y = 0; y < this->mHeight - 1; y++){
		for (u32 x = 1; x < this->mWidth - 1; x++){
			bool edge = false;

			tile1 = this->foreground_tiles[x+y*this->mWidth];

			if (tile1 > BLOCK_EXIT)
				this->foreground_tiles[x+y*this->mWidth] = 255;

			tile2 = this->foreground_tiles[x+(y+1)*this->mWidth];

			if (tile1 > 79 || tile1 == BLOCK_BARRIER_DOWN) tile1 = 1; else tile1 = 0;
			if (tile2 > 79) tile2 = 1; else tile2 = 0;

			if (tile1 == 1 && tile2 == 1){
				tile1 = this->foreground_tiles[x+1+(y+1)*this->mWidth];
				tile2 = this->foreground_tiles[x-1+(y+1)*this->mWidth];

				if (tile1 < 80  && !(tile1 < 60 && tile1 > 49)) tile1 = 1; else tile1 = 0;
				if (tile2 < 80  && !(tile2 < 60 && tile2 > 49)) tile2 = 1; else tile2 = 0;

				if (tile1 == 1){
					tile3 = this->foreground_tiles[x+1+y*this->mWidth];
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_BARRIER_DOWN)
						edge = true;
				}

				if (tile2 == 1){
					tile3 = this->foreground_tiles[x-1+y*this->mWidth];
					if (tile3 > 79 || (tile3 < 60 && tile3 > 49) || tile3 == BLOCK_BARRIER_DOWN)
						edge = true;
				}

				if (edge){
					this->edges[x+y*this->mWidth] = true;
					//this->background_tiles[x+y*this->mWidth] = 18; //Debug
				}
			}
		}
	}
}


void LevelSector::drawBackgroundTiles(int camera_x, int camera_y,int block_animation_frame)const{

    int kartta_x = camera_x/32;
	int kartta_y = camera_y/32;

	int tiles_w = screen_width/32 + 1;
	int tiles_h = screen_height/32 + 1;


	for (int x = 0; x < tiles_w; x++){
		if (x + kartta_x < 0 || uint(x + kartta_x) > this->mWidth) continue;

		for (int y = 0; y < tiles_h; y++){
			if (y + kartta_y < 0 || uint(y + kartta_y) > this->mHeight) continue;

			int i = x + kartta_x + (y + kartta_y) * this->mWidth;
			if( i < 0 || i >= int(this->mSize)) continue; //Dont access a not allowed address

			int block = background_tiles[i];

			if (block != 255){
				int px = ((block%10)*32);
				int py = ((block/10)*32);

				if (block == BLOCK_ANIM1 || block == BLOCK_ANIM2 || block == BLOCK_ANIM3 || block == BLOCK_ANIM4)
					px += block_animation_frame * 32;

				PDraw::image_cutclip(this->tileset2 ? this->tileset2->getImage() : this->tileset1->getImage(),	
				x*32-(camera_x%32), y*32-(camera_y%32), px, py, px+32, py+32);
			}
		}
	}
}

void LevelSector::drawForegroundTiles(int camera_x, int camera_y, int block_animation_frame,
int arrows_block_degree, int button1_timer_y, int button2_timer_y, int button3_timer_y)const{

    int kartta_x = camera_x/32;
	int kartta_y = camera_y/32;

	int tiles_w = screen_width/32 + 1;
	int tiles_h = screen_height/32 + 1;

    for (int x = -1; x < tiles_w + 1; x++) {
		if (x + kartta_x < 0 || uint(x + kartta_x) > this->mWidth) continue;

		for (int y = -1; y < tiles_h + 1; y++) {
			if (y + kartta_y < 0 || uint(y + kartta_y) > this->mHeight) continue;

			int i = x + kartta_x + (y + kartta_y) * this->mWidth;
			if( i < 0 || i >= int(this->mSize)) continue; //Dont access a not allowed address

			u8 block = foreground_tiles[i];

			if (block != 255 && block != BLOCK_BARRIER_DOWN){
				
				int px = (block % 10) * 32;
				int py = (block / 10) * 32;
				
				int ay = 0;
				int ax = 0;

				if (block == BLOCK_LIFT_VERT)
					ay = floor(sin_table(arrows_block_degree));

				else if (block == BLOCK_LIFT_HORI)
					ax = floor(cos_table(arrows_block_degree));

				else if (block == BLOCK_BUTTON1)
					ay = button1_timer_y/2;

				else if (block == BLOCK_BUTTON2_UP)
					ay = -button2_timer_y/2;

				else if (block == BLOCK_BUTTON2_DOWN)
					ay = button2_timer_y/2;

				else if (block == BLOCK_BUTTON2)
					ay = button2_timer_y/2;

				else if (block == BLOCK_BUTTON3_RIGHT)
					ax = button3_timer_y/2;

				else if (block == BLOCK_BUTTON3_LEFT)
					ax = -button3_timer_y/2;

				else if (block == BLOCK_BUTTON3)
					ay = button3_timer_y/2;

				else if (block == BLOCK_ANIM1 || block == BLOCK_ANIM2 || block == BLOCK_ANIM3 || block == BLOCK_ANIM4)
					px += block_animation_frame * 32;

				//hide drift tiles
				else if(block == BLOCK_DRIFT_LEFT || block == BLOCK_DRIFT_RIGHT){
					if(Episode!=nullptr && !Episode->visible_wind){
						continue;
					}
				}
				PDraw::image_cutclip(this->tileset1->getImage(), x*32-(camera_x%32)+ax, y*32-(camera_y%32)+ay, px, py, px+32, py+32);
			}
		}
	}    
}

PK2BLOCK LevelSector::getBlock(u32 x, u32 y, const std::array<PK2BLOCK, TILESET_SIZE>& block_types)const{
	PK2BLOCK block;
	//memset(&block, 0, sizeof(block));

	// Outside the screen
	if (x >= this->mWidth || y >= this->mHeight) {
		
		block.id  = 255;
		block.left  = x*32;
		block.right  = x*32 + 32;
		block.top    = y*32;
		block.bottom    = y*32 + 32;
		block.water  = false;
		block.border = true;

		block.left_side = 0;
		block.right_side = 0;
		block.top_side = 0;
		block.bottom_side = 0;

		return block;

	}

	u8 i = this->foreground_tiles[x+y*this->mWidth];

	if (i<150) { //If it is ground

		block        = block_types[i];
		block.left  = x*32+block_types[i].left;
		block.right  = x*32+32+block_types[i].right;
		block.top    = y*32+block_types[i].top;
		block.bottom    = y*32+32+block_types[i].bottom;

	} else { //If it is sky - Need to reset
	
		block.id  = 255;
		block.left  = x*32;
		block.right  = x*32 + 32;
		block.top    = y*32;
		block.bottom    = y*32 + 32;
		block.water  = false;

		block.left_side = 0;
		block.right_side = 0;
		block.top_side = 0;
		block.bottom_side = 0;
	
	}

	i = this->background_tiles[x+y*this->mWidth];

	if (i > 131 && i < 140)
		block.water = true;

	block.border = this->edges[x+y*this->mWidth];

	return block;
}

void LevelSector::openKeylocks(bool effect){
	this->sprites.onOpeningKeylocks();
	for (u32 y = 0; y < this->mHeight; y++){
		for(u32 x = 0; x < this->mWidth; x++){
			u8 block = this->foreground_tiles[x+y*this->mWidth];
			
			if (block == BLOCK_LOCK){
				this->foreground_tiles[x+y*this->mWidth] = 255;

				if(effect){
					Effect_SmokeClouds(x*32+24,y*32+6);
				}
			}
		}
	}
	this->calculateEdges();
}

void LevelSector::changeSkulls(bool effect){
	for (u32 y = 0; y < this->mHeight; y++){
		for(u32 x = 0; x < this->mWidth; x++){
			
			u8 front = this->foreground_tiles[x+y*this->mWidth];
			u8 back  = this->background_tiles[x+y*this->mWidth];

			if (front == BLOCK_SKULL_FOREGROUND){
				this->foreground_tiles[x+y*this->mWidth] = 255;
				if (back != BLOCK_SKULL_FOREGROUND && effect)
					Effect_SmokeClouds(x*32+24,y*32+6);

			}

			if (back == BLOCK_SKULL_BACKGROUND && front == 255)
				this->foreground_tiles[x+y*this->mWidth] = BLOCK_SKULL_FOREGROUND;
		}
	}

	this->calculateEdges();
}

void LevelSector::countStartSigns(std::vector<BlockPosition>& vec, u32 sector_id)const{
	for (u32 y = 0; y < this->mHeight; y++){
		for(u32 x = 0; x < this->mWidth; x++){
			u8 block = this->foreground_tiles[x+y*this->mWidth];
			if(block == BLOCK_START){
				vec.push_back(BlockPosition(x, y, sector_id));
			}
		}
	}
}
/**
 * @brief 
 * TODO
 * Redesign and optimise this function
 */
void LevelSector::startMusic(){

	this->music_name = PString::rtrim(this->music_name); // to fix bugs with blank field

	if (!this->music_name.empty()) {
		std::optional<PFile::Path> music_path = PFilesystem::FindAsset(this->music_name,
		PFilesystem::MUSIC_DIR);

		if (!music_path.has_value()) {

			PLog::Write(PLog::ERR, "PK2", "Can't find music \"%s\", trying \"song01.xm\"",this->music_name.c_str());
			music_path = PFilesystem::FindAsset("song01.xm",PFilesystem::MUSIC_DIR);
		}

		if(music_path.has_value()){
			if (PSound::start_music(*music_path) == -1){
				PLog::Write(PLog::ERR, "PK2", "Can't start the music");				
			}			
		}
		else{
			PLog::Write(PLog::ERR, "PK2", "Can't load any music file");
		}
	}
	else{
		PSound::stop_music();
	}
}