#ifndef PK2MAP
#define PK2MAP

typedef unsigned short USHORT;
typedef unsigned short WORD; 
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

const char PK2KARTTA_VIIMEISIN_VERSIO[4] = "1.3";

const DWORD PK2KARTTA_KARTTA_LEVEYS  = 256; 
const DWORD PK2KARTTA_KARTTA_KORKEUS = 224;
const DWORD PK2KARTTA_KARTTA_KOKO	 = PK2KARTTA_KARTTA_LEVEYS * PK2KARTTA_KARTTA_KORKEUS;
const DWORD PK2KARTTA_PALIKKA_PALETTI_LEVEYS  = 320;
const DWORD PK2KARTTA_PALIKKA_PALETTI_KORKEUS = 480;
const UCHAR PK2KARTTA_TAUSTAKUVA_EI	 = 0;
const UCHAR PK2KARTTA_EXTRA_EI		 = 0;

const DWORD PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA = 100;

const UCHAR PALIKKA_ESTO_ALAS		= 40;
const UCHAR PALIKKA_HISSI_HORI		= 41;
const UCHAR PALIKKA_HISSI_VERT		= 42;
const UCHAR PALIKKA_KYTKIN2_YLOS	= 43;
const UCHAR PALIKKA_KYTKIN2_ALAS	= 45;
const UCHAR PALIKKA_KYTKIN3_OIKEALLE = 44;
const UCHAR PALIKKA_KYTKIN3_VASEMMALLE = 46;
const UCHAR PALIKKA_LUKKO			= 47;
const UCHAR PALIKKA_KALLOSEINA		= 48;
const UCHAR PALIKKA_KALLOTAUSTA		= 49;
const UCHAR PALIKKA_ANIM1			= 60;
const UCHAR PALIKKA_ANIM2			= 65;
const UCHAR PALIKKA_ANIM3			= 70;
const UCHAR PALIKKA_ANIM4			= 75;
const UCHAR PALIKKA_VIRTA_VASEMMALLE= 140;
const UCHAR PALIKKA_VIRTA_OIKEALLE	= 141;
const UCHAR PALIKKA_VIRTA_YLOS		= 142;
const UCHAR PALIKKA_PIILO			= 143;
const UCHAR PALIKKA_TULI			= 144;
const UCHAR PALIKKA_KYTKIN1			= 145;
const UCHAR PALIKKA_KYTKIN2			= 146;
const UCHAR PALIKKA_KYTKIN3			= 147;
const UCHAR PALIKKA_ALOITUS			= 148;
const UCHAR PALIKKA_LOPETUS			= 149;

const int KYTKIN_ALOITUSARVO		= 2000;

const UCHAR ILMA_NORMAALI			= 0;
const UCHAR ILMA_SADE				= 1;
const UCHAR ILMA_METSA				= 2;
const UCHAR ILMA_SADEMETSA			= 3;
const UCHAR ILMA_LUMISADE			= 4;

const UCHAR TAUSTA_STAATTINEN			= 0;
const UCHAR TAUSTA_PALLARX_VERT			= 1;
const UCHAR TAUSTA_PALLARX_HORI			= 2;
const UCHAR TAUSTA_PALLARX_VERT_JA_HORI	= 3;

void PK2Kartta_Cos_Sin(double *cost, double *sint);
void PK2Kartta_Animoi(int degree, int anim, int aika1, int aika2, int aika3, bool keys);
void PK2Kartta_Aseta_Ruudun_Mitat(int leveys, int korkeus);

class PK2Kartta
{
	public:

	/* Atribuutit ------------------------*/

	char		versio[5];			// kartan versio esim. "12.3"
	char		palikka_bmp[13];	// .BMP -tiedosto, jossa on palikkapaletti
	char		taustakuva[13];		// .BMP -tiedosto, josta ladataan kartan taustakuva. 
	char		musiikki[13];		// tiedoston nimi, jossa on taustamusiikki

	char		nimi[40];			// kartan nimi
	char		tekija[40];			// kartan tekij‰

