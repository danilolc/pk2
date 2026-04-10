//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PLog.hpp"
#include "game/levelclass.hpp"
#include "file_converter.hpp"
#include <string>

#include "system.hpp"

void replaceObsoleteAIs(PrototypeClass* prototype){

	bool is_info = false;
	std::vector<int>& vec = prototype->AI_v;

	for(const int& ai: vec){
		if(ai>=AI_LEGACY_INFOS_BEGIN && ai<=AI_LEGACY_INFOS_END){
			prototype->info_id = ai - AI_LEGACY_INFOS_BEGIN + 1;
			is_info = true;
		}
	}

	if(is_info){
		vec.erase(std::remove_if(vec.begin(), vec.end(), [](int ai) { return ai>=201 && ai<=302; }), vec.end());
		vec.push_back(302);
	}
}


void convertToSpr2(const std::string& filename_in, const std::string& filename_out){
	PrototypesHandler handler(nullptr, true);
	try{
		PrototypeClass* prototype = handler.loadPrototype(filename_in);
		replaceObsoleteAIs(prototype);
		nlohmann::json j = *prototype;
		handler.savePrototype(prototype, filename_out);
		PLog::Write(PLog::INFO, "PK2 Tools", "Sprite %s converted to %s\n", filename_in.c_str(), filename_out.c_str());
	}
	catch(const std::exception&e){
		printf("%s\n", e.what());
	}
}

void convertLevel(const std::string& filename_in, const std::string& filename_out){
	try{
		LevelClass level;
		level.load(PFile::Path(filename_in), true);
		printf("Converting level \"%s\" to the new experimental format.\n", level.name.c_str());

		level.saveVersion15(PFile::Path(filename_out));
		printf("Done!\n");
	}
	catch(const std::exception& e){
		printf("%s\n", e.what());
	}
}

bool pk2_convertToNewFormat(const std::string& filename_in, const std::string& filename_out){
	if(filename_in.empty()){
		printf("You have to specify the sprite to convert!");
		return false;
	}

	
	/**
	 * @brief 
	 * Sprite
	 */
	if(filename_in.size()>4 && filename_in.substr(filename_in.size()-4,4)==".spr"){
		std::string filename_out2;
		if(filename_out.empty()){
			filename_out2 = filename_in + "2";
		}
		else{
			filename_out2 = filename_out;
		}
		convertToSpr2(filename_in, filename_out2);
		return true;
	}
	else if(filename_in.size()>5 && filename_in.substr(filename_in.size()-5,5)==".spr2"){
		std::string filename_out2;
		if(filename_out.empty()){
			filename_out2 = filename_in;
		}
		convertToSpr2(filename_in, filename_out2);
		return true;
	}

	/**
	 * @brief 
	 * Level
	 */
	else if(filename_in.size()>4 && filename_in.substr(filename_in.size()-4,4)==".map"){
		std::string filename_out2;
		if(filename_out.empty()){
			filename_out2 = filename_in.substr(0,filename_in.size()-4) + ".pk2lev";
		}
		else{
			filename_out2 = filename_out;
		}
		convertLevel(filename_in, filename_out2);
		return true;
	}

	printf("Unsupported file format to convert");
	
	return false;
}

/**
 * @brief 
 * A temporary function for updating sprites.
 */

void pk2_updateSprites(const std::string& dir){
	printf("%s\n", dir.c_str());
	
	for (const auto & entry : std::filesystem::directory_iterator(dir)){
        if(entry.is_regular_file()){
            std::filesystem::path p = entry.path();
            std::string s = p.string();
			printf("%s\n", s.c_str());
            if(s.size()>5 && s.substr(s.size()-5,5)==".spr2"){
				convertToSpr2(s, s);	
			}
		}
	}
}