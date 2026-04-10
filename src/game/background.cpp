//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "background.hpp"
#include "engine/PDraw.hpp"
#include "engine/PFilesystem.hpp"

#include "settings/config_txt.hpp"
#include "system.hpp"
#include "exceptions.hpp"

#include "engine/PLog.hpp"
#include "gfx/missing_texture.hpp"
#include "gfx/text.hpp"
#include <sstream>

void Background::load(const std::string&name){

	std::optional<PFile::Path> path = PFilesystem::FindAsset(name, PFilesystem::SCENERY_DIR);

    if (!path.has_value()){
		if(config_txt.panic_when_missing_assets){
			throw PExcept::FileNotFoundException(name, PExcept::MISSING_BACKGROUND);
		}
		else{

			/**
			 * @brief 
			 * Take palette from "ending.bmp" if background not found.
			 */


			std::optional<PFile::Path> path = PFilesystem::FindVanillaAsset("ending.bmp", PFilesystem::GFX_DIR);
			if(!path.has_value()){
				throw PExcept::FileNotFoundException("File \"ending.bmp\" not found!", PExcept::MISSING_BACKGROUND);
			}

			PLog::Write(PLog::ERR, "PK2", "Background \"%s\" not found!", name.c_str());
			
			std::pair<int, int> p = PDraw::image_load_with_palette(*path, false);
			this->palette = p.second;
			PDraw::image_delete(p.first);

			this->picture = new_missing_texture_placeholder(37, 32, screen_width, screen_height);
			this->width = screen_width;
			this->height = screen_height;
		}
	}
	else{
		std::pair<int, int> p = PDraw::image_load_with_palette(*path, false);
		this->picture = p.first;
		this->palette = p.second;

		PDraw::image_getsize(this->picture, this->width, this->height);

		//PDraw::palette_set_if_not_set(this->palette);

		if (this->picture == -1)
			throw PExcept::PException("Cannot load the background!");
	}
}

void Background::setPalette(){
	PDraw::palette_set(this->palette);
}

void Background::clear(){
    PDraw::image_delete(this->picture);
	PDraw::palette_delete(this->palette);
}


void Background::draw(int camera_x, int camera_y){
    int pallarx = ( camera_x % (this->width*3) ) / 3;
	int pallary = ( camera_y % (this->height*3) ) / 3;

	if (this->scrolling == BACKGROUND_STATIC){
	
		PDraw::image_clip(this->picture,0,0);
		PDraw::image_clip(this->picture,this->width,0);
	
	} else if (this->scrolling == BACKGROUND_PARALLAX_HORI){
	
		PDraw::image_clip(this->picture,0   - pallarx,0);
		PDraw::image_clip(this->picture,this->width - pallarx,0);

		if (screen_width > this->width)
			PDraw::image_clip(this->picture,this->width*2 - pallarx,0);
	
	} else if (this->scrolling == BACKGROUND_PARALLAX_VERT){
	
		PDraw::image_clip(this->picture,0,0   - pallary);
		PDraw::image_clip(this->picture,0,this->height - pallary);

		if (screen_width > this->width){
			PDraw::image_clip(this->picture,this->width,0   - pallary);
			PDraw::image_clip(this->picture,this->width,this->height - pallary);
		}
	
	} else if (this->scrolling == BACKGROUND_PARALLAX_VERT_AND_HORI){
	
		PDraw::image_clip(this->picture,0   - pallarx, 0-pallary);
		PDraw::image_clip(this->picture,this->width - pallarx, 0-pallary);
		PDraw::image_clip(this->picture,0   - pallarx, this->height-pallary);
		PDraw::image_clip(this->picture,this->width - pallarx, this->height-pallary);

		if (screen_width > this->width){
			PDraw::image_clip(this->picture,this->width*2 - pallarx,0-pallary);
			PDraw::image_clip(this->picture,this->width*2 - pallarx,this->height-pallary);
		}
	}

	/*if(this->missing){
		std::ostringstream os;
		os<<"Backround \""<<this->name<<"\" not found!";
		PDraw::font_write(fontti1, os.str(), 50, 50);
	}*/
};