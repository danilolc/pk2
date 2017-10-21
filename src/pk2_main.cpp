void PK_Quit(){
	if(!lopeta_peli) PisteDraw2_FadeOut(PD_FADE_FAST);
	lopeta_peli = true;
	musiikin_voimakkuus = 0;
}

int PK_Main(){
	static bool window_activated = true;
    bool window_closed = false;

	PK_Alusta_Tilat();

	if (window_closed){
		running = false;
		return 0;
	}

	if (window_activated) {
		MOUSE hiiri = PisteInput_UpdateMouse(pelin_tila == TILA_KARTTA);
		if (hiiri.x < 0) hiiri.x = 0;
		if (hiiri.y < 0) hiiri.y = 0;
		if (hiiri.x > 640-19) hiiri.x = 640-19;
		if (hiiri.y > 480-19) hiiri.y = 480-19;

		hiiri_x = hiiri.x;
		hiiri_y = hiiri.y;
	}
	else {
		hiiri_x = -30;
		hiiri_y = -30;
		if (PisteInput_Hiiri_Oikea() || PisteInput_Hiiri_Vasen()) {
			PisteInput_ActivateWindow(true);
			window_activated = true;
			menu_valittu_id = 0;
		}
	}

	switch (pelin_tila){
		case TILA_PELI       : PK_Main_Peli();       break;
		case TILA_MENUT      : PK_Main_Menut();      break;
		case TILA_KARTTA     : PK_Main_Kartta();     break;
		case TILA_PISTELASKU : PK_Main_Pistelasku(); break;
		case TILA_INTRO      : PK_Main_Intro();      break;
		case TILA_LOPPU      : PK_Main_Loppu();      break;
		default              : lopeta_peli = true;   break;
	}

	// GET MUSIC
	bool saada = false;

	if (musiikin_voimakkuus != musiikin_voimakkuus_nyt)
		saada = true;

	if (musiikin_max_voimakkuus > 64)
		musiikin_max_voimakkuus = 64;

	if (musiikin_max_voimakkuus < 0)
		musiikin_max_voimakkuus = 0;

	if (musiikin_voimakkuus > musiikin_max_voimakkuus)
		musiikin_voimakkuus = musiikin_max_voimakkuus;

	if (musiikin_voimakkuus_nyt < musiikin_voimakkuus)
		musiikin_voimakkuus_nyt++;

	if (musiikin_voimakkuus_nyt > musiikin_voimakkuus)
		musiikin_voimakkuus_nyt--;

	if (musiikin_voimakkuus_nyt > 64)
		musiikin_voimakkuus_nyt = 64;

	if (musiikin_voimakkuus_nyt < 0)
		musiikin_voimakkuus_nyt = 0;

	if (saada && settings.musiikki)
		PisteSound_SetMusicVolume(musiikin_voimakkuus_nyt);

	static bool wasPressed = false;

	bool skipped = !skip_frame && doublespeed; // If is in double speed and don't skip this frame, so the last frame was skipped, and it wasn't drawn
	if (PisteInput_Keydown(PI_ESCAPE) && key_delay == 0 && !skipped){ //Don't activate menu whith a not drawn screen
		if(test_level)
			PK_Quit();
		else{
			if (menu_nyt != MENU_PAAVALIKKO || pelin_tila != TILA_MENUT){
				pelin_seuraava_tila = TILA_MENUT;
				menu_nyt = MENU_PAAVALIKKO;
				degree_temp = degree;
			}
			else if (pelin_tila == TILA_MENUT && !wasPressed && PisteInput_Keydown(PI_ESCAPE) && menu_lue_kontrollit == 0){ // Just pressed escape in menu
				if(menu_valittu_id == menu_valinta_id-1)
					PK_Quit();
				else {
					menu_valittu_id = menu_valinta_id-1; // Set to "exit" option
					window_activated = false;
					PisteInput_ActivateWindow(false);
				}
			}
		}
	}

	wasPressed = PisteInput_Keydown(PI_ESCAPE);

	if (lopeta_peli && !PisteDraw2_IsFading())
		window_closed = true;
	running = !PK2_virhe;
	return 0;
}

int Unload(){
	if (!unload){
		PisteSound_StopMusic();
		delete kartta;
		delete tekstit;
		unload = true;
	}
	return 0;
}

int main(int argc, char *argv[]){
	char* test_path = NULL;

	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "dev") == 0) {
			dev_mode = true;
			Piste_SetDebug(true);
		}
		if (strcmp(argv[i], "nolimits") == 0) RAJAA_KARTANPIIRTOALUE = false;
		if (strcmp(argv[i], "test") == 0) {
			if (argc <= i + 1) PK2_virhe = true;
			else {
				test_level = true;
				test_path = argv[i + 1];
			}
		}
		if (strcmp(argv[i], "version") == 0) {
			printf(PK2_VERSION);
			printf("\n");
			exit(0);
		}
	}

	if (Piste_Init(RUUDUN_LEVEYS, RUUDUN_KORKEUS, GAME_NAME) < 0) {
		printf("PK2    - Failed to init PisteEngine.\n");
		return 0;
	}
	//Piste_SetFPS(60);

	//Game works in ../res
	chdir("../res");
	strcpy(tyohakemisto,".");

	PK_Asetukset_Lataa("data/settings.ini");

	tekstit = new PisteLanguage();

	if (!PK_Lataa_Kieli()){
		printf("PK2    - Could not find %s!\n",settings.kieli);
		strcpy(settings.kieli,"english.txt");
		if(!PK_Lataa_Kieli()){
			printf("PK2    - Could not find the default language file!\n");
			return 0;
		}
	}

	PK_Alusta_Tilat();

	pelin_seuraava_tila = TILA_INTRO;
	if (dev_mode)
		pelin_seuraava_tila = TILA_MENUT;
	if (test_level) {
		pelin_seuraava_tila = TILA_PELI;
		PK_Start_Test(test_path);
	}

	Piste_Loop(running, *PK_Main);

	if(PK2_virhe)
		printf("PK2    - Error!\n");

	PK_Asetukset_Tallenna("data/settings.ini");

	Unload();
	Piste_Quit();

	return 0;
}
