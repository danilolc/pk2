#pragma once

#include "platform.hpp"

#include "episode.hpp"

const int MAX_SAVES = 10;

struct PK2SAVE{
	int   jakso;
	char  episodi[PE_PATH_SIZE];
	char  nimi[20];
	bool  kaytossa;
	bool  jakso_lapaisty[EPISODI_MAX_LEVELS];
	DWORD pisteet;
};

extern PK2SAVE tallennukset[MAX_SAVES];