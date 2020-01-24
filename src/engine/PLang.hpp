//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/PFile.hpp"
#include "engine/platform.hpp"

const int MAX_TEXTS = 200;
const int MAX_TEXT_LENGTH = 80;
const int MAX_HEAD_LENGTH = 50;

class PLang{
	public:
		PLang();
		PLang(PFile::Path path);
		~PLang();
		bool Read_File(PFile::Path path);
		int Hae_Indeksi(const char *title);
		const char* Get_Text(int index);
		void Replace_Text(int index, const char *teksti);

	private:
		char tekstit[MAX_TEXTS][MAX_TEXT_LENGTH+1];
		char otsikot[MAX_TEXTS][MAX_HEAD_LENGTH+1];
		int  read;
};
