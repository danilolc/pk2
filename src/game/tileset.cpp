//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include <sstream>
#include "tileset.hpp"

#include "engine/PDraw.hpp"
#include "engine/PFile.hpp"
#include "engine/PFilesystem.hpp"
#include "engine/PLog.hpp"

#include "settings/config_txt.hpp"
#include "gfx/missing_texture.hpp"

#include "system.hpp"
#include "exceptions.hpp"
#include <array>


void Tileset::clear(){
    PDraw::image_delete(this->tiles);
    PDraw::image_delete(this->water_tiles);
}


void Tileset::loadImage(const std::string& name){
	std::optional<PFile::Path> path = PFilesystem::FindAsset(name, PFilesystem::TILES_DIR);
	if(!path.has_value()){
		if(config_txt.panic_when_missing_assets){
			throw PExcept::FileNotFoundException(name, PExcept::MISSING_TILESET);
		}
		else{
			PLog::Write(PLog::ERR, "PK2", "Tileset \"%s\" not found!", name.c_str());
			this->tiles = new_missing_texture_placeholder(174, 0, 320, 480);
		}		
	}
	else{
		PDraw::image_load(this->tiles, *path, true);
		if(this->tiles==-1){
			throw PExcept::PException("Unable to load tileset image!");
		}

		int w=0, h=0;
		PDraw::image_getsize(this->tiles, w, h);

		if(w!=320 || (h!=480 && h!= 608)){
			std::ostringstream os;
			os<<"Incorrect tileset size: "<<w<<" x "<<h;
			throw PExcept::PException(os.str());
		}
	}


	this->water_tiles = PDraw::image_cut(this->tiles,0,416,320,32);
	this->calculateBlockMasks();
	this->make254Transparent();
}

void Tileset::animate(int animation_timer, int button1_timer, int fire_color1, int fire_color2){
	if(animation_timer%2 == 0){
		this->animateFire(button1_timer, fire_color1, fire_color2);
		this->animateWaterSurface();
		this->animateWaterfall();
		this->animateRollUp();

		if(animation_timer%4 == 0){
			this->animateWater(animation_timer);
		}
	}
}

void Tileset::animateFire(int button1_timer, int color1, int color2){
    u8 *buffer = NULL;
	u32 width;
	int x,y;
	int color;

	PDraw::drawimage_start(this->tiles, buffer, width);

	for (x=128;x<160;x++)
		for (y=448;y<479;y++)
		{
			color = buffer[x+(y+1)*width];

			if (color != 255)
			{
				color %= 32;
				color = color - this->anim_rand()%4;

				if (color < 0)
					color = 255;
				else
				{
					if (color > 21)
						color += color2; //128;
					else
						color += color1; //64;
				}
			}

			buffer[x+y*width] = color;
		}

	if (button1_timer < 20)
	{
		for (x=128;x<160;x++)
			buffer[x+479*width] = color2 + 16 + this->anim_rand()%15; //this->anim_rand()%15+144;
	}
	else
		for (x=128;x<160;x++)
			buffer[x+479*width] = 255;

	PDraw::drawimage_end(tiles);
}

void Tileset::animateWaterfall(){
    u8 *buffer = NULL;
	u32 width;
	int x,y,plus;
	int color,color2;

	u8 temp[32*32];

	PDraw::drawimage_start(this->tiles, buffer, width);

	for (x=32;x<64;x++)
		for (y=416;y<448;y++)
			temp[x-32+(y-416)*32] = buffer[x+y*width];

	color2 = temp[0];
	x = 1;
	while (color2==255 && x<32){
		color2 = temp[x];
		++x;
	}	

	color2 = (color2/32)*32;	// mahdollistaa erivriset vesiputoukset

	for (x=32;x<64;x++)
	{
		plus = this->anim_rand()%2+2;//...+1
		for (y=416;y<448;y++)
		{
			color = temp[x-32+(y-416)*32];

			if (color != 255)	// mahdollistaa eri leveyksiset vesiputoukset
			{
				color %= 32;
				if (color > 10)//20
					color--;
				if (this->anim_rand()%40 == 1)
					color = 11+this->anim_rand()%11;//15+this->anim_rand()%8;//9+this->anim_rand()%5;
				if (this->anim_rand()%160 == 1)
					color = 30;
				buffer[x + (416+(y+plus)%32)*width] = color+color2;
			}
			else
				buffer[x + (416+(y+plus)%32)*width] = color;
		}
	}

	PDraw::drawimage_end(this->tiles);
}

void Tileset::animateWaterSurface(){
    u8 *buffer = NULL;
	u32 width;
	int x,y;

	u8 temp[32];

	PDraw::drawimage_start(this->tiles, buffer, width);

	for (y=416;y<448;y++)
		temp[y-416] = buffer[y*width];

	for (y=416;y<448;y++)
	{
		for (x=0;x<31;x++)
		{
			buffer[x+y*width] = buffer[x+1+y*width];
		}
	}

	for (y=416;y<448;y++)
		buffer[31+y*width] = temp[y-416];

	PDraw::drawimage_end(this->tiles);
}