	int			jakso;				// monesko jakso episodissa
	int			ilma;				// kartan ilmasto
	int			aika;				// aikaraja
	UCHAR		extra;				// erikoisasetus. ei m‰‰ritelty
	UCHAR		tausta;				// tieto miten taustakuva esitet‰‰n. ei m‰‰ritelty
	DWORD		kytkin1_aika;		// kuinka pitk‰‰n kytkin 1 pysyy alhaalla
	DWORD		kytkin2_aika;		// kuinka pitk‰‰n kytkin 2 pysyy alhaalla
	DWORD		kytkin3_aika;		// kuinka pitk‰‰n kytkin 3 pysyy alhaalla
	int			pelaaja_sprite;		// mist‰ prototyypist‰ tehd‰‰n pelaajasprite

	UCHAR		taustat[PK2KARTTA_KARTTA_KOKO];	// kartan kaikki taustat 256*224
	UCHAR		seinat [PK2KARTTA_KARTTA_KOKO];	// kartan kaikki seinat  256*224 
	UCHAR		spritet[PK2KARTTA_KARTTA_KOKO];	// kartan kaikki spritet 256*224
	char		protot [PK2KARTTA_KARTTA_MAX_PROTOTYYPPEJA][13]; // karttaan liittyv‰t spritejen prototyypit (tiedosto.spr)
	bool		reunat [PK2KARTTA_KARTTA_KOKO];
	
	int			palikat_buffer;		// indeksi bufferiin, jossa on kartassa k‰ytett‰v‰t palikat.
	int			taustakuva_buffer;	// indeksi bufferiin, jossa on kartan taustakuva.
	int			palikat_vesi_buffer; // indeksi bufferiin, jossa on vesipalikoiden alkuper‰iset kuvat

	int	  x,y;						// Kartan ikonin sijainti karttaruudulla.
	int	  ikoni;					// Ikonin numero, joka n‰kyy karttaruudulla (icons.bmp kuvasta)

	static char	pk2_hakemisto[256]; // PK2.exe:n sis‰lt‰v‰ hakemisto

	/* Metodit --------------------------*/

	PK2Kartta();						// Oletusmuodostin
	PK2Kartta(const PK2Kartta &kartta);	// Kopiointimuodostin
	~PK2Kartta();						// Hajoitin
	
	PK2Kartta &operator = (const PK2Kartta &kartta);	//Sijoitusoperaattori

	int Lataa(char *polku, char *nimi);		// Lataa kartta
	int Lataa_Pelkat_Tiedot(char *polku, char *nimi);	// Lataa kartta ilman grafiikoita
	int LataaVersio01(char *filename);	// Lataa kartta versio 0.1
	int LataaVersio10(char *filename);	// Lataa kartta versio 1.0
	int LataaVersio11(char *filename);	// Lataa kartta versio 1.1
	int LataaVersio12(char *filename);  // Lataa kartta versio 1.2
	int LataaVersio13(char *filename);  // Lataa kartta versio 1.3
	int Tallenna(char *filename);	// Tallenna kartta (uusimpana versiona)
	void Tyhjenna();				// Tyhjenna kartta
	RECT LaskeTallennusAlue(UCHAR *lahde, UCHAR *&kohde);
	RECT LaskeTallennusAlue(UCHAR *alue);
	void LueTallennusAlue(UCHAR *lahde, RECT alue, int kohde);
	int Lataa_Taustakuva(char *polku, char *filename);
	int Lataa_PalikkaPaletti(char *polku, char *filename);
	int Lataa_TaustaMusiikki(char *filename);
	int Piirra_Taustat(int, int, bool);
	int Piirra_Seinat (int, int, bool);
	void Kopioi(PK2Kartta &kartta);
	void Animoi_Tuli(void);
	void Animoi_Vesiputous(void);
	void Animoi_Virta_Ylos(void);
	void Animoi_Vedenpinta(void);
	void Animoi_Vesi(void);
};

#endif