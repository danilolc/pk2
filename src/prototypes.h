//SPRITET JA NIIDEN PROTOTYYPIT
PK2Sprite_Prototyyppi protot[MAX_PROTOTYYPPEJA];

PK2Sprite spritet[MAX_SPRITEJA];

int taustaspritet[MAX_SPRITEJA];

int seuraava_vapaa_proto = 0;

PK2Sprite_Prototyyppi *esineet[MAX_ESINEITA];
int esineita = 0;


//Empty the prototypes
void PK_Prototyyppi_Tyhjenna(){
	for (int i=0; i<MAX_PROTOTYYPPEJA; i++){
		for (int j=0;j<MAX_AANIA;j++)
			if (protot[i].aanet[j] > -1)
				PisteSound_FreeSFX(protot[i].aanet[j]);
		protot[i].Uusi();
		strcpy(kartta->protot[i],"");
	}

	seuraava_vapaa_proto = 0;
}

int PK_Prototyyppi_Lataa_Aani(char *polku, char *tiedosto){
	char aanitiedosto[255];
	if (strcmp(tiedosto,"")!=0){
		strcpy(aanitiedosto,polku);
		strcat(aanitiedosto,tiedosto);
		return PisteSound_LoadSFX(aanitiedosto);
	}

	return -1;
}
//Load sprite prototypes
int PK_Prototyyppi_Lataa_Vanha(char *polku, char *tiedosto){
	char aanipolku[255];
	char testipolku[255];
	strcpy(aanipolku,polku);

	//Check if have space
	if (seuraava_vapaa_proto < MAX_PROTOTYYPPEJA){
		if (protot[seuraava_vapaa_proto].Lataa(polku, tiedosto) == 1)
			return 1;
		protot[seuraava_vapaa_proto].indeksi = seuraava_vapaa_proto;
	}
	else return 2;

	//Load sounds
	for (int i=0;i<MAX_AANIA;i++){

		if (strcmp(protot[seuraava_vapaa_proto].aanitiedostot[i],"")!=0){

			strcpy(testipolku,aanipolku);
			strcat(testipolku,"/");
			strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

			if (PisteUtils_CheckFile(testipolku))
				protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			else{
			//if (protot[seuraava_vapaa_proto].aanet[i] == -1) {
				//strcpy(aanipolku,"sprites/");
				getcwd(aanipolku, _MAX_PATH);
				strcat(aanipolku,"/sprites/");

				strcpy(testipolku,aanipolku);
				strcat(testipolku,"/");
				strcat(testipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);

				if (PisteUtils_CheckFile(testipolku))
					protot[seuraava_vapaa_proto].aanet[i] = PK_Prototyyppi_Lataa_Aani(aanipolku,protot[seuraava_vapaa_proto].aanitiedostot[i]);
			}
		}
		//if (protot[seuraava_vapaa_proto].aanet[i] == -1)
		//	return 3;
	}

	//PisteLog_Kirjoita("  - Loading completed. \n");

	seuraava_vapaa_proto++;

	return 0;
}

void PK_Prototyyppi_Aseta_Muutos_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].muutos_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i) {
				if (strcmp(protot[i].muutos_sprite,protot[j].tiedosto)==0){
					protot[i].muutos = j;
					loytyi = true;
				}
			}
			j++;
		}

		if (!loytyi) {
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].muutos_sprite)==0)
				protot[i].muutos = seuraava_vapaa_proto-1; // jokainen lataus kasvattaa seuraava_vapaa_proto:a
		}
	}
}

void PK_Prototyyppi_Aseta_Bonus_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].bonus_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].bonus_sprite,protot[j].tiedosto)==0){
					protot[i].bonus = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].bonus_sprite)==0)
				protot[i].bonus = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus1_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].ammus1_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].ammus1_sprite,protot[j].tiedosto)==0){
					protot[i].ammus1 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);


			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus1_sprite)==0)
				protot[i].ammus1 = seuraava_vapaa_proto-1;
		}
	}
}

void PK_Prototyyppi_Aseta_Ammus2_Sprite(int i){
	int j = 0;
	bool loytyi = false;

	if (strcmp(protot[i].ammus2_sprite,"")!=0){
		while (j<MAX_PROTOTYYPPEJA && !loytyi){
			if (j!=i){
				if (strcmp(protot[i].ammus2_sprite,protot[j].tiedosto)==0){
					protot[i].ammus2 = j;
					loytyi = true;
				}
			}

			j++;
		}

		if (!loytyi){
			char polku[_MAX_PATH];
			strcpy(polku,"sprites/");
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku, protot[i].ammus2_sprite)==0)
				protot[i].ammus2 = seuraava_vapaa_proto-1;
		}
	}
}

int PK_Prototyyppi_Lataa_Kaikki(){
	char polku[_MAX_PATH];
	int viimeinen_proto;

	for (int i=0;i < MAX_PROTOTYYPPEJA;i++){
		if (strcmp(kartta->protot[i],"") != 0){
			viimeinen_proto = i;
			strcpy(polku,"");
			PK_Lisaa_Episodin_Hakemisto(polku);
			//PK_Lisaa_Episodin_Hakemisto(polku);

			if (PK_Prototyyppi_Lataa_Vanha(polku,kartta->protot[i])==1){
				strcpy(polku,"");
				strcpy(polku,"sprites/");
				//PK_Lisaa_Episodin_Hakemisto(polku);
				if (PK_Prototyyppi_Lataa_Vanha(polku,kartta->protot[i])==1)
					seuraava_vapaa_proto++;
					//return -1;
			}
		}
		else
			seuraava_vapaa_proto++;
	}

	seuraava_vapaa_proto = viimeinen_proto+1;

	for (int i=0;i<MAX_PROTOTYYPPEJA;i++){
		PK_Prototyyppi_Aseta_Muutos_Sprite(i);
		PK_Prototyyppi_Aseta_Bonus_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus1_Sprite(i);
		PK_Prototyyppi_Aseta_Ammus2_Sprite(i);
	}

	return 0;
}
