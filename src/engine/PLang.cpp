//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PLang.hpp"

#include <SDL.h>
#include <cstring>

const int LUE_SKIP    = 0,
          LUE_OTSIKKO = 1,
          LUE_TEKSTI  = 2;

const char MARKER_1 = '*',
           MARKER_2 = ':';

PLang::PLang(){
	read = LUE_SKIP;
	for (int i=0;i<MAX_TEXTS;i++){
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}

}

PLang::PLang(char *tiedosto){
	read = LUE_SKIP;

	for (int i=0;i<MAX_TEXTS;i++){
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}

	Read_File(tiedosto);
}

PLang::~PLang(){}

bool PLang::Read_File(char *filename){

	SDL_RWops *io = SDL_RWFromFile(filename, "r");

	if (io == nullptr)
		return false;

	for (int i = 0; i < MAX_TEXTS; i++) {
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}

	char merkki;
	int taulukko_index = 0;
	int mjono_index = 0;
	read = LUE_SKIP;

	bool jatka = true;

	while(jatka && SDL_RWread(io, &merkki, 1, 1)) {
		switch (merkki){
			case MARKER_1:
				if (read == LUE_SKIP){
					read = LUE_OTSIKKO;
					mjono_index = 0;
				} else{
					read = LUE_SKIP;
					taulukko_index++;
				}
				break;

			case MARKER_2:
				if (read == LUE_OTSIKKO){
					read = LUE_TEKSTI;
					mjono_index = 0;
					break;
				}
				if (read == LUE_TEKSTI){
					if (mjono_index < MAX_TEXT_LENGTH){
						tekstit[taulukko_index][mjono_index] = merkki;
						tekstit[taulukko_index][mjono_index+1] = '\0';
						mjono_index++;
					}
				}
				break;

			case '\r':
			case '\n':
				if (read != LUE_SKIP){
					read = LUE_SKIP;
					taulukko_index++;
				}
				break;

			case '\t': break;
			case '\v': break;

			default:
				if (read != LUE_SKIP && !(mjono_index == 0 && merkki == ' ')){
					if (read == LUE_OTSIKKO){
						if (mjono_index < MAX_HEAD_LENGTH){
							otsikot[taulukko_index][mjono_index] = merkki;
							otsikot[taulukko_index][mjono_index+1] = '\0';
							mjono_index++;
						}
					}
					if (read == LUE_TEKSTI){
						if (mjono_index < MAX_TEXT_LENGTH){
							tekstit[taulukko_index][mjono_index] = merkki;
							tekstit[taulukko_index][mjono_index+1] = '\0';
							mjono_index++;
						}
					}
				}
				break;
		}

		if (taulukko_index >= MAX_TEXTS)
			jatka = false;
	}

    SDL_RWclose(io);
	return true;
}

int PLang::Hae_Indeksi(const char *otsikko){
	int i=0;

	while (i < MAX_TEXTS && strcmp(otsikot[i],otsikko) != 0)
		i++;

	if (i == MAX_TEXTS)
		return -1;

	return i;

}

const char* PLang::Get_Text(int index){
	if (index >= 0 && index < MAX_TEXTS)
		return tekstit[index];
	else
		return ".....";
}

void PLang::Replace_Text(int index, const char *teksti){
	if (index >= 0 && index < MAX_TEXTS)
		strcpy(tekstit[index], teksti);
}
