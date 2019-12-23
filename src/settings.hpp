#pragma once

#include "types.hpp"

struct PK2SETTINGS {
	char versio[4];
	bool ladattu; // if it was started here
	char kieli[128]; // language

	// grafiikka
	DWORD ruudun_korkeus; //Not used
	DWORD ruudun_leveys; //Not used
	bool  lapinakyvat_objektit;
	bool  lapinakyvat_menutekstit;
	bool  saa_efektit;
	bool  nayta_tavarat;
	bool  tausta_spritet;

	// kontrollit
	DWORD control_left;
	DWORD control_right;
	DWORD control_jump;
	DWORD control_down;
	DWORD control_walk_slow;
	DWORD control_attack1;
	DWORD control_attack2;
	DWORD control_open_gift;

	// audio
	bool musiikki;
	bool aanet;

	//Version 1.1
	bool isFullScreen;
	bool isFiltered;
	bool isFit;

	bool isWide;

	//Version 1.2
	BYTE music_max_volume;
	BYTE sfx_max_volume;

};

int settings_open(PK2SETTINGS* settings);
int settings_save(PK2SETTINGS* settings);