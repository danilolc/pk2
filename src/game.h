int kamera_x = 0;
int kamera_y = 0;
double dkamera_x = 0;
double dkamera_y = 0;
double dkamera_a = 0;
double dkamera_b = 0;


//INFO MUUTTUJAT
bool	piirra_infot = false;
int		info_spriteja = 0;
int		info_spriteja_piirretty = 0;
int		info_spriteja_aktiivisena = 0;

//Clean items
void PK_Esineet_Alusta(){
	for (int i=0;i<MAX_ESINEITA;i++)
		esineet[i] = NULL;
}

int PK_Esineet_Piirra(){
	int x,y;

	y = KARTANPIIRTO_KORKEUS-35;//36
	x = item_paneeli_x + 35;//40

	BYTE v1, v2;

	esineita = 0;

	for (int i=0;i<MAX_ESINEITA;i++)
		if (esineet[i] != NULL)
		{
			esineita++;

			if (i == 0)
			{
				v1 = 31;
				v2 = 16+128;
			}
			else
			{
				v1 = 0;
				v2 = 16;

			}

			if (false)
			{

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2+4,
														y-esineet[i]->korkeus/2+4,
														x+esineet[i]->leveys/2+4,
														y+esineet[i]->korkeus/2+4,
														0);

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2-1,
														y-esineet[i]->korkeus/2-1,
														x+esineet[i]->leveys/2+1,
														y+esineet[i]->korkeus/2+1,
														v1);

				PisteDraw2_ScreenFill(	x-esineet[i]->leveys/2+1,
														y-esineet[i]->korkeus/2+1,
														x+esineet[i]->leveys/2-1,
														y+esineet[i]->korkeus/2-1,
														v2);

			}


			esineet[i]->Piirra(x-esineet[i]->leveys/2,y-esineet[i]->korkeus/2,0);
			x += 38;
		}


	return 0;
}

bool PK_Esineet_Lisaa(PK2Sprite_Prototyyppi *proto){
	int i=0;
	bool lisatty = false;

	char ilmo[80];
	strcpy(ilmo,tekstit->Hae_Teksti(txt_game_newitem));  //"a new item: ";

	while (i<MAX_ESINEITA && !lisatty)
	{
		if (esineet[i] == NULL)
		{
			lisatty = true;
			esineet[i] = proto;
			//strcat(ilmo,proto->nimi);
			PK_Uusi_Ilmoitus(ilmo);
		}
		i++;
	}
	return lisatty;
}

int PK_Esineet_Kayta(){
	if (esineet[0] != NULL)
		PK_Sprite_Lisaa(*esineet[0],0,spritet[pelaaja_index].x-esineet[0]->leveys,spritet[pelaaja_index].y,MAX_SPRITEJA/*pelaaja_index*/);

	for (int i=0;i<MAX_ESINEITA-1;i++)
		esineet[i] = esineet[i+1];

	esineet[MAX_ESINEITA-1] = NULL;

	return 0;
}

int PK_Esineet_Vaihda(){
	if (esineet[0] == NULL)
		return 0;

	PK2Sprite_Prototyyppi *temp = esineet[0];

	for (int i=0;i<MAX_ESINEITA-1;i++)
		esineet[i] = esineet[i+1];

	int lkm = 0;

	while(lkm < MAX_ESINEITA-1 && esineet[lkm] != NULL)
		lkm++;

	esineet[lkm] = temp;

	return 0;
}


int PK_Spritet(){
	info_spriteja_aktiivisena = 0;

	//bool aktiivinen;
	int i;

	for (i=0;i<MAX_SPRITEJA;i++) //If it is on screen
	{
		// Onko sprite l�hell� tapahtumien keskipistett�? Jos on, niin aktivoidaan.
		if (spritet[i].x < kamera_x+640+320 &&//KARTANPIIRTO_LEVEYS+KARTANPIIRTO_LEVEYS/2 &&
			spritet[i].x > kamera_x-320 &&//KARTANPIIRTO_LEVEYS/2 &&
			spritet[i].y < kamera_y+480+240 &&//KARTANPIIRTO_KORKEUS+KARTANPIIRTO_KORKEUS/2 &&
			spritet[i].y > kamera_y-240)//KARTANPIIRTO_KORKEUS/2)
			spritet[i].aktiivinen = true;
		else
			spritet[i].aktiivinen = false;

		if (spritet[i].piilota == true)
			spritet[i].aktiivinen = false;
	}

	for (i=0;i<MAX_SPRITEJA;i++) //1
	{
		if (spritet[i].aktiivinen && spritet[i].tyyppi->tyyppi != TYYPPI_TAUSTA)
		{
			if (spritet[i].tyyppi->tyyppi == TYYPPI_BONUS)
				PK_Sprite_Liikuta_Bonus(i);
			else
				PK_Sprite_Liikuta(i);

			info_spriteja_aktiivisena++;
		}
	}

	return 0;
}

