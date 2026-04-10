#include "prototypes_handler.hpp"

#include <fstream>
#include <filesystem>

#include "engine/PLog.hpp"
#include "engine/PString.hpp"
#include "engine/PFilesystem.hpp"
#include "episode/episodeclass.hpp"

#include "exceptions.hpp"
#include "system.hpp"

#include "settings/config_txt.hpp"

void PrototypesHandler::clear(){
	this->unloadSpriteAssets();
    for(PrototypeClass*&prot:mPrototypes){
		if(prot!=nullptr){
			delete prot;
			prot = nullptr;
		}
	}
	mPrototypes.clear();
}

PrototypeClass* PrototypesHandler::loadPrototype(const std::string& filename_cAsE){
	std::string filename = PString::rtrim(PString::lowercase(filename_cAsE));


	std::string filename_stem =  std::filesystem::path(filename).stem().string();	
	/**
	 * @brief Verify if the sprite is already loaded.
	 * It's completely necessary because it prevents infinite recursive loop in some cases.
	 */
	for(PrototypeClass* prot:mPrototypes){
		if(prot->filename==filename_stem){
			return prot;
		}
	}

	if(this->mAssetsLoaded){
		throw PExcept::PException("Cannot load a sprite prototype after loading sprite assets!");
	}
	
	std::string extension = std::filesystem::path(filename).extension().string();

	std::optional<PFile::Path> path;

	if(commandLineTool && extension==".spr"){
		path = PFilesystem::FindAsset(filename, PFilesystem::SPRITES_DIR);
	}
	else if(extension==".spr2"){
		path = PFilesystem::FindAsset(filename, PFilesystem::SPRITES_DIR, ".spr");
	}
	else if(extension==".spr"){
		path = PFilesystem::FindAsset(filename + "2", PFilesystem::SPRITES_DIR, ".spr");
	}
	else{
		path = PFilesystem::FindAsset(filename + ".spr2", PFilesystem::SPRITES_DIR, ".spr");
	}

	if(!path.has_value()){
		if(config_txt.panic_when_missing_assets){
			throw PExcept::FileNotFoundException(filename_cAsE, PExcept::MISSING_SPRITE_PROTOTYPE);
		}
		else{
			PLog::Write(PLog::ERR, "PK2 Sprites", "Sprite \"%s/.spr2\" not found!", filename_cAsE.c_str());

			PrototypeClass* placeholder = new PrototypeClass();
			placeholder->initMissingPlaceholder();
			placeholder->filename = filename_stem;
			return placeholder;
		}		
	}

	extension = PString::lowercase(std::filesystem::path(path->str()).extension().string());


	PrototypeClass* protot = new PrototypeClass();

	if(extension==".spr2"){
		/**
		 * @brief 
		 * TO DO Redesign it 
		 */
		protot->loadPrototypeJSON(*path,
			[&](const std::string& filename_in){return this->loadPrototype(filename_in);});
	}
	else if(extension==".spr"){
		protot->loadPrototypeLegacy(*path);
	}
	else{
		throw PExcept::PException("Unknown sprite extension \""+extension+"\"");
	}
	protot->filename = filename_stem;
	mPrototypes.push_back(protot);

	if(this->mEpisode!=nullptr && !this->mEpisode->ignore_collectable){
		const std::string& collectable_name = this->mEpisode->collectable_name;
		if(protot->name.compare(0, collectable_name.size(), collectable_name)==0){
			protot->big_apple = true;
		}
	}

	if(this->commandLineTool)return protot;

	//Check if ambient
	if(protot->type==TYPE_BACKGROUND || protot->type==TYPE_FOREGROUND || protot->type==TYPE_TELEPORT){
		protot->ambient = true;
	}
	else if(protot->indestructible && protot->damage==0 && protot->type!=TYPE_BONUS){
		protot->ambient=true;
	}

	/**
	 * @brief 
	 * Load dependecies
	 */

	//Load transformation
	if(!protot->transformation_str.empty()){
		protot->transformation = this->loadPrototype(protot->transformation_str);

		/**
		 * @brief 
		 * Fix red bonus stone not hurting enemies on fall
		 */
		if(protot->ambient && protot->transformation->type==TYPE_BONUS && !protot->enemy){
			protot->ambient = false;
		}
	}

	//Load bunus
	if(!protot->bonus_str.empty()){
		protot->bonus = this->loadPrototype(protot->bonus_str);
	}

	//Load ammo1
	if(!protot->ammo1_str.empty()){
		protot->ammo1 = this->loadPrototype(protot->ammo1_str);
	}

	//Load ammo2
	if(!protot->ammo2_str.empty()){
		protot->ammo2 = this->loadPrototype(protot->ammo2_str);
	}
	
	return protot;
}

void PrototypesHandler::savePrototype(PrototypeClass*prototype, const std::string& filename)const{

	std::string filename2;
	if(filename.size()>5 && filename.substr(filename.size()-5,5)==".spr2"){
		filename2 = filename;
	}
	else{
		filename2 = filename+".spr2";
	}
	nlohmann::json j = *prototype;
	std::ofstream out(filename2.c_str());
	if(!out.good()){	
		throw std::runtime_error("Cannot save the file!");
	}
	out<<j.dump(4);
	out.close();
}


PrototypeClass* PrototypesHandler::get(std::size_t index)const{
    if(index<mPrototypes.size()){
		return mPrototypes[index];
	}
	return nullptr;
}

void PrototypesHandler::loadSpriteAssets(){
	if(this->mEpisode==nullptr){
		throw PExcept::PException("Cannot load sprite assets if the episode pointer is null");
	}

    for(PrototypeClass* prototype: this->mPrototypes){
		prototype->loadAssets();
	}

	this->mAssetsLoaded = true;
}
void PrototypesHandler::unloadSpriteAssets(){
	for(PrototypeClass* prototype: this->mPrototypes){
		prototype->unloadAssets();
	}

	this->mAssetsLoaded = false;
}