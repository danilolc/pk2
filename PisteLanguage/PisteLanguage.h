#ifndef PISTELANGUAGE
#define PISTELANGUAGE

const int MAX_TEKSTEJA = 200;
const int MAX_TEKSTIN_PITUUS = 80;
const int MAX_OTSIKON_PITUUS = 50;//30

const int	LUE_SKIP	= 0,
			LUE_OTSIKKO = 1,
			LUE_TEKSTI	= 2;

const char	EROTIN_1	= '*',
			EROTIN_2	= ':';

class PisteLanguage
{
public:

	PisteLanguage();
	PisteLanguage(char *filename);
	~PisteLanguage();

	bool Lue_Tiedosto(char *filename);
	int Hae_Indeksi(char *title);
	char *Hae_Teksti(int index);
	void Korvaa_Teksti(int index, char *teksti);

	//PisteLanguage *file;

private:
	// 200 kpl tekstejä 
	char tekstit[MAX_TEKSTEJA][MAX_TEKSTIN_PITUUS+1];
	// tekstien otsikot
	char otsikot[MAX_TEKSTEJA][MAX_OTSIKON_PITUUS+1];

	int  lue; 
};

#endif