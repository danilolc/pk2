
//PK_EpisodeScore_Start/
void EpisodeScore_Start(){
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++){
		episodipisteet.pisteet[i] = 0;
		episodipisteet.ajat[i] = 0;
		strcpy(episodipisteet.top_pelaajat[i]," ");
		strcpy(episodipisteet.nopeimmat_pelaajat[i]," ");
	}

	episodipisteet.episodin_top_pisteet = 0;
	strcpy(episodipisteet.episodin_top_pelaaja," ");
}

//PK_EpisodeScore_Compare - used in PISTELASKU screen startup
int PK_Episodipisteet_Vertaa(int jakso, DWORD episteet, DWORD aika, bool loppupisteet){
	int paluu = 0;
	if (!loppupisteet) {
		if (episteet > episodipisteet.pisteet[jakso]) {
			strcpy(episodipisteet.top_pelaajat[jakso],pelaajan_nimi);
			episodipisteet.pisteet[jakso] = episteet;
			jakso_uusi_ennatys = true;
			paluu++;
		}
		if ((aika < episodipisteet.ajat[jakso] || episodipisteet.ajat[jakso] == 0) && kartta->aika > 0) {
			strcpy(episodipisteet.nopeimmat_pelaajat[jakso],pelaajan_nimi);
			episodipisteet.ajat[jakso] = aika;
			jakso_uusi_ennatysaika = true;
			paluu++;
		}
	}
	else {
		if (episteet > episodipisteet.episodin_top_pisteet) {
		    episodipisteet.episodin_top_pisteet = episteet;
			strcpy(episodipisteet.episodin_top_pelaaja,pelaajan_nimi);
			episodi_uusi_ennatys = true;
			paluu++;
		}
	}
	return paluu;
}
//PK_EpisodeScore_Open - used in KARTTA startup
int PK_Episodipisteet_Lataa(char *filename){
	PK_Lisaa_Episodin_Hakemisto(filename);

	ifstream *tiedosto = new ifstream(filename, ios::binary);
	char versio[4];

	if (tiedosto->fail()){
		delete (tiedosto);
		EpisodeScore_Start();
		return 1;
	}

	tiedosto->read ((char *)versio, 4);

	if (strcmp(versio,"1.0") == 0) {
		tiedosto->read ((char *)&episodipisteet, sizeof (episodipisteet));
	}

	delete (tiedosto);

	return 0;
}
//PK_EpisodeScore_Save - used in PISTELASKU screen startup
int PK_Episodipisteet_Tallenna(char *filename){
	PK_Lisaa_Episodin_Hakemisto(filename);

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write ("1.0", 4);
	tiedosto->write ((char *)&episodipisteet, sizeof (episodipisteet));
	delete (tiedosto);
	return 0;
}

