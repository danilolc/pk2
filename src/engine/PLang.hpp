//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

// TODO - use unordered_map

#include <string>
#include <vector>

#include "PFile.hpp"
#include "types.hpp"

class PLang {

	public:

		bool loaded = false;

		PLang();
		PLang(PFile::Path path);
		~PLang();
		bool Read_File(PFile::Path path);

		const std::string& getString(int id, const std::string& def)const;
		bool getBoolean(int id, bool def)const;
		int getInteger(int id, int def)const;

		const std::string& getString(const std::string& title, const std::string& def)const{
			return this->getString(this->Search_Id(title), def);
		}
		bool getBoolean(const std::string& title, bool def)const{
			return this->getBoolean(this->Search_Id(title), def);
		}

		int getInteger(const std::string& title, int def)const{
			return this->getInteger(this->Search_Id(title), def);
		}

		int Set_Text(const std::string& title, const char* text);
		/**
		 * @brief 
		 * TODO
		 * Replace it with getString
		 */
		const std::string& Get_Text(int index)const;
		int Search_Id(const std::string& title)const;

		int searchLocalizedText(const std::string& title);
	private:

		static const std::string PLACEHOLDER;

		//std::vector<std::pair<std::string, std::string>> texts;
		
		std::vector<std::string> values;
		std::vector<std::string> keys;
	
};