int PK_Spritet_Piirra_Taustat(){
	double xl, yl, alku_x, alku_y, yk;
	int i;


	for (int in=0;in<MAX_SPRITEJA;in++)
	{
		i = taustaspritet[in];
		// Onko sprite n�kyv�
		if (spritet[i].tyyppi != NULL && i != -1)
		{
			if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi == TYYPPI_TAUSTA)
			{
				//Tarkistetaanko onko sprite tai osa siit� kuvassa

				alku_x = spritet[i].alku_x;
				alku_y = spritet[i].alku_y;

				if (spritet[i].tyyppi->pallarx_kerroin != 0)
				{
					xl =  alku_x - kamera_x-KARTANPIIRTO_LEVEYS/2 - spritet[i].tyyppi->leveys/2;
					xl /= spritet[i].tyyppi->pallarx_kerroin;
					yl =  alku_y - kamera_y-KARTANPIIRTO_KORKEUS/2 - spritet[i].tyyppi->korkeus/2;
					yk = spritet[i].tyyppi->pallarx_kerroin;///1.5;
					if (yk != 0)
						yl /= yk;


				}
				else
					xl = yl = 0;

				switch(spritet[i].tyyppi->AI[0])
				{
				case AI_TAUSTA_KUU					:	yl += KARTANPIIRTO_KORKEUS/3+50; break;
				/*case AI_TAUSTA_LIIKKUU_VASEMMALLE	:	if (spritet[i].a == 0)
															spritet[i].a = rand()%3;
														spritet[i].alku_x -= spritet[i].a;
														if (spritet[i].piilossa && spritet[i].alku_x < kamera_x)
														{
													  		  spritet[i].alku_x = kamera_x+RUUDUN_LEVEYS+spritet[i].tyyppi->leveys*2;
															  spritet[i].a = rand()%3;
														}
														break;*/
				case AI_LIIKKUU_X_COS:			spritet[i].AI_Liikkuu_X(cos_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_Y_COS:			spritet[i].AI_Liikkuu_Y(cos_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_X_SIN:			spritet[i].AI_Liikkuu_X(sin_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				case AI_LIIKKUU_Y_SIN:			spritet[i].AI_Liikkuu_Y(sin_table[degree%360]);
												alku_x = spritet[i].x;
												alku_y = spritet[i].y;
												break;
				default: break;
				}

				spritet[i].x = alku_x-xl;
				spritet[i].y = alku_y-yl;

				//Tarkistetaanko onko sprite tai osa siit� kuvassa
				if (spritet[i].x - spritet[i].tyyppi->leveys/2  < kamera_x+RUUDUN_LEVEYS &&
					spritet[i].x + spritet[i].tyyppi->leveys/2  > kamera_x &&
					spritet[i].y - spritet[i].tyyppi->korkeus/2 < kamera_y+RUUDUN_KORKEUS &&
					spritet[i].y + spritet[i].tyyppi->korkeus/2 > kamera_y)
				{
					spritet[i].Piirra(kamera_x,kamera_y);
					spritet[i].piilossa = false;

					info_spriteja_piirretty++;
				}
				else
				{
					if (!paused)
						spritet[i].Animoi();
					spritet[i].piilossa = true;
				}

				info_spriteja++;

			}
		}
	}
	return 0;
}

int PK_Spritet_Piirra(){
	info_spriteja = 0;
	info_spriteja_piirretty = 0;
	int stars, sx;
	double star_x, star_y;

	for (int i=0;i<MAX_SPRITEJA;i++)
	{
		// Onko sprite n�kyv�
		if (!spritet[i].piilota && spritet[i].tyyppi->tyyppi != TYYPPI_TAUSTA)
		{
			//Tarkistetaanko onko sprite tai osa siit� kuvassa
			if (spritet[i].x - spritet[i].tyyppi->leveys/2  < kamera_x+RUUDUN_LEVEYS &&
				spritet[i].x + spritet[i].tyyppi->leveys/2  > kamera_x &&
				spritet[i].y - spritet[i].tyyppi->korkeus/2 < kamera_y+RUUDUN_KORKEUS &&
				spritet[i].y + spritet[i].tyyppi->korkeus/2 > kamera_y)
			{
			if (spritet[i].isku > 0 && spritet[i].tyyppi->tyyppi != TYYPPI_BONUS && spritet[i].energia < 1)
				PK_Piirra_Iskuvalays((DWORD)spritet[i].x-8,(DWORD)spritet[i].y-8);


			if (nakymattomyys == 0 || degree%2 == 0 || i != pelaaja_index)
				spritet[i].Piirra(kamera_x,kamera_y);
			//else
			//	spritet[i].Piirra(kamera_x,kamera_y);

				if (spritet[i].energia < 1 && spritet[i].tyyppi->tyyppi != TYYPPI_AMMUS)
				{
					sx = (int)spritet[i].x;
					for (stars=0; stars<3; stars++)
					{
						star_x = spritet[i].x-8 + (sin_table[((stars*120+degree)*2)%359])/3;
						star_y = spritet[i].y-18 + (cos_table[((stars*120+degree)*2+sx)%359])/8;
						PK_Piirra_Tahti((DWORD)star_x,(DWORD)star_y,100,128);
					}
				}

				info_spriteja_piirretty++;
			}
			else
			{
				if (!paused)
					spritet[i].Animoi();

				if (spritet[i].energia < 1)
					spritet[i].piilota = true;
			}

			info_spriteja++;
		}
	}
	return 0;
}



int PK_Kamera(){


	kamera_x = (int)spritet[pelaaja_index].x-KARTANPIIRTO_LEVEYS/2;
	kamera_y = (int)spritet[pelaaja_index].y-KARTANPIIRTO_KORKEUS/2;

	/*
	if (!PisteInput_Hiiri_Vasen())
	{
		kamera_x = (int)spritet[pelaaja_index].x-KARTANPIIRTO_LEVEYS/2;
		kamera_y = (int)spritet[pelaaja_index].y-KARTANPIIRTO_KORKEUS/2;
	}
	else
	{
		kamera_x += PisteInput_Hiiri_X(0)*5;
		kamera_y += PisteInput_Hiiri_Y(0)*5;
	}*/

	if (jaristys > 0)
	{
		dkamera_x += (rand()%jaristys-rand()%jaristys)/5;
		dkamera_y += (rand()%jaristys-rand()%jaristys)/5;

		jaristys--;
	}

	if (kytkin_tarina > 0)
	{
		dkamera_x += (rand()%9-rand()%9);//3
		dkamera_y += (rand()%9-rand()%9);

		kytkin_tarina--;
	}

	if (dkamera_x != kamera_x)
		dkamera_a = (kamera_x - dkamera_x) / 15;

	if (dkamera_y != kamera_y)
		dkamera_b = (kamera_y - dkamera_y) / 15;

	if (dkamera_a > 6)
		dkamera_a = 6;

	if (dkamera_a < -6)
		dkamera_a = -6;

	if (dkamera_b > 6)
		dkamera_b = 6;

	if (dkamera_b < -6)
		dkamera_b = -6;

	dkamera_x += dkamera_a;
	dkamera_y += dkamera_b;

	kamera_x = (int)dkamera_x;
	kamera_y = (int)dkamera_y;

	if (kamera_x < 0)
		kamera_x = 0;

	if (kamera_y < 0)
		kamera_y = 0;

	if (kamera_x > int(PK2KARTTA_KARTTA_LEVEYS-KARTANPIIRTO_LEVEYS/32)*32)
		kamera_x = int(PK2KARTTA_KARTTA_LEVEYS-KARTANPIIRTO_LEVEYS/32)*32;

	if (kamera_y > int(PK2KARTTA_KARTTA_KORKEUS-KARTANPIIRTO_KORKEUS/32)*32)
		kamera_y = int(PK2KARTTA_KARTTA_KORKEUS-KARTANPIIRTO_KORKEUS/32)*32;

	return 0;
}

int PK_Piirra_Info(){
	int fy, vali;
	char lukua[20];

	fy = 35;

	vali = PisteDraw2_Font_Write(fontti1,"spriteja: ",10,fy);
	itoa(info_spriteja,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(fontti1,"aktiivisia: ",10,fy);
	itoa(info_spriteja_aktiivisena,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	vali = PisteDraw2_Font_Write(fontti1,"piirretty: ",10,fy);
	itoa(info_spriteja_piirretty,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,10+vali,fy);
	fy += 10;

	for (int i=0;i<40;i++)
	{
		itoa(protot[i].indeksi,lukua,10);
		PisteDraw2_Font_Write(fontti1,lukua,410,10+i*10);
		PisteDraw2_Font_Write(fontti1,protot[i].tiedosto,430,10+i*10);
		PisteDraw2_Font_Write(fontti1,protot[i].bonus_sprite,545,10+i*10);
	}

	PisteDraw2_Font_Write(fontti1,viesti,10,80);

	for (int i=0;i<EPISODI_MAX_JAKSOJA;i++)
	{
		if (strcmp(jaksot[i].nimi,"")!=0)
			PisteDraw2_Font_Write(fontti1,jaksot[i].nimi,0,240+i*10);
	}

	PisteDraw2_Font_Write(fontti1,seuraava_kartta,10,450);


	char dluku[50];

	gcvt(spritet[pelaaja_index].x,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,420);
	gcvt(spritet[pelaaja_index].y,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,430);
	gcvt(spritet[pelaaja_index].b,7,dluku);
	PisteDraw2_Font_Write(fontti1,dluku,10,440);

	itoa(spritet[pelaaja_index].hyppy_ajastin,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,270,450);

	char tpolku[_MAX_PATH] = "";
	PK_Lisaa_Episodin_Hakemisto(tpolku);

	PisteDraw2_Font_Write(fontti1,tpolku,10,470);

	itoa(nakymattomyys,lukua,10);
	PisteDraw2_Font_Write(fontti1,lukua,610,470);

	return 0;
}

int PK_Piirra_Alapaneeli(){

	if (esineita > 0 && item_paneeli_x < 10)
		item_paneeli_x++;

	if (esineita == 0 && item_paneeli_x > -215)
		item_paneeli_x--;

	if (item_paneeli_x > -215)
		PisteDraw2_Image_CutClip(kuva_peli,item_paneeli_x,KARTANPIIRTO_KORKEUS-60,
		                        1,216,211,266);
	if (item_paneeli_x > 5)
			PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_items),15,KARTANPIIRTO_KORKEUS-65);


	char luku[15];
	int vali = 0;

	int x, y;

	//////////////
	// piirr� aika
	//////////////
	if (aika > 0)
	{
		int min = aika/60,
			sek = aika%60;


		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 342;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_time),x,y-20);


		itoa(min,luku,10);
		PisteDraw2_Font_Write(        fontti4,luku,x+1,y+1);
		vali += PisteDraw2_Font_Write(fontti2,luku,x,y);
		vali += PisteDraw2_Font_Write(fontti1,":",x+vali,y);
		if (sek < 10)
		{
			PisteDraw2_Font_Write(        fontti4,"0",x+vali+1,y+1);
			vali += PisteDraw2_Font_Write(fontti2,"0",x+vali,y);
		}
		itoa(sek,luku,10);

		PisteDraw2_Font_Write(        fontti4,luku,x+vali+1,y+1);
		vali += PisteDraw2_Font_Write(fontti2,luku,x+vali,y);
	}

	/////////////////
	// piirr� avaimet
	/////////////////
	if (avaimia > 0)
	{
		x = RUUDUN_LEVEYS / 2 - 546 / 2 + 483;
		y = KARTANPIIRTO_KORKEUS-39;
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_keys),x,y-20);

		itoa(avaimia,luku,10);
		PisteDraw2_Font_Write(fontti4,luku,x+1,y+1);
		PisteDraw2_Font_Write(fontti2,luku,x,y);
	}

	/////////////////
	// piirr� esineet
	/////////////////
	PK_Esineet_Piirra();

	return 0;
}

