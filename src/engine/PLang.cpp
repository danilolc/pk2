//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "PLang.hpp"
#include "PLog.hpp"
#include "PString.hpp"

#include <SDL.h>
#include <cstring>
#include <stdlib.h>

enum {
	READ_SKIP,
	READ_TITLE,
	READ_TEXT,
	READ_SPACE
};

const char MARKER_1 = '*',
           MARKER_2 = ':';

const std::string PLang::PLACEHOLDER = ".....";

PLang::PLang() {

}

PLang::PLang(PFile::Path path) {

	Read_File(path);

}

PLang::~PLang(){}

bool PLang::Read_File(PFile::Path path){

	try{
		PFile::RW io = path.GetRW2("r");
		values.clear();
		keys.clear();

		u8 marker;
		int table_index = -1;
		int read = READ_SKIP;

		while(io.read(&marker, 1)) {

			if (marker == '\r' || marker == '\n') {
				read = READ_SKIP;
				continue;
			}

			switch (read) {
				case READ_SKIP:
					if (marker == MARKER_1) {
						read = READ_TITLE;
						values.push_back("");
						keys.push_back("");
						table_index++;
					} 
					break;
				
				case READ_TITLE:
					if (marker == MARKER_2)
						read = READ_SPACE;
					else
						keys[table_index] += marker;
					break;
				
				case READ_SPACE:
					if (marker == ' ' || marker == '\t')
						break;
					read = READ_TEXT;
					
				case READ_TEXT:
					values[table_index] += marker;
					break;

			}
		}
		io.close();
	}
	catch(const PFile::PFileException& e){
		PLog::Write(PLog::ERR, "Plang", e.what());
		return false;
	}

	for(std::string& str: this->values){
		std::string::size_type index = 0;
		while ((index = str.find("\\n", index)) != std::string::npos) {
			str.replace(index, 2, "\n");
			++index;
		}
	}
	

	loaded = true;
	return true;
}

int PLang::Search_Id(const std::string& title)const {

	if (!loaded)
		return -1;

	size_t i;
	for (i = 0; i < keys.size(); i++)
		if (keys[i] == title) break;

	if (i >= keys.size())
		return -1;

	return i;

}

int PLang::searchLocalizedText(const std::string& title) {

	int id = this->Search_Id(title);
	if(id==-1){
		PLog::Write(PLog::WARN, "PLang", "Missing translation for *%s", title.c_str());
		id = keys.size();
		keys.emplace_back(title);
		values.emplace_back(std::string("...") + title + "...");
	}
	return id;
}



const std::string& PLang::Get_Text(int index)const {

	if (!loaded)
		return PLACEHOLDER;

	if (index>=0 && index < (int)values.size())
		return values[index];
	else
		return PLACEHOLDER;
}

int PLang::Set_Text(const std::string& title, const char* text) {

	if (!loaded)
		return -1;

	int idx = this->Search_Id(title);
	if (idx != -1) {
	
		values[idx] = text;
		return idx;
	
	}

	keys.push_back(title);
	values.push_back(text);
	return keys.size() - 1;

}

const std::string& PLang::getString(int id, const std::string& def)const{
	if(id>=0 && id < (int)this->values.size()){
		return this->values[id];
	}

	return def;
}

bool PLang::getBoolean(int id, bool def)const{
	if(id >= 0 && id < (int)this->values.size()){
		const std::string& text = PString::lowercase(this->values[id]);
		if(text=="default")return def;
		else if(text=="true")return true;
		else if(text=="false")return false;
		else if(text=="yes")return true;
		else if(text=="no")return false;
	}

	return def;
}

int PLang::getInteger(int id, int def)const{
	if(id >= 0 && id < (int)this->values.size()){
		int tmp = 0;
		const char* src = this->values[id].c_str();
		if(sscanf(src, "%i", &tmp)==1){
			return tmp;
		}
	}
	return def;
}