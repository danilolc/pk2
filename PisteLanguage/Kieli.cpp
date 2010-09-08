#define WIN32_LEAN_AND_MEAN  
#define INITGUID

#include <string.h>
#include <istream.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include "PisteLanguage.h"

int main()
{
	PisteLanguage *kieli = new PisteLanguage();

	if (!kieli->Lue_Tiedosto("suomi.lng"))
		cout<<"tiedoston luku epäonnistui!\n";
	else
		cout<<"tiedoston luku onnistui!\n";

	int teksti = 0;

	teksti = kieli->Hae_Indeksi("testi2");
	cout<<"indeksi: "<<teksti<<" teksti: |"<<kieli->Hae_Teksti(teksti)<<"|\n";	

	for (int i=0;i<10;i++)
	{
		cout<<"indeksi: "<<i<<" teksti: |"<<kieli->Hae_Teksti(i)<<"|\n";
		teksti++;
	}
	cin;

	delete kieli;

	return 0;
}
