#include "particles.h"

#include "PisteDraw.h"

#define MAX_PARTIKKELEITA       300
#define MAX_TAUSTAPARTIKKELEITA 200

struct PK2PARTIKKELI{
	int			tyyppi;
	double		x;
	double		y;
	double		a;
	double		b;
	int			anim;
	int			aika;
	double		paino;
	int			vari;
};

PK2PARTIKKELI partikkelit[MAX_PARTIKKELEITA];
int partikkeli_index = 0;

PK2PARTIKKELI taustapartikkelit[MAX_TAUSTAPARTIKKELEITA];
int taustapartikkeli_index = 0;

int PK_Piirra_Piste(DWORD x, DWORD y, int pros, BYTE vari){
	PisteDraw2_ScreenFill(x-kamera_x, y-kamera_y,x-kamera_x+1,y-kamera_y+1,vari+25);
	return 0;
}
int PK_Piirra_Tahti(DWORD x, DWORD y, int pros, BYTE vari){
	if (pros > 99 || !settings.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,1,1,11,11);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 2, 2, 10, 10, x-kamera_x, y-kamera_y, pros, vari);

	return 0;
}
int PK_Piirra_Iskuvalays(DWORD x, DWORD y){
	int framex = ((degree%12)/3) * 58;
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x-28+8, y-kamera_y-27+8,1+framex,83,1+57+framex,83+55);//140<-83
	return 0;
}
int PK_Piirra_Valo(DWORD x, DWORD y, int pros, BYTE vari){
	if (settings.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 1, 14, 13, 13, x-kamera_x, y-kamera_y, pros, vari);
	else{
		int vx = (vari/32) * 14;
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,1+vx,14+14,14+vx,27+14);
	}

	return 0;
}
int PK_Piirra_Kipina(DWORD x, DWORD y, int pros, BYTE vari){
	if (settings.lapinakyvat_objektit)
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 99, 14, 7, 7, x-kamera_x, y-kamera_y, pros, vari);
	else{
		int vx = (vari/32) * 8;
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x, y-kamera_y,99+vx,14+14,106+vx,21+14);
	}

	return 0;
}
void PK_Piirra_Hoyhen(int x, int y, int &anim){
	int xplus = (anim/7) * 21;
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,14+xplus,1,34+xplus,12);
	anim++;
	if (anim > 63)
		anim = 0;
}
void PK_Piirra_Savupilvi(int x, int y, int &anim){
	int frame = (anim/7);
	int xplus = (frame%17) * 36;
	int yplus = 0;

	if (anim < 7*30) {

		if (frame > 16)
			yplus = 32;

		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,1+xplus,338+yplus,34+xplus,366+yplus);
		anim++;
	}

	//if (anim > 63)
	//	anim = 0;
}
void PK_Piirra_Polypilvi(DWORD x, DWORD y, int pros, BYTE vari){
	if (pros > 99 || !settings.lapinakyvat_objektit)
		PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,226,2,224,49);
	else
		PK_Piirra_Lapinakyva_Objekti(kuva_peli_sysmem, 226, 2, 18, 19, x-kamera_x, y-kamera_y, pros, vari);
	PisteDraw2_Image_CutClip(kuva_peli,x-kamera_x,y-kamera_y,226, 2, 18, 19); //Tirar isso
}

