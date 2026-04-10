//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "game/gifts.hpp"
#include <cstring>
#include <vector>

GiftsHandler::GiftsHandler(){
	for(PrototypeClass*& proto: this->gifts_list){
		proto = nullptr;
	}	
}


void GiftsHandler::draw(int x, int y)const{

	/*int x,y;

	y = screen_height-35;//36
	x = panel_x + 35;//40*/
	for(PrototypeClass*proto: this->gifts_list){
		if(proto!=nullptr){
			proto->draw(x - proto->width / 2, y - proto->height / 2, 0);
		}

		x += 38;
	}
}

void GiftsHandler::clean(){
	this->giftsNumber = 0;
	for(PrototypeClass*& proto: this->gifts_list){
		proto = nullptr;
	}
}
bool GiftsHandler::add(PrototypeClass* protot){
	for(std::size_t i=0;i<this->gifts_list.size();++i){
		if(this->gifts_list[i] == nullptr){
			this->gifts_list[i] = protot;
			++this->giftsNumber;
			return true;
		}
	}

	return false;
}

void GiftsHandler::remove(int i){

	if(this->gifts_list[i] == nullptr)
		return;

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		this->gifts_list[i] = this->gifts_list[i+1];
	
	this->gifts_list[MAX_GIFTS - 1] = nullptr;
	
	--this->giftsNumber;
}

void GiftsHandler::use(SpritesHandler& sh, SpriteClass*player){
	if(this->giftsNumber > 0){
		sh.addGiftSprite(gifts_list[0], player);
		this->remove(0);
	}
}

void GiftsHandler::changeOrder(){
	if (this->gifts_list[0] == nullptr)
		return;

	PrototypeClass* temp = this->gifts_list[0];

	for (int i = 0; i < MAX_GIFTS - 1; i++)
		this->gifts_list[i] = this->gifts_list[i+1];

	int count = 0;

	while(count < MAX_GIFTS-1 && gifts_list[count] != nullptr)
		count++;

	this->gifts_list[count] = temp;
}

int GiftsHandler::totalScore()const{
	int res = 0;

	for(PrototypeClass*proto: this->gifts_list){
		if(proto!=nullptr){
			res += proto->score + 500;
		}
	}

	return res;
}

nlohmann::json GiftsHandler::toJson()const{
	std::vector<std::string> vec;
	for(const PrototypeClass* proto: this->gifts_list){
		if(proto!=nullptr){
			vec.emplace_back(proto->filename);
		}
	}
	return nlohmann::json(vec);
}

void GiftsHandler::fromJson(const nlohmann::json& j, PrototypesHandler& prototypes){
	std::vector<std::string> vec;
	j.get_to(vec);

	this->clean();
	for(const std::string& filenane: vec){
		this->add(prototypes.loadPrototype(filenane));
	}
}