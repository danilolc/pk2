#ifndef PK2_SETTINGS
#define PK2_SETTINGS

//PK2RECORDING ??
struct PK2TALLENNUS{
	int   jakso;
	char  episodi[_MAX_PATH];
	char  nimi[20];
	bool  kaytossa;
	bool  jakso_lapaisty[EPISODI_MAX_JAKSOJA];
	DWORD pisteet;
};

struct PK2SETTINGS {
	double versio;
	bool ladattu; // if it was started here
	char kieli[128]; // language

	// grafiikka
	DWORD ruudun_korkeus;
	DWORD ruudun_leveys;
	bool  lapinakyvat_objektit;
	bool  lapinakyvat_menutekstit;
	bool  saa_efektit;
	bool  nayta_tavarat;
	bool  tausta_spritet;

	// kontrollit
	DWORD kontrolli_vasemmalle;
	DWORD kontrolli_oikealle;
	DWORD kontrolli_hyppy;
	DWORD kontrolli_alas;
	DWORD kontrolli_juoksu;
	DWORD kontrolli_hyokkays1;
	DWORD kontrolli_hyokkays2;
	DWORD kontrolli_kayta_esine;

	// audio
	bool musiikki;
	bool aanet;

	//Version 1.1
	bool isFullScreen;
	bool isFiltered;
	bool isFit;

};


int   musiikin_voimakkuus = 64;
int   musiikin_max_voimakkuus = 64;
int   musiikin_voimakkuus_nyt = 64;

bool doublespeed = false;
bool skip_frame = false;

int   fps = 0;
bool  fps_nayta = false;



//PK_Settings_Start
void PK_Asetukset_Alusta(){
	settings.ladattu = false;
	strcpy(settings.kieli,"english.txt");

	settings.ruudun_leveys = SCREEN_WIDTH;
	settings.ruudun_korkeus = SCREEN_HEIGHT;
	settings.lapinakyvat_objektit = true;
	settings.lapinakyvat_menutekstit = false;
	settings.saa_efektit = true;
	settings.nayta_tavarat = true;
	settings.tausta_spritet = true;

	settings.aanet = true;
	settings.musiikki = true;

	settings.kontrolli_vasemmalle	= PI_LEFT;
	settings.kontrolli_oikealle		= PI_RIGHT;
	settings.kontrolli_hyppy		= PI_UP;
	settings.kontrolli_alas			= PI_DOWN;
	settings.kontrolli_juoksu		= PI_RALT;
	settings.kontrolli_hyokkays1	= PI_RCONTROL;
	settings.kontrolli_hyokkays2	= PI_RSHIFT;
	settings.kontrolli_kayta_esine	= PI_SPACE;

	settings.isFiltered = true;
	settings.isFit = true;
	settings.isFullScreen = true;

	PisteUtils_CreateDir("data");
	PK_Asetukset_Tallenna("data/settings.ini");
}
//PK_Settings_Open
int PK_Asetukset_Lataa(char *filename){
	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_Asetukset_Alusta();
		return 1;
	}

	tiedosto->read((char*)versio, 4);

	if (strcmp(versio, "1.0") == 0) {
		delete (tiedosto);
		PK_Asetukset_Alusta();
		return 2;
	}
	if (strcmp(versio, "1.1") == 0)
		tiedosto->read((char*)&settings, sizeof(settings));

	delete (tiedosto);

	settings.ladattu = true;
	kontrolli_vasemmalle	= settings.kontrolli_vasemmalle;
	kontrolli_oikealle		= settings.kontrolli_oikealle;
	kontrolli_hyppy			= settings.kontrolli_hyppy;
	kontrolli_alas			= settings.kontrolli_alas;
	kontrolli_juoksu		= settings.kontrolli_juoksu;
	kontrolli_hyokkays1		= settings.kontrolli_hyokkays1;
	kontrolli_hyokkays2		= settings.kontrolli_hyokkays2;
	kontrolli_kayta_esine	= settings.kontrolli_kayta_esine;

	return 0;
}
//PK_Settings_Save
int PK_Asetukset_Tallenna(char *filename){
	settings.ruudun_korkeus	= RUUDUN_KORKEUS;
	settings.ruudun_leveys		= RUUDUN_LEVEYS;

	settings.kontrolli_vasemmalle		= kontrolli_vasemmalle;
	settings.kontrolli_oikealle		= kontrolli_oikealle;
	settings.kontrolli_hyppy			= kontrolli_hyppy;
	settings.kontrolli_alas			= kontrolli_alas;
	settings.kontrolli_juoksu			= kontrolli_juoksu;
	settings.kontrolli_hyokkays1		= kontrolli_hyokkays1;
	settings.kontrolli_hyokkays2		= kontrolli_hyokkays2;
	settings.kontrolli_kayta_esine		= kontrolli_kayta_esine;

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.1", 4);
	tiedosto->write ((char *)&settings, sizeof (settings));

	delete (tiedosto);

	return 0;
}






#endif
