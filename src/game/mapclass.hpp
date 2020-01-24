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

const char PK2KARTTA_VIIMEISIN_VERSIO[4] = "1.3";

const u32 PK2KARTTA_KARTTA_LEVEYS  = 256;
const u32 PK2KARTTA_KARTTA_KORKEUS = 224;
const u32 PK2KARTTA_KARTTA_KOKO	 = PK2KARTTA_KARTTA_LEVEYS * PK2KARTTA_KARTTA_KORKEUS;
const u32 PK2KARTTA_BLOCK_PALETTI_LEVEYS  = 320;
const u32 PK2KARTTA_BLOCK_PALETTI_KORKEUS = 480;
const u8 PK2KARTTA_TAUSTAKUVA_EI	 = 0;
const u8 PK2KARTTA_EXTRA_EI		 = 0;

const u32 PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA = 100;

const u8 BLOCK_ESTO_ALAS		= 40;
const u8 BLOCK_HISSI_HORI		= 41;
const u8 BLOCK_HISSI_VERT		= 42;
const u8 BLOCK_KYTKIN2_YLOS	= 43;
const u8 BLOCK_KYTKIN2_ALAS	= 45;
const u8 BLOCK_KYTKIN3_OIKEALLE = 44;
const u8 BLOCK_KYTKIN3_VASEMMALLE = 46;
const u8 BLOCK_LUKKO			= 47;
const u8 BLOCK_KALLOSEINA		= 48;
const u8 BLOCK_KALLOTAUSTA		= 49;
const u8 BLOCK_ANIM1			= 60;
const u8 BLOCK_ANIM2			= 65;
const u8 BLOCK_ANIM3			= 70;
const u8 BLOCK_ANIM4			= 75;
const u8 BLOCK_VIRTA_VASEMMALLE= 140;
const u8 BLOCK_VIRTA_OIKEALLE	= 141;
const u8 BLOCK_VIRTA_YLOS		= 142;
const u8 BLOCK_PIILO			= 143;
const u8 BLOCK_TULI			= 144;
const u8 BLOCK_KYTKIN1			= 145;
const u8 BLOCK_KYTKIN2			= 146;
const u8 BLOCK_KYTKIN3			= 147;
const u8 BLOCK_ALOITUS			= 148;
const u8 BLOCK_LOPETUS			= 149;

const int KYTKIN_ALOITUSARVO		= 2000;

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
void MapClass_Set_Screen_Size(int width, int height);

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
	int			aika;				// map time
	u8		extra;				// extra config - not used
	u8		tausta;				// bg movemant type
	u32		kytkin1_aika;		// button 1 time - not used
	u32		kytkin2_aika;		// button 2 time - not used
	u32		kytkin3_aika;		// button 3 time - not used
	int			pelaaja_sprite;		// player prototype

	u8		taustat[PK2KARTTA_KARTTA_KOKO];	// map bg tiles 256*224
	u8		seinat [PK2KARTTA_KARTTA_KOKO];	// map fg tiles 256*224
	u8		spritet[PK2KARTTA_KARTTA_KOKO];	// map sprites 256*224
	char		protot [PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA][13]; // map prototype list .spr
	bool		reunat [PK2KARTTA_KARTTA_KOKO]; // map edges - calculated during game

	int			palikat_buffer;		// index of block palette
	int			taustakuva_buffer;	// index of bg image
	int			palikat_vesi_buffer; // index of water palette

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

	int Tallenna(char *filename);	// Save map
	void Tyhjenna();				// clean map
	MAP_RECT LaskeTallennusAlue(u8 *lahde, u8 *&kohde);
	MAP_RECT LaskeTallennusAlue(u8 *alue);
	void LueTallennusAlue(u8 *lahde, MAP_RECT alue, int kohde);
	int Piirra_Taustat(int kamera_x, int kamera_y, bool editor);
	int Piirra_Seinat (int kamera_x, int kamera_y, bool editor);
	void Kopioi(MapClass &kartta);

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
	int Load_BlockPalette(PFile::Path path);
	int Load_BGSfx(PFile::Path path);

	void Animoi_Tuli(void);
	void Animoi_Vesiputous(void);
	void Animoi_Virta_Ylos(void);
	void Animoi_Vedenpinta(void);
	void Animoi_Vesi(void);
	
};
