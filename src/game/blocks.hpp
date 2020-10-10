//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"

const u8 BLOCK_MAX_MASKS = 150;

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

	u8   koodi;
	bool tausta;
	u8   vasemmalle, oikealle, ylos, alas;
	int  vasen, oikea, yla, ala;
	bool water;
	bool border;

};

struct PK2BLOCKMASK {

	short int ylos[32];
	short int alas[32];
	short int vasemmalle[32];
	short int oikealle[32];

};

void Block_Set_Barriers(PK2BLOCK &palikka);

PK2BLOCK Block_Get(u32 x, u32 y);