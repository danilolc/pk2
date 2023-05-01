//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

// TODO - use unordered_map

#include <string>
#include <vector>

#include "engine/PFile.hpp"
#include "engine/platform.hpp"

class PLang {

	public:

		bool loaded = false;

		PLang();
		PLang(PFile::Path path);
		~PLang();
		bool Read_File(PFile::Path path);
		int Search_Id(const char *title);
		const char* Get_Text(size_t index);
		int Set_Text(const char* title, const char* text);

	private:
	
		std::vector<std::string> tekstit;
		std::vector<std::string> titles;
	
};
