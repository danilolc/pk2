//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/types.hpp"

const BYTE BLOCK_MAX_MASKS = 150;

enum BLOCKS {

	BLOCK_BACKGROUND,
	BLOCK_WALL,
	BLOCK_MAKI_OIKEA_YLOS, //unnused
	BLOCK_MAKI_VASEN_YLOS, //unnused
	BLOCK_MAKI_OIKEA_ALAS, //unnused
	BLOCK_MAKI_VASEN_ALAS, //unnused
	BLOCK_MAKI_YLOS,       //unnused
	BLOCK_MAKI_ALAS        //unnused

};

struct PK2BLOCK {

	BYTE	koodi;
	bool	tausta;
	BYTE	vasemmalle, oikealle, ylos, alas;
	int 	vasen, oikea, yla, ala;
	bool	water;
	bool	border;

};

struct PK2BLOCKMASK {

	short int	ylos[32];
	short int	alas[32];
	short int	vasemmalle[32];
	short int	oikealle[32];

};

void Block_Set_Barriers(PK2BLOCK &palikka);

PK2BLOCK Block_Get(int x, int y);