int PK_Piirra_Peli_Ylaosa(){
	char luku[15];
	int vali = 20;

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_energy),40,10);
	ltoa(spritet[pelaaja_index].energia,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,40+vali+1,10+1);
	PisteDraw2_Font_Write(fontti2,luku,40+vali,10);

	vali = PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_score),230,10);
	ltoa(jakso_pisteet,luku,10);
	PisteDraw2_Font_Write(fontti4,luku,230+vali+1,10+1);
	PisteDraw2_Font_Write(fontti2,luku,230+vali,10);

	if (spritet[pelaaja_index].ammus2 != -1)
	{
		//itoa(ammuksia2,luku,10);
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_attack1), KARTANPIIRTO_LEVEYS-170,10);
		protot[spritet[pelaaja_index].ammus2].Piirra(KARTANPIIRTO_LEVEYS-170,20,0);
		//PisteDraw2_Font_Write(fontti2,luku,RUUDUN_LEVEYS-35,50);
	}

	if (spritet[pelaaja_index].ammus1 != -1)
	{
		//itoa(ammuksia1,luku,10);
		PisteDraw2_Font_Write(fontti1,tekstit->Hae_Teksti(txt_game_attack2), KARTANPIIRTO_LEVEYS-90,25);
		protot[spritet[pelaaja_index].ammus1].Piirra(KARTANPIIRTO_LEVEYS-90,35,0);
		//PisteDraw2_Font_Write(fontti2,luku,RUUDUN_LEVEYS-35,10);
	}


	if (ilmoitus_ajastin > 0)
	{
		int ilm_pituus = strlen(ilmoitus) * 8 + 8; // 300

		RECT alue = {RUUDUN_LEVEYS/2-(ilm_pituus/2),60,RUUDUN_LEVEYS/2+(ilm_pituus/2),60+20};

		if (ilmoitus_ajastin < 20)
		{
			alue.top	+= (20 - ilmoitus_ajastin) / 2;
			alue.bottom -= (20 - ilmoitus_ajastin) / 2;
		}

		if (ilmoitus_ajastin > MAX_ILMOITUKSENNAYTTOAIKA - 20)
		{
			alue.top	+= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - ilmoitus_ajastin) / 2;
			alue.bottom -= 10 - (MAX_ILMOITUKSENNAYTTOAIKA - ilmoitus_ajastin) / 2;
		}

		PisteDraw2_ScreenFill(alue.left-1,alue.top-1,alue.right+1,alue.bottom+1,51);
		PisteDraw2_ScreenFill(alue.left,alue.top,alue.right,alue.bottom,38);

		if (ilmoitus_ajastin >= 100)
			PisteDraw2_Font_Write(fontti1,ilmoitus,alue.left+4,alue.top+4);
		else
			PisteDraw2_Font_WriteAlpha(fontti1,ilmoitus,alue.left+4,alue.top+4,ilmoitus_ajastin);
	}

	return 0;
}

