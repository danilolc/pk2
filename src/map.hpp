//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################

#ifndef PK2MAP
#define PK2MAP

#include "platform.hpp"

#include <cstring>
#include <string>

#include <vector>

typedef struct {
	int left, top, right, bottom;
} RECT;

enum UNLOCK {
	SCORE = 0, COLLECTABLES
};

const char PK2KARTTA_VIIMEISIN_VERSIO[4] = "1.3";

const DWORD PK2KARTTA_KARTTA_LEVEYS  = 256;
const DWORD PK2KARTTA_KARTTA_KORKEUS = 224;
const DWORD PK2KARTTA_KARTTA_KOKO	 = PK2KARTTA_KARTTA_LEVEYS * PK2KARTTA_KARTTA_KORKEUS;
const DWORD PK2KARTTA_BLOCK_PALETTI_LEVEYS  = 320;
const DWORD PK2KARTTA_BLOCK_PALETTI_KORKEUS = 480;
const BYTE PK2KARTTA_TAUSTAKUVA_EI	 = 0;
const BYTE PK2KARTTA_EXTRA_EI		 = 0;

const DWORD PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA = 100;

const BYTE BLOCK_ESTO_ALAS		= 40;
const BYTE BLOCK_HISSI_HORI		= 41;
const BYTE BLOCK_HISSI_VERT		= 42;
const BYTE BLOCK_KYTKIN2_YLOS	= 43;
const BYTE BLOCK_KYTKIN2_ALAS	= 45;
const BYTE BLOCK_KYTKIN3_OIKEALLE = 44;
const BYTE BLOCK_KYTKIN3_VASEMMALLE = 46;
const BYTE BLOCK_LUKKO			= 47;
const BYTE BLOCK_KALLOSEINA		= 48;
const BYTE BLOCK_KALLOTAUSTA		= 49;
const BYTE BLOCK_ANIM1			= 60;
const BYTE BLOCK_ANIM2			= 65;
const BYTE BLOCK_ANIM3			= 70;
const BYTE BLOCK_ANIM4			= 75;
const BYTE BLOCK_VIRTA_VASEMMALLE= 140;
const BYTE BLOCK_VIRTA_OIKEALLE	= 141;
const BYTE BLOCK_VIRTA_YLOS		= 142;
const BYTE BLOCK_PIILO			= 143;
const BYTE BLOCK_TULI			= 144;
const BYTE BLOCK_KYTKIN1			= 145;
const BYTE BLOCK_KYTKIN2			= 146;
const BYTE BLOCK_KYTKIN3			= 147;
const BYTE BLOCK_ALOITUS			= 148;
const BYTE BLOCK_LOPETUS			= 149;

const int SWITCH_TIME_DEFAULT		= 2000;

const BYTE ILMA_NORMAALI			= 0;
const BYTE ILMA_SADE				= 1;
const BYTE ILMA_METSA				= 2;
const BYTE ILMA_SADEMETSA			= 3;
const BYTE ILMA_LUMISADE			= 4;

const BYTE TAUSTA_STAATTINEN			= 0;
const BYTE TAUSTA_PALLARX_VERT			= 1;
const BYTE TAUSTA_PALLARX_HORI			= 2;
const BYTE TAUSTA_PALLARX_VERT_JA_HORI	= 3;

void PK2Kartta_Cos_Sin(double *cost, double *sint);
void PK2Kartta_Animoi(int degree, int anim, int aika1, int aika2, int aika3, bool keys);
void PK2Kartta_Aseta_Ruudun_Mitat(int leveys, int korkeus);

class PK2Kartta {
	public:

	char		versio[5];			// map version. eg "1.3"
	std::string	palikka_bmp;	// path of block palette .bmp
	std::string taustakuva;		// path of map bg .bmp
	std::string musiikki;		// path of map music

	std::string		map_name;			// map name
	std::string		tekija;			// map author

	int			jakso;				// level of the episode. 0 means secret level
	int			ilma;				// map climate
	int			aika = 0;				// map time
	BYTE		extra;				// extra config - not used
	BYTE		tausta;				// bg movemant type
	DWORD		kytkin1_aika;		// button 1 time
	DWORD		kytkin2_aika;		// button 2 time
	DWORD		kytkin3_aika;		// button 3 time
	int			pelaaja_sprite;		// player prototype

	BYTE		taustat[PK2KARTTA_KARTTA_KOKO];	// map bg tiles 256*224
	BYTE		seinat [PK2KARTTA_KARTTA_KOKO];	// map fg tiles 256*224
	BYTE		spritet[PK2KARTTA_KARTTA_KOKO];	// map sprites 256*224
	char protot [PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA][13]; // map prototype list .spr
	
	std::vector<std::string> prototypes;


	bool		reunat [PK2KARTTA_KARTTA_KOKO]; // map edges - calculated during game

	int			palikat_buffer;		// index of block palette
	int			taustakuva_buffer;	// index of bg image
	int			palikat_vesi_buffer; // index of water palette

	int	  x,y;						// map icon pos
	int	  ikoni;					// map icon id

	int background_width;
	int background_height;

	int level_number_secret;

	int collectable_amount;

	int unlock;
	int unlock_score;
	int unlock_collectables;
	bool is_secret = false;

	int collectables;

	PK2Kartta();						// Oletusmuodostin
	PK2Kartta(const PK2Kartta &kartta);	// Kopiointimuodostin
	~PK2Kartta();						// Hajoitin

	int Load(std::string filename, std::string episode);		// Lataa kartta
	int Lataa_Pelkat_Tiedot(char *polku, const char *nimi);	// Lataa kartta ilman grafiikoita

	void Tyhjenna();				// clean map
	RECT LaskeTallennusAlue(BYTE *lahde, BYTE *&kohde);
	RECT LaskeTallennusAlue(BYTE *alue);
	void LueTallennusAlue(BYTE *lahde, RECT alue, int kohde);
	int Piirra_Taustat(int, int, bool);
	int Piirra_Seinat (int, int, bool);

	//PK2 functions
	int Count_Keys();
	void Change_SkullBlocks();
	void Open_Locks();
	void Calculate_Edges();
	void Select_Start();
	void Place_Sprites();

	private:

	int LataaVersio10(char *filename);	// Lataa kartta versio 1.0
	int LataaVersio11(char *filename);	// Lataa kartta versio 1.1
	int LataaVersio12(char *filename);  // Lataa kartta versio 1.2
	int LoadVersion13(std::string filename);  // Lataa kartta versio 1.3
	int LoadVersion14(std::string filename);  // Load map version 1.4
	
	int Load_Background(std::string filename, std::string episode);
	int Load_Tileset(std::string filename, std::string episode);
	int Lataa_TaustaMusiikki(char *filename);

	void Animoi_Tuli(void);
	void Animoi_Vesiputous(void);
	void Animoi_Virta_Ylos(void);
	void Animoi_Vedenpinta(void);
	void Animoi_Vesi(void);
	
};
#endif