void Tileset::animateWater(int animation_timer){

    u8 *buffer_lahde = NULL, *buffer_kohde = NULL;
	u32 leveys_lahde, leveys_kohde;
	int x, y, color1, color2;
	int d1 = animation_timer / 2, d2;
	int sini, cosi;
	int vx,vy;
	int i;


	PDraw::drawimage_start(tiles, buffer_kohde, leveys_kohde);
	PDraw::drawimage_start(water_tiles, buffer_lahde, leveys_lahde);

	for (y=0;y<32;y++){
		d2 = d1;

		for (x=0;x<32;x++){
			sini = (y+d2/2) * 11.25;
			cosi = (x+d1/2) * 11.25;
			sini = (int)sin_table(sini);
			cosi = (int)cos_table(cosi);

			vy = (y+sini/11)%32;
			vx = (x+cosi/11)%32;

			if (vy < 0){
				vy = -vy;
				vy = 31-(vy%32);
			}

			if (vx < 0){
				vx= -vx;
				vx = 31-(vx%32);
			}

			color1 = buffer_lahde[64+vx+vy*leveys_lahde];
			buffer_lahde[32+x+y*leveys_lahde] = color1;
			d2 = 1 + d2 % 360;
		}

		d1 = 1 + d1 % 360;
	}

	int vy2;

	for (int p=2;p<5;p++){
		i = p*32;

		for (y=0;y<32;y++){
			//d2 = d1;
			vy = y*leveys_lahde;
			vy2 = (y+416)*leveys_kohde;

			for (x=0;x<32;x++){
				vx = x+vy;
				color1 = buffer_lahde[32+vx];
				color2 = buffer_lahde[ i+vx];
				buffer_kohde[i+x+vy2] = (color1 + color2*2) / 3;
			}
		}
	}
	PDraw::drawimage_end(tiles);
	PDraw::drawimage_end(water_tiles);
}

void Tileset::animateRollUp(){
	u8 *buffer = NULL;
	u32 width;
	int y;

	u8 temp[32];

	PDraw::drawimage_start(this->tiles, buffer, width);

	memcpy(temp, 64 + buffer + 448*width, 32);

	for (y=448; y<479; y++)
		memcpy(buffer + 64 + y*width, buffer + 64 + (y+1)*width, 32);

	memcpy(buffer + 64 + 479*width, temp, 32);

	PDraw::drawimage_end(this->tiles);
}

int Tileset::calculateSplashColor(){
	u8* buffer = nullptr;
	u32 width = 0;

	PDraw::drawimage_start(tiles, buffer, width);

	/**
	 * @brief
	 * gray 	 - 0 
	 * blue 	 - 1
	 * red 		 - 2
	 * green	 - 3
	 * orange	 - 4
	 * violet	 - 5
	 * turquoise - 6
	 * unknown	 - 7
	 */

	std::array<int, 8> colorCounters = {0};
	for(int y = 416; y < 448; ++y){
		//for(int x = 64; x < 320; ++x){
		for(int x = 0; x<32;++x){ //flowing water

			int color = buffer[x + y*width];
			if(color==255)continue; //do not count transparent pixels

			color /= 32;
			if(color < 7){
				colorCounters[color] += 1;
			}
			else{
				colorCounters[7] += 1; //Unknown colors
			}
		}
	}

	PDraw::drawimage_end(tiles);

	int max_val = colorCounters[0];
	int max_index = 0;

	for(int i=1; i < 8; ++i){
		int val = colorCounters[i];

		if(val > max_val){
			max_val = val;
			max_index = i;
		}
	}

	if(max_index==7){
		/**
		 * @brief 
		 * Fallback to the blue if unknown colors predominate.
		 */
		max_index = 1;
	}

	return 32* max_index;
}

void Tileset::make254Transparent(){
	u8 *buffer = NULL;
	u32 width;
	int x,y;

	int w, h;
	PDraw::image_getsize(this->tiles, w, h);

	PDraw::drawimage_start(this->tiles, buffer, width);
	for (y = 0; y < h; y++)
		for(x = 0; x < w; x++)
			if (buffer[x+y*width] == 254)
				buffer[x+y*width] = 255;
	PDraw::drawimage_end(this->tiles);
}

void Tileset::calculateBlockMasks(){
	u8 *buffer = nullptr;
	u32 width;
	int x, y;
	u8 color;

	PDraw::drawimage_start(this->tiles, buffer, width);
	for (int mask=0; mask<TILESET_SIZE; mask++){
		for (x=0; x<32; x++){
			y=0;
			while (y<31 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*width])==255)
				y++;

			block_masks[mask].alas[x] = y;
		}

		for (x=0; x<32; x++){
			y=31;
			while (y>=0 && (color = buffer[x+(mask%10)*32 + (y+(mask/10)*32)*width])==255)
				y--;

			block_masks[mask].ylos[x] = 31-y;
		}
	}
	PDraw::drawimage_end(this->tiles);
}