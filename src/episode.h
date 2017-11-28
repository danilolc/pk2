//Used to manage the episodes

#ifndef PK2_EPISODE
#define PK2_EPISODE

#include "types.h"

const int EPISODI_MAX_JAKSOJA = 100; //= 50;
const int MAX_EPISODEJA	= 300;

struct PK2EPISODIPISTEET{
    DWORD pisteet[EPISODI_MAX_JAKSOJA];
	char top_pelaajat[EPISODI_MAX_JAKSOJA][20];
	DWORD ajat[EPISODI_MAX_JAKSOJA];
	char nopeimmat_pelaajat[EPISODI_MAX_JAKSOJA][20];

	DWORD episodin_top_pisteet;
	char  episodin_top_pelaaja[20];
};

struct PK2JAKSO{
	char	tiedosto[_MAX_PATH];
	char	nimi[40];
	int		x,y;
	int		jarjestys;
	bool	lapaisty;
	int		ikoni;
};

PK2EPISODIPISTEET episodipisteet;

int	jakso = 1;
int jaksoja = 1;
int episodi_lkm = 0;
int jakso_indeksi_nyt = 1;
char episodit[MAX_EPISODEJA][_MAX_PATH];
char episodi[_MAX_PATH];
int  episodisivu = 0;
PK2JAKSO jaksot[EPISODI_MAX_JAKSOJA];
bool jakso_lapaisty = false;
bool uusinta = false;
bool peli_ohi = false;
DWORD lopetusajastin = 0;
DWORD jakso_pisteet = 0;
DWORD fake_pisteet = 0;

#endif