int PK_Piirra_Nuppi(int x, int y, int t){

	int paluu = 0;

	t = t * 25;

	int vilkku = 50 + (int)(sin_table[degree%360]*2);

	if (vilkku < 0)
		vilkku = 0;

	//PisteDraw2_Image_Clip(kuva_peli,x,y,1+t,58,23+t,80);

	if (hiiri_x > x && hiiri_x < x+17 && hiiri_y > y && hiiri_y < y+17)
	{
		if (key_delay == 0 && (PisteInput_Hiiri_Vasen() || PisteInput_Keydown(PI_SPACE)
													    || PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1,PI_OHJAIN_NAPPI_1)))
		{
			key_delay = 30;
			return 2;
		}

		if (t == 25)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-2, y-2, 60, 96);
		if (t == 0)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-4, y-4, 60, 32);
		if (t == 50)
			PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-4, y-4, 60, 64);

		paluu = 1;
	}

	if (t == 25)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 247, 1, 25, 25, x-2, y-2, vilkku, 96);

	if (((degree/45)+1)%4==0 || t==0)
		PisteDraw2_Image_CutClip(kuva_peli,x,y,1+t,58,23+t,80);

	return paluu;
}


int PK_Piirra_Tausta(){
	int pallarx = (kamera_x%1920)/3;
	int pallary = (kamera_y%1440)/3;

	int pallarx2 = (kamera_x%1280)/2;
	int pallary2 = (kamera_y%960)/2;

	PisteDraw2_ScreenFill(34);//0


	if (kartta->tausta == TAUSTA_STAATTINEN)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_HORI)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,0);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,0);

		if (KARTANPIIRTO_LEVEYS > 640)
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,0);
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,0 - pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0,480 - pallary);

		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,0 - pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640,480 - pallary);
		}
	}

	if (kartta->tausta == TAUSTA_PALLARX_VERT_JA_HORI)
	{
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,	0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,0-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,0 - pallarx,	480-pallary);
		PisteDraw2_Image_Clip(kartta->taustakuva_buffer,640 - pallarx,480-pallary);

		if (KARTANPIIRTO_LEVEYS > 640)
		{
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,0-pallary);
			PisteDraw2_Image_Clip(kartta->taustakuva_buffer,1280 - pallarx,480-pallary);
		}
	}

	/*pilvet 1*/
	/*
	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,0-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,0-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,480-pallary2);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,480-pallary2);
	*/
	/*pilvet 2
	pallarx2 = (int)((kamera_x%1600)/2.5);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,95,0,332,640,382);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,95,0,332,640,382);

	pallarx2 = (int)((kamera_x%1280)/2);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,65,0,186,640,263);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,65,0,186,640,263);

	pallarx2 = (int)((kamera_x%960)/1.5);

	PisteDraw2_Image_Clip(tausta_extra,0-pallarx2,0,0,0,640,109);
	PisteDraw2_Image_Clip(tausta_extra,640-pallarx2,0,0,0,640,109);
	*/
	return 0;
}