//PK_Particles_Clear
int PK_Partikkelit_Tyhjenna(){
	int i = 0;

	partikkeli_index = 0;

	while (i < MAX_PARTIKKELEITA){
		partikkelit[i].a = 0;
		partikkelit[i].aika = 0;
		partikkelit[i].anim = 0;
		partikkelit[i].b = 0;
		partikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
		partikkelit[i].x = 0;
		partikkelit[i].y = 0;
		partikkelit[i].paino = 0;
		partikkelit[i].vari = 0;
		i++;
	}

	return 0;
}
//PK_Particles_New
void PK_Partikkeli_Uusi(int tyyppi, double x, double y, double a, double b, int aika, double paino, int vari){ //Create particles
	if (x < kamera_x+RUUDUN_LEVEYS+10 && x > kamera_x-10 &&
		y < kamera_y+RUUDUN_KORKEUS+10 && y > kamera_y-10){
		if (partikkeli_index >= MAX_PARTIKKELEITA)
			partikkeli_index = 0;

		partikkelit[partikkeli_index].a = a;
		partikkelit[partikkeli_index].aika = aika;
		partikkelit[partikkeli_index].anim = rand()%63;
		partikkelit[partikkeli_index].b = b;
		partikkelit[partikkeli_index].tyyppi = tyyppi;
		partikkelit[partikkeli_index].x = x;
		partikkelit[partikkeli_index].y = y;
		partikkelit[partikkeli_index].paino = paino;
		partikkelit[partikkeli_index].vari = vari;

		partikkeli_index++;
	}
}
//PK_Particles_Draw
int	PK_Partikkelit_Piirra(){
	int pros;
	int piirtoleveys, piirtokorkeus;

	if (RAJAA_KARTANPIIRTOALUE){
		piirtoleveys  = KARTANPIIRTO_LEVEYS;
		piirtokorkeus = KARTANPIIRTO_KORKEUS;
	}
	else{
		piirtoleveys  = RUUDUN_LEVEYS;
		piirtokorkeus = RUUDUN_KORKEUS;
	}

	for (int i=0;i<MAX_PARTIKKELEITA;i++){
		if (partikkelit[i].aika > 0){
			if (partikkelit[i].x < kamera_x+piirtoleveys &&
				partikkelit[i].x > kamera_x &&
				partikkelit[i].y < kamera_y+piirtokorkeus &&
				partikkelit[i].y > kamera_y) {
				if (partikkelit[i].aika < 100)
					pros = partikkelit[i].aika;
				else
					pros = 100;

				switch (partikkelit[i].tyyppi){
				case PARTIKKELI_TAHTI		:	PK_Piirra_Tahti((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Star
				case PARTIKKELI_HOYHEN		:	PK_Piirra_Hoyhen((int)partikkelit[i].x,(int)partikkelit[i].y,partikkelit[i].anim);
												break; //Feather
				case PARTIKKELI_POLYPILVI	:	PK_Piirra_Polypilvi((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Dust
				case PARTIKKELI_SAVUPILVI	:	PK_Piirra_Savupilvi((int)partikkelit[i].x,(int)partikkelit[i].y, partikkelit[i].anim);
												break; //Smoke
				case PARTIKKELI_VALO		:	PK_Piirra_Valo((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Light
				case PARTIKKELI_KIPINA		:	PK_Piirra_Kipina((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Spark
				case PARTIKKELI_PISTE		:	PK_Piirra_Piste((int)partikkelit[i].x,(int)partikkelit[i].y,pros,partikkelit[i].vari);
												break; //Score
				default						:	break;
				}
			}
		}
		else
			partikkelit[i].aika = 0;
	}
	return 0;
}
//PK_Particles_Update
int	PK_Partikkelit(){
	for (int i=0;i<MAX_PARTIKKELEITA;i++){
		if (partikkelit[i].aika > 0){
			partikkelit[i].x += partikkelit[i].a;
			partikkelit[i].y += partikkelit[i].b;

			if (partikkelit[i].paino > 0)
				partikkelit[i].b += partikkelit[i].paino;

			partikkelit[i].aika--;
		}
	}
	return 0;
}
//PK_Particles_Empty
int PK_Taustapartikkelit_Tyhjenna(){
	int i = 0;

	taustapartikkeli_index = 0;

	//kartta->ilma = ILMA_SADE;

	while (i < MAX_TAUSTAPARTIKKELEITA)
	{
		taustapartikkelit[i].a = rand()%10-rand()%10;
		taustapartikkelit[i].aika = 1;
		taustapartikkelit[i].anim = rand()%10;
		taustapartikkelit[i].b = rand()%9+1;
		taustapartikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
		taustapartikkelit[i].x = rand()%KARTANPIIRTO_LEVEYS;
		taustapartikkelit[i].y = rand()%KARTANPIIRTO_KORKEUS;
		taustapartikkelit[i].paino = rand()%10;
		taustapartikkelit[i].vari = 0;
		i++;
	}

	if (kartta->ilma == ILMA_SADE || kartta->ilma == ILMA_SADEMETSA)
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_VESIPISARA;
		}

	if (kartta->ilma == ILMA_METSA || kartta->ilma == ILMA_SADEMETSA)
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/8;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_LEHTI1 + rand()%4;
		}

	if (kartta->ilma == ILMA_LUMISADE){

		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/2;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_HIUTALE4;
		}
		for(i=0;i < MAX_TAUSTAPARTIKKELEITA/3;i++)
		{
			taustapartikkelit[i].tyyppi = TAUSTAPARTIKKELI_HIUTALE1 + rand()%2+1;//3
		}
	}
	return 0;
}

void PK_Taustapartikkelit_Vesipisara(PK2PARTIKKELI &p){
	p.y += p.b / 3.0 + 2.0;

	int kx = (int)(p.x-kamera_x);
	int ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+1,ky+4,40+(int)p.b);
};
void PK_Taustapartikkelit_Lehti1(PK2PARTIKKELI &p){
	p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,96+6+(int)p.b+(int)(p.x+p.y)%10);
};
void PK_Taustapartikkelit_Lehti2(PK2PARTIKKELI &p){
	p.x += p.a / 14.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/10)%4)*23;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1+frame,141,21+frame,152);
};
void PK_Taustapartikkelit_Lehti3(PK2PARTIKKELI &p){
	p.x += p.a / 12.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%4)*20;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,93+frame,141,109+frame,150);

};
void PK_Taustapartikkelit_Lehti4(PK2PARTIKKELI &p){
	p.x += p.a / 11.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y),
		frame = (int(p.y/5)%2)*14;

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,173+frame,141,183+frame,150);
};
void PK_Taustapartikkelit_Hiutale1(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/50.0;
	p.y += p.b / 7.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,1,155,8,162);
};
void PK_Taustapartikkelit_Hiutale2(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.x += sin_table[int(p.y)%360]/100.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,11,155,16,160);
};
void PK_Taustapartikkelit_Hiutale3(PK2PARTIKKELI &p){
	//p.x += p.a / 11.0;
	p.y += p.b / 8.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_Image_CutClip(kuva_peli,kx,ky,19,155,22,158);
};
void PK_Taustapartikkelit_Hiutale4(PK2PARTIKKELI &p){
	//p.x += p.a / 9.0;
	p.y += p.b / 9.0;

	int kx = (int)(p.x-kamera_x),
		ky = (int)(p.y-kamera_y);

	PisteDraw2_ScreenFill(kx,ky,kx+2,ky+2,20+(int)p.b);
};
int	PK_Taustapartikkelit(){

	for (int i=0;i<MAX_TAUSTAPARTIKKELEITA;i++)
	{
		if (taustapartikkelit[i].tyyppi != PARTIKKELI_EI_MIKAAN)
		{
			switch(taustapartikkelit[i].tyyppi)
			{
				case TAUSTAPARTIKKELI_VESIPISARA : PK_Taustapartikkelit_Vesipisara(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI1	 : PK_Taustapartikkelit_Lehti1(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI2	 : PK_Taustapartikkelit_Lehti2(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI3	 : PK_Taustapartikkelit_Lehti3(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_LEHTI4	 : PK_Taustapartikkelit_Lehti4(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE1	 : PK_Taustapartikkelit_Hiutale1(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE2	 : PK_Taustapartikkelit_Hiutale2(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE3	 : PK_Taustapartikkelit_Hiutale3(taustapartikkelit[i]);break;
				case TAUSTAPARTIKKELI_HIUTALE4	 : PK_Taustapartikkelit_Hiutale4(taustapartikkelit[i]);break;
				default : taustapartikkelit[i].tyyppi = PARTIKKELI_EI_MIKAAN;
			}

			if (taustapartikkelit[i].x  >  kamera_x + KARTANPIIRTO_LEVEYS)
				taustapartikkelit[i].x  =  kamera_x + int(taustapartikkelit[i].x - kamera_x + KARTANPIIRTO_LEVEYS) % KARTANPIIRTO_LEVEYS;

			if (taustapartikkelit[i].x  <  kamera_x)
				taustapartikkelit[i].x  =  kamera_x + KARTANPIIRTO_LEVEYS - int(kamera_x - taustapartikkelit[i].x) % KARTANPIIRTO_LEVEYS;

			if (taustapartikkelit[i].y  >  kamera_y + KARTANPIIRTO_KORKEUS)
				taustapartikkelit[i].y  =  kamera_y + int(taustapartikkelit[i].y - kamera_y + KARTANPIIRTO_KORKEUS) % KARTANPIIRTO_KORKEUS;

			if (taustapartikkelit[i].y  <  kamera_y)
				taustapartikkelit[i].y  =  kamera_y + KARTANPIIRTO_KORKEUS - int(kamera_y - taustapartikkelit[i].y) % KARTANPIIRTO_KORKEUS;
		}
	}
	return 0;
}

void PK_Tehoste_Hoyhenet(DWORD x, DWORD y){
	for (int i=0;i<9;i++)//6
		PK_Partikkeli_Uusi(	PARTIKKELI_HOYHEN,x+rand()%17-rand()%17,y+rand()%20-rand()%10,
							(rand()%16-rand()%16)/10.0,(45+rand()%45)/100.0,300+rand()%40,0,0);
}
void PK_Tehoste_Loiskahdus(DWORD x, DWORD y, BYTE vari){
	/*
	for (int i=0;i<12;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_VALO,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5,(rand()%7-rand()%7)/3,
							rand()%50+60,0.025,vari);*/
	for (int i=0;i<7;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_KIPINA,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%4-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	for (int i=0;i<20;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x+rand()%17-13,y+rand()%17-13,
							(rand()%5-rand()%5)/4.0,(rand()%2-rand()%7)/3.0,
							rand()%50+40,0.025,vari);//0.015

	PK_Soita_Aani(loiskahdus_aani, 100, x, y, AANET_SAMPLERATE, true);
}
void PK_Tehoste_Rajahdys(DWORD x, DWORD y, BYTE vari){
	int i;

	for (i=0;i<3;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%4)+3) / -10.0,450,0.0,vari);

	for (i=0;i<9;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_VALO,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,vari);

	for (i=0;i<8;i++)//8//10
		PK_Partikkeli_Uusi(	PARTIKKELI_KIPINA,x+rand()%17-13,y+rand()%17-13,
							(rand()%3-rand()%3),//(rand()%7-rand()%7)/5,
							(rand()%7-rand()%7)/3,
							rand()%20+60,0.015,vari);//50+60

	for (i=0;i<20;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x+rand()%17-13,y+rand()%17-13,
							(rand()%7-rand()%7)/5.0,(rand()%7-rand()%7)/3.0,
							rand()%40+60,0.025,vari);
}
void PK_Tehoste_Savu(DWORD x, DWORD y, BYTE vari){
	for (int i=0;i<3;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,vari);
	for (int i=0;i<6;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_POLYPILVI,x+rand()%30-rand()%30-10,y+rand()%30-rand()%30+10,
							0,-0.3,rand()%50+60,0,vari);
}
void PK_Tehoste_Savupilvet(DWORD x, DWORD y){
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(	PARTIKKELI_SAVUPILVI,x+rand()%17-32,y+rand()%17,
							0,double((rand()%3)+3) / -10.0/*-0.3*/,450,0.0,0);
}
void PK_Tehoste_Tahdet(DWORD x, DWORD y, BYTE vari){
	for (int i=0;i<5;i++)
		PK_Partikkeli_Uusi(PARTIKKELI_TAHTI,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0/* 0.05*/,vari);//300

	for (int i=0;i<3;i++)//12
		PK_Partikkeli_Uusi(	PARTIKKELI_PISTE,x-5, y-5, (rand()%3-rand()%3)/1.5, rand()%3*-1,100,(rand()%5+5)/100.0,vari);
}
void PK_Tehosteet(BYTE tehoste, DWORD x, DWORD y){
	switch (tehoste){
	case TUHOUTUMINEN_HOYHENET			: PK_Tehoste_Hoyhenet(x, y); break;
	case TUHOUTUMINEN_TAHDET_HARMAA		: PK_Tehoste_Tahdet(x,y,0);break;
	case TUHOUTUMINEN_TAHDET_SININEN	: PK_Tehoste_Tahdet(x,y,32);break;
	case TUHOUTUMINEN_TAHDET_PUNAINEN	: PK_Tehoste_Tahdet(x,y,64);break;
	case TUHOUTUMINEN_TAHDET_VIHREA		: PK_Tehoste_Tahdet(x,y,96);break;
	case TUHOUTUMINEN_TAHDET_ORANSSI	: PK_Tehoste_Tahdet(x,y,128);break;
	case TUHOUTUMINEN_TAHDET_VIOLETTI	: PK_Tehoste_Tahdet(x,y,160);break;
	case TUHOUTUMINEN_TAHDET_TURKOOSI	: PK_Tehoste_Tahdet(x,y,192);break;
	case TUHOUTUMINEN_RAJAHDYS_HARMAA	: PK_Tehoste_Rajahdys(x,y,0);break;
	case TUHOUTUMINEN_RAJAHDYS_SININEN	: PK_Tehoste_Rajahdys(x,y,32);break;
	case TUHOUTUMINEN_RAJAHDYS_PUNAINEN	: PK_Tehoste_Rajahdys(x,y,64);break;
	case TUHOUTUMINEN_RAJAHDYS_VIHREA	: PK_Tehoste_Rajahdys(x,y,96);break;
	case TUHOUTUMINEN_RAJAHDYS_ORANSSI	: PK_Tehoste_Rajahdys(x,y,128);break;
	case TUHOUTUMINEN_RAJAHDYS_VIOLETTI	: PK_Tehoste_Rajahdys(x,y,160);break;
	case TUHOUTUMINEN_RAJAHDYS_TURKOOSI	: PK_Tehoste_Rajahdys(x,y,192);break;
	case TUHOUTUMINEN_SAVU_HARMAA		: PK_Tehoste_Savu(x,y,0);break;
	case TUHOUTUMINEN_SAVU_SININEN		: PK_Tehoste_Savu(x,y,32);break;
	case TUHOUTUMINEN_SAVU_PUNAINEN		: PK_Tehoste_Savu(x,y,64);break;
	case TUHOUTUMINEN_SAVU_VIHREA		: PK_Tehoste_Savu(x,y,96);break;
	case TUHOUTUMINEN_SAVU_ORANSSI		: PK_Tehoste_Savu(x,y,128);break;
	case TUHOUTUMINEN_SAVU_VIOLETTI		: PK_Tehoste_Savu(x,y,160);break;
	case TUHOUTUMINEN_SAVU_TURKOOSI		: PK_Tehoste_Savu(x,y,192);break;
	case TUHOUTUMINEN_SAVUPILVET		: PK_Tehoste_Savupilvet(x,y);break;
	default	: break;
	}
}
