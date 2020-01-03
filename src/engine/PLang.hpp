//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"

const int MAX_TEXTS = 200;
const int MAX_TEXT_LENGTH = 80;
const int MAX_HEAD_LENGTH = 50;

class PLang{
	public:
		PLang();
		PLang(char *filename); // TODO - use PFile::Path
		~PLang();
		bool Read_File(char *filename); // TODO - use PFile::Path
		int Hae_Indeksi(const char *title);
		const char* Get_Text(int index);
		void Replace_Text(int index, const char *teksti);

	private:
		char tekstit[MAX_TEXTS][MAX_TEXT_LENGTH+1];
		char otsikot[MAX_TEXTS][MAX_HEAD_LENGTH+1];
		int  read;
};