int PK_Piirra_Peli(){
	char luku[15];
	int vali = 20;

	PK_Piirra_Tausta();

	PK2Kartta_Animoi(degree,palikka_animaatio/7,kytkin1,kytkin2,kytkin3,false);

	if (!skip_frame)
		if (settings.tausta_spritet)
			PK_Spritet_Piirra_Taustat();

	if (settings.saa_efektit)
		PK_Taustapartikkelit();

	if (!skip_frame)
		kartta->Piirra_Taustat(kamera_x,kamera_y,false);

	if (!paused)
	{
		PK_Partikkelit();
		if (!jakso_lapaisty && (!aikaraja || aika > 0))
			PK_Spritet();
		PK_Fadetekstit();
	}

	if (!skip_frame)
		PK_Spritet_Piirra();

	//PK_Piirra_Bonukset();

	if (!skip_frame)
		PK_Partikkelit_Piirra();

	if (!skip_frame)
		kartta->Piirra_Seinat(kamera_x,kamera_y, false);

	PK_Fadetekstit_Piirra();

	if (settings.nayta_tavarat)
		PK_Piirra_Alapaneeli();

	PK_Piirra_Peli_Ylaosa();

	/*
	//////////////
	// piirr� aika
	//////////////
	int min = aika/60,
		sek = aika%60;
	vali = PisteDraw2_Font_Write(fontti2,"time ",272,RUUDUN_KORKEUS-20);
	itoa(min,luku,10);
	vali += PisteDraw2_Font_Write(fontti2,luku,272+vali,RUUDUN_KORKEUS-20);
	vali += PisteDraw2_Font_Write(fontti1,":",272+vali,RUUDUN_KORKEUS-15);

	if (sek < 10)
		vali += PisteDraw2_Font_Write(fontti2,"0",272+vali,RUUDUN_KORKEUS-20);

	itoa(sek,luku,10);
	vali += PisteDraw2_Font_Write(fontti2,luku,272+vali,RUUDUN_KORKEUS-20);

	/////////////////
	// piirr� avaimet
	/////////////////
	vali = PisteDraw2_Font_Write(fontti2,"keys ",450,RUUDUN_KORKEUS-20);
	itoa(avaimia,luku,10);
	PisteDraw2_Font_Write(fontti2,luku,450+vali,RUUDUN_KORKEUS-20);

	/////////////////
	// piirr� esineet
	/////////////////
	PK_Esineet_Piirra();
	*/
	if (piirra_infot)
		PK_Piirra_Info();

	///////////////////
	// piirr� framerate
	///////////////////
	if (fps_nayta){
		vali = PisteDraw2_Font_Write(fontti1,"fps: ",570,48);
		fps = Piste_GetFPS();
		itoa(fps,luku,10);
		PisteDraw2_Font_Write(fontti1,luku,570+vali,48);
	}

	///////////////////
	// piirr� pause
	///////////////////
	if (paused)
		PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_game_paused),KARTANPIIRTO_LEVEYS/2-82,KARTANPIIRTO_KORKEUS/2-9);

	////////////////////////
	// piirr� jakso l�p�isty
	////////////////////////
	if (jakso_lapaisty)
	{
		PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_clear),fontti2,KARTANPIIRTO_LEVEYS/2-120,KARTANPIIRTO_KORKEUS/2-9);

		/*
		if (aikaraja)
		{
			vali = PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_timebonus),fontti2,KARTANPIIRTO_LEVEYS/2-120,KARTANPIIRTO_KORKEUS/2+20);
			itoa(fake_pisteet,luku,10);
			PK_Piirra_LaineTeksti(luku,fontti2,KARTANPIIRTO_LEVEYS/2-120+vali,KARTANPIIRTO_KORKEUS/2+20);
		}*/

	}
	else
		if (peli_ohi)
		{
			if (spritet[pelaaja_index].energia < 1)
			{
				PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_ko),fontti2,KARTANPIIRTO_LEVEYS/2-90,KARTANPIIRTO_KORKEUS/2-9-10);
			}
			else
				if (aika < 1 && aikaraja)
					PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_timeout),fontti2,KARTANPIIRTO_LEVEYS/2-67,KARTANPIIRTO_KORKEUS/2-9-10);

			PK_Piirra_LaineTeksti(tekstit->Hae_Teksti(txt_game_tryagain),fontti2,KARTANPIIRTO_LEVEYS/2-75,KARTANPIIRTO_KORKEUS/2-9+10);
		}


	if (skip_frame) Piste_IgnoreFrame();

	if (doublespeed) skip_frame = !skip_frame;
	else skip_frame = false;

	palikka_animaatio = 1 + palikka_animaatio % 34;

	return 0;
}



