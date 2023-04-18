//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PLang.hpp"

#include <SDL.h>
#include <cstring>

enum {
	READ_SKIP,
	READ_TITLE,
	READ_TEXT,
	READ_SPACE
};

const char MARKER_1 = '*',
           MARKER_2 = ':';

PLang::PLang() {

}

PLang::PLang(PFile::Path path) {

	Read_File(path);

}

PLang::~PLang(){}

bool PLang::Read_File(PFile::Path path){

	PFile::RW* io = path.GetRW("r");

	if (io == nullptr)
		return false;

	tekstit.clear();
	titles.clear();

	u8 marker;
	int table_index = -1;
	int read = READ_SKIP;

	while(io->read(&marker, 1)) {

		if (marker == '\r' || marker == '\n') {
			read = READ_SKIP;
			continue;
		}

		switch (read) {
			case READ_SKIP:
				if (marker == MARKER_1) {
					read = READ_TITLE;
					tekstit.push_back("");
					titles.push_back("");
					table_index++;
				} 
				break;
			
			case READ_TITLE:
				if (marker == MARKER_2)
					read = READ_SPACE;
				else
					titles[table_index] += marker;
				break;
			
			case READ_SPACE:
				if (marker == ' ' || marker == '\t')
					break;
				read = READ_TEXT;
				
			case READ_TEXT:
				tekstit[table_index] += marker;
				break;

		}
	}

    io->close();

	loaded = true;
	return true;
}

int PLang::Search_Id(const char *text) {

	if (!loaded)
		return -1;

	size_t i;
	for (i = 0; i < titles.size(); i++)
		if (titles[i] == text) break;

	if (i >= titles.size())
		return -1;

	return i;

}

const char* PLang::Get_Text(size_t index) {

	if (!loaded)
		return ".....";

	if (index < tekstit.size())
		return tekstit[index].c_str();
	else
		return ".....";
}

int PLang::Set_Text(const char* title, const char* text) {

	if (!loaded)
		return -1;

	int idx = this->Search_Id(title);
	if (idx != -1) {
	
		tekstit[idx] = text;
		return idx;
	
	}

	titles.push_back(title);
	tekstit.push_back(text);
	return titles.size() - 1;

}