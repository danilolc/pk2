//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "blocks.hpp"

#include "game/game.hpp"
#include "game/map.hpp"
#include "system.hpp"

void PK_Block_Set_Barriers(PK2BLOCK &block) {

	block.tausta = false;

	block.oikealle	= BLOCK_WALL;
	block.vasemmalle	= BLOCK_WALL;
	block.ylos		= BLOCK_WALL;
	block.alas		= BLOCK_WALL;

	// Special Floor

	if (block.koodi > 139){
		block.oikealle	= BLOCK_BACKGROUND;
		block.vasemmalle	= BLOCK_BACKGROUND;
		block.ylos		= BLOCK_BACKGROUND;
		block.alas		= BLOCK_BACKGROUND;
	}

	// Lifts

	if (block.koodi == BLOCK_HISSI_HORI){
		block.vasen += (int)cos_table[degree%360];
		block.oikea += (int)cos_table[degree%360];
	}
	if (block.koodi == BLOCK_HISSI_VERT){
		block.ala += (int)sin_table[degree%360];
		block.yla += (int)sin_table[degree%360];
	}

	// Walk-through Floor

	if (block.koodi == BLOCK_ESTO_ALAS){
		block.oikealle	= BLOCK_BACKGROUND;
		block.ylos		= BLOCK_BACKGROUND;
		block.alas		= BLOCK_WALL;
		block.vasemmalle	= BLOCK_BACKGROUND;
		block.ala -= 27;
	}

	// Hill

	if (block.koodi > 49 && block.koodi < 60){
		block.oikealle	= BLOCK_BACKGROUND;
		block.ylos		= BLOCK_WALL;
		block.alas		= BLOCK_WALL;
		block.vasemmalle	= BLOCK_BACKGROUND;
		block.ala += 1;
	}

	// Switches

	if (block.koodi >= BLOCK_KYTKIN1 && block.koodi <= BLOCK_KYTKIN3){
		block.oikealle	= BLOCK_WALL;
		block.ylos		= BLOCK_WALL;
		block.alas		= BLOCK_WALL;
		block.vasemmalle	= BLOCK_WALL;
	}

	// Switches Affected Floors

	int kytkin1_y = 0,
		kytkin2_y = 0,
		kytkin3_x = 0;

	if (Game->button1 > 0){
		kytkin1_y = 64;

		if (Game->button1 < 64)
			kytkin1_y = Game->button1;

		if (Game->button1 > KYTKIN_ALOITUSARVO-64)
			kytkin1_y = KYTKIN_ALOITUSARVO - Game->button1;
	}

	if (Game->button2 > 0){
		kytkin2_y = 64;

		if (Game->button2 < 64)
			kytkin2_y = Game->button2;

		if (Game->button2 > KYTKIN_ALOITUSARVO-64)
			kytkin2_y = KYTKIN_ALOITUSARVO - Game->button2;
	}

	if (Game->button3 > 0){
		kytkin3_x = 64;

		if (Game->button3 < 64)
			kytkin3_x = Game->button3;

		if (Game->button3 > KYTKIN_ALOITUSARVO-64)
			kytkin3_x = KYTKIN_ALOITUSARVO - Game->button3;
	}


	if (block.koodi == BLOCK_KYTKIN2_YLOS){
		block.ala -= kytkin2_y/2;
		block.yla -= kytkin2_y/2;
	}

	if (block.koodi == BLOCK_KYTKIN2_ALAS){
		block.ala += kytkin2_y/2;
		block.yla += kytkin2_y/2;
	}

	if (block.koodi == BLOCK_KYTKIN2){
		block.ala += kytkin2_y/2;
		block.yla += kytkin2_y/2;
	}

	if (block.koodi == BLOCK_KYTKIN3_OIKEALLE){
		block.oikea += kytkin3_x/2;
		block.vasen += kytkin3_x/2;
		block.koodi = BLOCK_HISSI_HORI; // samat idea sivusuuntaan ty�nn�ss�
	}

	if (block.koodi == BLOCK_KYTKIN3_VASEMMALLE){
		block.oikea -= kytkin3_x/2;
		block.vasen -= kytkin3_x/2;
		block.koodi = BLOCK_HISSI_HORI; // samat idea sivusuuntaan ty�nn�ss�
	}

	if (block.koodi == BLOCK_KYTKIN3){
		block.ala += kytkin3_x/2;
		block.yla += kytkin3_x/2;
	}

	if (block.koodi == BLOCK_KYTKIN1){
		block.ala += kytkin1_y/2;
		block.yla += kytkin1_y/2;
	}

}

PK2BLOCK PK_Block_Get(int x, int y) {

	PK2BLOCK block;

    // Outside the screen
	if (x < 0 || x > PK2KARTTA_KARTTA_LEVEYS || y < 0 || y > PK2KARTTA_KARTTA_KORKEUS) {
		block.koodi  = 255;
		block.tausta = true;
		block.vasen  = x*32;
		block.oikea  = x*32+32;
		block.yla	   = y*32;
		block.ala    = y*32+32;
		block.water   = false;
		block.border  = true;
		return block;
	}

	BYTE i = Game->map->seinat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	if (i<150){ //If it is ground
		block = Game->lasketut_palikat[i];
		block.vasen  = x*32+Game->lasketut_palikat[i].vasen;
		block.oikea  = x*32+32+Game->lasketut_palikat[i].oikea;
		block.yla	   = y*32+Game->lasketut_palikat[i].yla;
		block.ala    = y*32+32+Game->lasketut_palikat[i].ala;
	}
	else{ //If it is sky - Need to reset
		block.koodi  = 255;
		block.tausta = true;
		block.vasen  = x*32;
		block.oikea  = x*32+32;
		block.yla	   = y*32;
		block.ala    = y*32+32;
		block.water   = false;

		block.vasemmalle = 0;
		block.oikealle = 0;
		block.ylos = 0;
		block.alas = 0;
	}

	i = Game->map->taustat[x+y*PK2KARTTA_KARTTA_LEVEYS];

	if (i>131 && i<140)
		block.water = true;

	block.border = Game->map->reunat[x+y*PK2KARTTA_KARTTA_LEVEYS];


	return block;
}