int PK_Start_Game(){
    PisteDraw2_SetXOffset(0);
    PisteDraw2_ScreenFill(0);
    PisteDraw2_Font_Write(fontti2,tekstit->Hae_Teksti(txt_game_loading),RUUDUN_LEVEYS/2-82,RUUDUN_KORKEUS/2-9);

    if (jaksot[jakso_indeksi_nyt].lapaisty)
        uusinta = true;
    else
        uusinta = false;

    if (!peli_kesken) {
        jakso_lapaisty = false;

        PK_Sprite_Tyhjenna(); //Reset sprites

        if (PK_Kartta_Lataa(seuraava_kartta) == 1)
            PK2_virhe = true;

        PK_Palikka_Laske_Palikat();

        PK_Alusta_Fadetekstit(); //Reset fade text

        PK_Esineet_Alusta();
        peli_kesken = true;
        musiikin_voimakkuus = musiikin_max_voimakkuus;
        degree = 0;
        item_paneeli_x = -215;
        piste_lisays = 0;
    }
    else {
        degree = degree_temp;
    }
    return 0;
}
int PK_Main_Peli(){
	PK_Kamera();
	PK_Piirra_Peli();
	PK_Palikka_Paivita_Lasketut_Palikat();

	if (!paused){
		degree = 1 + degree%359;

		if (kytkin1 > 0)
			kytkin1 --;

		if (kytkin2 > 0)
			kytkin2 --;

		if (kytkin3 > 0)
			kytkin3 --;

		if (ilmoitus_ajastin > 0)
			ilmoitus_ajastin--;

		if (piste_lisays > 0)
		{
			jakso_pisteet++;
			piste_lisays--;
		}

		if (aikaraja && !jakso_lapaisty)
		{
			if (sekunti > 0)
			{
				sekunti -= 10;
			}
			else
			{
				sekunti = 1000;

				if (aika > 0)
				{
					aika--;
				}
				else
				{
					peli_ohi = true;
					//PisteDraw2_FadeOut(PD_FADE_SLOW);
				}
			}
		}

		if (nakymattomyys > 0)
			nakymattomyys--;

	}

	if (spritet[pelaaja_index].energia < 1 && !peli_ohi){
		peli_ohi = true;
		key_delay = 50;
	}

	if (key_delay > 0)
		key_delay--;

	if (jakso_lapaisty || peli_ohi){
		if (lopetusajastin > 1)
			lopetusajastin--;

		if (lopetusajastin == 0)
			lopetusajastin = 800;//2000;

		if (PisteInput_Keydown(kontrolli_hyokkays1) || PisteInput_Keydown(kontrolli_hyokkays2) ||
			PisteInput_Keydown(kontrolli_hyppy) || PisteInput_Keydown(PI_RETURN))
			if (lopetusajastin > 2 && lopetusajastin < 600/*1900*/ && key_delay == 0)
				lopetusajastin = 2;

		if (lopetusajastin == 2)
		{
			PisteDraw2_FadeOut(PD_FADE_NORMAL);
			//musiikin_voimakkuus = 0;
		}
	}
	if (lopetusajastin == 1 && !PisteDraw2_IsFading()){
		if(test_level){
			lopeta_peli = true;
			PK_Quit();
		}
		else{
			if (jakso_lapaisty) pelin_seuraava_tila = TILA_PISTELASKU;
			else pelin_seuraava_tila = TILA_KARTTA;
		}
	}

	if (key_delay == 0){
		if (PisteInput_Keydown(kontrolli_kayta_esine) && spritet[pelaaja_index].energia > 0){
			PK_Esineet_Kayta();
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_P) && !jakso_lapaisty){
			paused = !paused;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_DELETE))
			spritet[pelaaja_index].energia = 0;
		if (PisteInput_Keydown(PI_I)){
			fps_nayta = !fps_nayta;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_TAB)){
			PK_Esineet_Vaihda();
			key_delay = 10;
		}
	}

	if (key_delay == 0 && dev_mode){ //Debug
		if (PisteInput_Keydown(PI_Z)){
			kytkin1 = KYTKIN_ALOITUSARVO;
			kytkin2 = KYTKIN_ALOITUSARVO;
			kytkin3 = KYTKIN_ALOITUSARVO;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_X)) {
			kytkin1 = 64;
			kytkin2 = 64;
			kytkin3 = 64;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_T)) {
			doublespeed = !doublespeed;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_G)){
			settings.lapinakyvat_objektit = !settings.lapinakyvat_objektit;
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_L)){
			PK_Kartta_Avaa_Lukot();
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_K)){
			PK_Kartta_Vaihda_Kallopalikat();
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_F)){
			settings.isFullScreen = !settings.isFullScreen;
			PisteDraw2_FullScreen(settings.isFullScreen);
			key_delay = 20;
		}
		if (PisteInput_Keydown(PI_I)){
			piirra_infot = !piirra_infot;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_R)){
			PK_Kartta_Etsi_Alku();
			spritet[pelaaja_index].energia = 10;
			key_delay = 10;
		}
		if (PisteInput_Keydown(PI_U)){
			spritet[pelaaja_index].b = -10;
		}
		if (PisteInput_Keydown(PI_END)){
			if (PisteSound_StartMusic("music/hiscore.xm")!=0)
				PK2_virhe = true;

			key_delay = 20;
			jakso_lapaisty = true;
			jaksot[jakso_indeksi_nyt].lapaisty = true;
			jakso++;
		}
		if (PisteInput_Keydown(PI_E))
			spritet[pelaaja_index].energia = 3;
		if (PisteInput_Keydown(PI_LSHIFT)){
			for (int r=1;r<6;r++)
				PK_Partikkeli_Uusi(PARTIKKELI_KIPINA,spritet[pelaaja_index].x+rand()%10-rand()%10, spritet[pelaaja_index].y+rand()%10-rand()%10,0,0,rand()%100,0.1,32);
			spritet[pelaaja_index] = PK2Sprite(&protot[PROTOTYYPPI_KANA],1,false,spritet[pelaaja_index].x, spritet[pelaaja_index].y);
		}
	}

	return 0;
}
