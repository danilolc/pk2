//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/platform.hpp"
#include "engine/PFile.hpp"

typedef struct {
	int left, top, right, bottom;
} MAP_RECT;

const char PK2MAP_LAST_VERSION[4] = "1.3";

const u32 PK2MAP_MAP_WIDTH  = 256;
const u32 PK2MAP_MAP_HEIGHT = 224;
const u32 PK2MAP_MAP_SIZE   = PK2MAP_MAP_WIDTH * PK2MAP_MAP_HEIGHT;
const u32 PK2MAP_BLOCK_PALETTE_WIDTH  = 320;
const u32 PK2MAP_BLOCK_PALETTE_HEIGHT = 480;

const u8 PK2KARTTA_TAUSTAKUVA_EI      = 0;
const u8 PK2KARTTA_EXTRA_EI           = 0;

const u32 PK2MAP_MAP_MAX_PROTOTYPES = 100;

enum {

	BLOCK_ESTO_ALAS = 40,
	BLOCK_HISSI_HORI,
	BLOCK_HISSI_VERT,
	BLOCK_KYTKIN2_YLOS,
	BLOCK_KYTKIN3_OIKEALLE,
	BLOCK_KYTKIN2_ALAS,
	BLOCK_KYTKIN3_VASEMMALLE,
	BLOCK_LUKKO,
	BLOCK_KALLOSEINA,
	BLOCK_KALLOTAUSTA,

	BLOCK_ANIM1 = 60,
	BLOCK_ANIM2 = 65,
	BLOCK_ANIM3 = 70,
	BLOCK_ANIM4 = 75,

	BLOCK_VIRTA_VASEMMALLE = 140,
	BLOCK_VIRTA_OIKEALLE,
	BLOCK_VIRTA_YLOS,
	BLOCK_PIILO,
	BLOCK_TULI,
	BLOCK_KYTKIN1,
	BLOCK_KYTKIN2,
	BLOCK_KYTKIN3,
	BLOCK_ALOITUS,
	BLOCK_LOPETUS,

};

const int SWITCH_INITIAL_VALUE  = 2000;

const u8 ILMA_NORMAALI			= 0;
const u8 ILMA_SADE				= 1;
const u8 ILMA_METSA				= 2;
const u8 ILMA_SADEMETSA			= 3;
const u8 ILMA_LUMISADE			= 4;

const u8 TAUSTA_STAATTINEN			= 0;
const u8 TAUSTA_PALLARX_VERT			= 1;
const u8 TAUSTA_PALLARX_HORI			= 2;
const u8 TAUSTA_PALLARX_VERT_JA_HORI	= 3;

void MapClass_Animoi(int degree, int anim, int aika1, int aika2, int aika3, bool keys);

class MapClass
{
	public:

	/* Atributs ------------------------*/

	char		versio[5];			// map version. eg "1.3"
	char		palikka_bmp[13];	// path of block palette .bmp
	char		taustakuva[13];		// path of map bg .bmp
	char		musiikki[13];		// path of map music

	char		nimi[40];			// map name
	char		tekija[40];			// map author

	int			jakso;				// level of the episode
	int			ilma;				// map climate
	int			aika;				// map time (in (dec)conds)
	u8		extra;				// extra config - not used
	u8		tausta;				// bg movemant type
	u32		kytkin1_aika;		// button 1 time - not used
	u32		kytkin2_aika;		// button 2 time - not used
	u32		kytkin3_aika;		// button 3 time - not used
	int			pelaaja_sprite;		// player prototype

	u8		taustat[PK2MAP_MAP_SIZE];	// map bg tiles 256*224
	u8		seinat [PK2MAP_MAP_SIZE];	// map fg tiles 256*224
	u8		spritet[PK2MAP_MAP_SIZE];	// map sprites 256*224
	char		protot [PK2MAP_MAP_MAX_PROTOTYPES][13]; // map prototype list .spr
	bool		reunat [PK2MAP_MAP_SIZE]; // map edges - calculated during game

	int			tiles_buffer;		// index of block palette
	int			bg_tiles_buffer;
	int			background_buffer;	// index of bg image
	int			water_buffer; // index of water palette
	int			bg_water_buffer;

	int	  x,y;						// map icon pos
	int	  icon;					// map icon id

	//static char	pk2_hakemisto[256]; // PK2.exe:n sis�lt�v� hakemisto

	/* Metodit --------------------------*/

	MapClass();						// Oletusmuodostin
	MapClass(const MapClass &kartta);	// Kopiointimuodostin
	~MapClass();						// Hajoitin

	MapClass &operator = (const MapClass &kartta);	//Sijoitusoperaattori

	int Load(PFile::Path path);		// Load kartta
	int Load_Plain_Data(PFile::Path path);	// Load kartta ilman grafiikoita

	void Tyhjenna();				// clean map
	MAP_RECT LaskeTallennusAlue(u8 *lahde, u8 *&kohde);
	MAP_RECT LaskeTallennusAlue(u8 *alue);
	void LueTallennusAlue(u8 *lahde, MAP_RECT alue, int kohde);
	int Piirra_Taustat(int kamera_x, int kamera_y, bool editor);
	int Piirra_Seinat (int kamera_x, int kamera_y, bool editor);
	void Kopioi(const MapClass &kartta);

	//PK2 functions
	int Count_Keys();
	void Change_SkullBlocks();
	void Open_Locks();
	void Calculate_Edges();
	void Select_Start();
	void Place_Sprites();

	private:

	int LoadVersion01(PFile::Path path);
	int LoadVersion10(PFile::Path path);
	int LoadVersion11(PFile::Path path);
	int LoadVersion12(PFile::Path path);
	int LoadVersion13(PFile::Path path);
	
	int Load_BG(PFile::Path path);
	int Load_TilesImage(PFile::Path path);
	int Load_BGSfx(PFile::Path path);

	void Animate_Fire(int tiles);
	void Animate_Waterfall(int tiles);
	void Animate_RollUp(int tiles);
	void Animate_WaterSurface(int tiles);
	void Animate_Water(int tiles, int water_tiles);
	
};