//PK_Save_Start ?? - Used in first start
void PK_Jaksot_Alusta(){
	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++){
		strcpy(jaksot[i].nimi,"");
		strcpy(jaksot[i].tiedosto,"");
		jaksot[i].x = 0;
		jaksot[i].y = 0;
		jaksot[i].jarjestys = -1;
		jaksot[i].lapaisty = false;
		jaksot[i].ikoni = 0;
	}
}
//PK_Search_Dir -- PisteUtils_Search_Dir
void PK_Jaksot_Hae(){
	int i=0;
	char hakemisto[_MAX_PATH];
	char list[EPISODI_MAX_JAKSOJA][_MAX_PATH];
	for (int j = 0; j < EPISODI_MAX_JAKSOJA; j++)
		memset(list[j], '\0', _MAX_PATH);

	PK2Kartta *temp = new PK2Kartta();

	strcpy(hakemisto,"");
	PK_Lisaa_Episodin_Hakemisto(hakemisto);
	jaksoja = PisteUtils_Scandir(".map", hakemisto, list, EPISODI_MAX_JAKSOJA);

	for (i=0;i<=jaksoja;i++){
		strcpy(jaksot[i].tiedosto,list[i]);
		if (temp->Lataa_Pelkat_Tiedot(hakemisto,jaksot[i].tiedosto) == 0){
			strcpy(jaksot[i].nimi, temp->nimi);
			jaksot[i].x = temp->x;//   142 + i*35;
			jaksot[i].y = temp->y;// 270;
			jaksot[i].jarjestys = temp->jakso;
			jaksot[i].ikoni = temp->ikoni;
		}
	}

	PK2JAKSO jakso;

	bool lopeta = false;

	while (!lopeta){
		lopeta = true;

		for (i=0;i<jaksoja;i++){
			if (jaksot[i].jarjestys > jaksot[i+1].jarjestys){
				jakso = jaksot[i];
				jaksot[i] = jaksot[i+1];
				jaksot[i+1] = jakso;
				lopeta = false;
			}
		}
	}
	delete temp;
}
int EpisodesOrder(){
	DWORD i, t;
	char temp[_MAX_PATH] = "";
	bool tehty;

	if (episodi_lkm > 1) {

		for (i=episodi_lkm-1;i>=0;i--) {

			tehty = true;

			//for (t=0;t<i;t++) {
			for (t=2;t<i+2;t++) {
				if (PisteUtils_AlphaSort(episodit[t],episodit[t+1]) == 1) {
					strcpy(temp, episodit[t]);
					strcpy(episodit[t], episodit[t+1]);
					strcpy(episodit[t+1], temp);
					tehty = false;
				}
			}

			if (tehty)
				return 0;
		}
	}

	return 0;
}
//PK_Episode_Search - Used in first start
int PK_Episodit_Hae(){
	int i;
	char hakemisto[_MAX_PATH];

	for (i=0;i<MAX_EPISODEJA;i++)
		strcpy(episodit[i],"");

	strcpy(hakemisto,"episodes/");

	episodi_lkm = PisteUtils_Scandir("/", hakemisto, episodit, MAX_EPISODEJA) - 2;

	EpisodesOrder();

	return 0;
}
//PK_Records_Empty
int PK_Tallennukset_Tyhjenna(){
	for (int i = 0;i < MAX_TALLENNUKSIA;i++){
		tallennukset[i].kaytossa = false;
		strcpy(tallennukset[i].episodi," ");
		strcpy(tallennukset[i].nimi,"empty");
		tallennukset[i].jakso = 0;
		tallennukset[i].pisteet = 0;
		for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
			tallennukset[i].jakso_lapaisty[j] = false;
	}

	return 0;
}
//PK_Records_Search
int PK_Tallennukset_Hae_Kaikki(char *filename){
	char versio[2];
	char lkmc[8];
	int lkm, i;

	ifstream *tiedosto = new ifstream(filename, ios::binary);

	if (tiedosto->fail()){
		delete (tiedosto);
		PK_Tallennukset_Tyhjenna();
		return 1;
	}

	PK_Tallennukset_Tyhjenna();

	tiedosto->read(versio,	sizeof(versio));

	if (strcmp(versio,"2")==0){
		tiedosto->read(lkmc, sizeof(lkmc));
		lkm = atoi(lkmc);

		for (i=0;i<lkm;i++)
			tiedosto->read ((char *)&tallennukset[i], sizeof (tallennukset[i]));
	}

	delete (tiedosto);

	return 0;
}
//PK_Records_SaveAll
int PK_Tallennukset_Tallenna_Kaikki(char *filename){
	char versio[2] = "2";
	char lkm[8];

	itoa(MAX_TALLENNUKSIA,lkm,10);

	ofstream *tiedosto = new ofstream(filename, ios::binary);
	tiedosto->write(versio,	sizeof(versio));
	tiedosto->write(lkm,	sizeof(lkm));
	for (int i=0;i< MAX_TALLENNUKSIA;i++)
		tiedosto->write((char *)&tallennukset[i],	sizeof(tallennukset[i]));

	delete (tiedosto);

	return 0;
}
//PK_Records_Load
int PK_Tallennukset_Lataa(int i){
	if (strcmp(tallennukset[i].episodi," ")!=0){
		strcpy(episodi,tallennukset[i].episodi);
		strcpy(pelaajan_nimi, tallennukset[i].nimi);
		jakso = tallennukset[i].jakso;
		pisteet = tallennukset[i].pisteet;

		PK_Jaksot_Alusta();

		//for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
		//	jaksot[j].lapaisty = tallennukset[i].jakso_lapaisty[j];

		pelin_seuraava_tila = TILA_KARTTA;
		lataa_peli = i;
		peli_kesken = false;
	}

	return 0;
}
//PK_Records_Save
int PK_Tallennukset_Tallenna(int i){
	tallennukset[i].kaytossa = true;
	strcpy(tallennukset[i].episodi, episodi);
	strcpy(tallennukset[i].nimi,pelaajan_nimi);
	tallennukset[i].jakso = jakso;
	tallennukset[i].pisteet = pisteet;

	for (int j = 0;j < EPISODI_MAX_JAKSOJA;j++)
		tallennukset[i].jakso_lapaisty[j] = jaksot[j].lapaisty;

	PK_Tallennukset_Tallenna_Kaikki("data/saves.dat");

	return 0;
}
