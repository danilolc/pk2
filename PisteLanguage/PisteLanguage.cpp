#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include <iostream.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h> 

#include "PisteLanguage.h"

PisteLanguage::PisteLanguage()
{
	lue = LUE_SKIP;

	for (int i=0;i<MAX_TEKSTEJA;i++)
	{
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}

}

PisteLanguage::PisteLanguage(char *tiedosto)
{
	lue = LUE_SKIP;

	for (int i=0;i<MAX_TEKSTEJA;i++)
	{
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}

	Lue_Tiedosto(tiedosto);
}

PisteLanguage::~PisteLanguage(){}

bool PisteLanguage::Lue_Tiedosto(char *filename)
{
	
	ifstream *tiedosto = new ifstream(filename, ios::in | ios::nocreate);

	if (tiedosto->fail())
	{
		delete (tiedosto);
		return false;
	}

	for (int i=0;i<MAX_TEKSTEJA;i++)
	{
		strcpy(tekstit[i],"");
		strcpy(otsikot[i],"");
	}	

	char merkki;
	int taulukko_index = 0;
	int mjono_index = 0;
	lue = LUE_SKIP;

	bool jatka = true;

	while(jatka && tiedosto->peek() != EOF)
	{
		//tiedosto->read(merkki, sizeof(merkki));
	
		merkki = tiedosto->get();

		switch (merkki)
		{
		case EROTIN_1	:	if (lue == LUE_SKIP)
							{
								lue = LUE_OTSIKKO;
								mjono_index = 0;
							}
							else
							{
								lue = LUE_SKIP;
								taulukko_index++;
							}
							break;
		
		case EROTIN_2	:	if (lue == LUE_OTSIKKO)
							{
								lue = LUE_TEKSTI;
								mjono_index = 0;
								break;
							}

							if (lue == LUE_TEKSTI)
							{
								if (mjono_index < MAX_TEKSTIN_PITUUS)
								{
									tekstit[taulukko_index][mjono_index] = merkki;
									tekstit[taulukko_index][mjono_index+1] = '\0';
									mjono_index++;
								}
							}
							break;
		
		case '\n'		:	if (lue != LUE_SKIP)
							{
								lue = LUE_SKIP;
								taulukko_index++;
							}
							break;

		case '\t'		:	break;
		case '\v'		:	break;
							
		default			:	if (lue != LUE_SKIP && !(mjono_index == 0 && merkki == ' '))
							{
								if (lue == LUE_OTSIKKO)
								{
									if (mjono_index < MAX_OTSIKON_PITUUS)
									{
										//strcat(otsikot[taulukko_index],(char *)merkki);
										otsikot[taulukko_index][mjono_index] = merkki;
										otsikot[taulukko_index][mjono_index+1] = '\0';
										mjono_index++;
									}
								}
								if (lue == LUE_TEKSTI)
								{
									if (mjono_index < MAX_TEKSTIN_PITUUS)
									{
										//strcat(tekstit[taulukko_index],(char *)merkki);
										tekstit[taulukko_index][mjono_index] = merkki;
										tekstit[taulukko_index][mjono_index+1] = '\0';
										mjono_index++;
									}
								}
							}
							break;
							
		}
	
		if (taulukko_index >= MAX_TEKSTEJA)
			jatka = false;
	}

	delete tiedosto;
	
	return true;
}

int PisteLanguage::Hae_Indeksi(char *otsikko)
{
	int i=0;

	while (i < MAX_TEKSTEJA && strcmp(otsikot[i],otsikko) != 0)
		i++;

	if (i == MAX_TEKSTEJA)
		return -1;

	return i;

}

char *PisteLanguage::Hae_Teksti(int index)
{
	if (index >= 0 && index < MAX_TEKSTEJA)
		return tekstit[index];
	else
		return ".....";
}

void PisteLanguage::Korvaa_Teksti(int index, char *teksti)
{
	if (index >= 0 && index < MAX_TEKSTEJA)
		strcpy(tekstit[index],teksti);
}