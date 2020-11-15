//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "engine/PLang.hpp"

#include <SDL.h>
#include <cstring>

const int LUE_SKIP    = 0,
          LUE_OTSIKKO = 1,
          LUE_TEKSTI  = 2;

const char MARKER_1 = '*',
           MARKER_2 = ':';

PLang::PLang() {

	read = LUE_SKIP;

	tekstit.clear();
	titles.clear();

}

PLang::PLang(PFile::Path path) {

	read = LUE_SKIP;

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
	int table_index = 0;
	read = LUE_SKIP;

	tekstit.push_back("");
	titles.push_back("");

	while(io->read(&marker, 1)) {
		switch (marker) {
			case MARKER_1:
				if (read == LUE_SKIP) {
					read = LUE_OTSIKKO;
				} else {
					read = LUE_SKIP;

					tekstit.push_back("");
					titles.push_back("");
					table_index++;
					
				}
				break;

			case MARKER_2:
				if (read == LUE_OTSIKKO) {
					read = LUE_TEKSTI;
					break;
				}
				if (read == LUE_TEKSTI) {
					tekstit[table_index] += marker;
				}
				break;

			case '\r':
			case '\n':
				if (read != LUE_SKIP){
					read = LUE_SKIP;
					
					tekstit.push_back("");
					titles.push_back("");
					table_index++;
					
				}
				break;

			case '\t': break;
			case '\v': break;

			default:
				if (read != LUE_SKIP){
					if (read == LUE_OTSIKKO) {
						titles[table_index] += marker;
					}
					if (read == LUE_TEKSTI) {
						tekstit[table_index] += marker;
					}
				}
				break;
		}
	}

    io->close();
	return true;
}

int PLang::Search_Id(const char *text) {

	size_t i;
	for (i = 0; i < titles.size(); i++)
		if (titles[i] == text) break;

	if (i >= titles.size())
		return -1;

	return i;

}

const char* PLang::Get_Text(size_t index) {
	if (index < tekstit.size())
		return tekstit[index].c_str(); //?
	else
		return ".....";
}

int PLang::Set_Text(const char* title, const char* text) {

	int idx = this->Search_Id(title);
	if (idx != -1) {
	
		tekstit[idx] = text;
		return idx;
	
	}

	titles.push_back(title);
	tekstit.push_back(text);
	return titles.size() - 1;

}