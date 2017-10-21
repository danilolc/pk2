#ifndef PK2_GRAPHICS
#define PK2_GRAPHICS

#define MAX_FADETEKSTEJA 40

//PK2FADETEXT
struct PK2FADETEKSTI{
	char teksti[20];
	int fontti;
	int x,y,ajastin;
};

int  kuva_peli = -1;
int  kuva_peli_sysmem = -1;
int  kuva_tausta = -1;

int kamera_x = 0;
int kamera_y = 0;
double dkamera_x = 0;
double dkamera_y = 0;
double dkamera_a = 0;
double dkamera_b = 0;

int fontti1;
int fontti2;
int fontti3;
int fontti4;
int fontti5;

PK2FADETEKSTI fadetekstit[MAX_FADETEKSTEJA];
int fadeteksti_index = 0;

double cos_table[360];
double sin_table[360];

int degree = 0,
	degree_temp = 0;


//PK_Calculate_Tables
void PK_Laske_Taulut(){
	int i;
	for (i=0; i<360; i++) cos_table[i] = cos(3.1415*2* (i%360)/180)*33;
	for (i=0; i<360; i++) sin_table[i] = sin(3.1415*2* (i%360)/180)*33;
}

//Create menu background shadow
int PK_Sumenna_Kuva(int kbuffer, DWORD kleveys, int kkorkeus, int startx){
	BYTE *buffer = NULL;
	DWORD leveys;
	BYTE vari,/* vari2, vari3,*/ vari32;
	DWORD x, mx, my;
	int y;
	double kerroin;


	if (PisteDraw2_DrawImage_Start(kbuffer,*&buffer,(DWORD &)leveys)==1)
		return 1;

	if (kleveys > leveys)
		kleveys = leveys;

	kkorkeus -= 2;
	kleveys  -= 2;

	kleveys += startx - 30;

	kerroin = 3;//2.25;//2

	//for (y=0;y<kkorkeus;y++)
	for (y=35;y<kkorkeus-30;y++)
	{
		my = (y)*leveys;
		//for(x=0;x<kleveys;x++)
		for(x=startx;x<kleveys-30;x++)
		{
			mx = x+my;
			vari   = buffer[mx];

			vari32 = VARI_TURKOOSI;//(vari>>5)<<5;
			vari %= 32;

			if (x == startx || x == kleveys-31 || y == 35 || y == kkorkeus-31)
				vari = int((double)vari / (kerroin / 1.5));//1.25
			else
				vari = int((double)vari / kerroin);//1.25

			vari += vari32;

			buffer[mx] = vari;
		}

		if (kerroin > 1.005)
			kerroin = kerroin - 0.005;
	}

	if (PisteDraw2_DrawImage_End(kbuffer)==1)
		return 1;

	return 0;
}

void PK_Alusta_Fadetekstit(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
		fadetekstit[i].ajastin = 0;
}

void PK_Fadeteksti_Uusi(int fontti, char *teksti, DWORD x, DWORD y, DWORD ajastin){
	fadetekstit[fadeteksti_index].fontti = fontti;
	strcpy(fadetekstit[fadeteksti_index].teksti,teksti);
	fadetekstit[fadeteksti_index].x = x;
	fadetekstit[fadeteksti_index].y = y;
	fadetekstit[fadeteksti_index].ajastin = ajastin;
	fadeteksti_index++;

	if (fadeteksti_index >= MAX_FADETEKSTEJA)
		fadeteksti_index = 0;
}

int PK_Fadetekstit_Piirra(){
	int pros;

	for (int i=0;i<MAX_FADETEKSTEJA;i++)
	{
		if (fadetekstit[i].ajastin > 0)
		{
			if (fadetekstit[i].ajastin > 50)
				pros = 100;
			else
				pros = fadetekstit[i].ajastin * 2;

			if (settings.lapinakyvat_objektit && pros < 100)
			{
				if (PisteDraw2_Font_WriteAlpha( fadetekstit[i].fontti,fadetekstit[i].teksti,
														fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y, pros)==-1)
					return 1;
			}
			else
				PisteDraw2_Font_Write( fadetekstit[i].fontti,fadetekstit[i].teksti,
									 fadetekstit[i].x-kamera_x, fadetekstit[i].y-kamera_y);

		}
	}
	return 0;
}

void PK_Fadetekstit(){
	for (int i=0;i<MAX_FADETEKSTEJA;i++)
	{
		if (fadetekstit[i].ajastin > 0)
		{
			fadetekstit[i].ajastin--;

			if (fadetekstit[i].ajastin%2 == 0)
				fadetekstit[i].y--;

			if (fadetekstit[i].x < kamera_x || fadetekstit[i].x > kamera_x + RUUDUN_LEVEYS ||
				fadetekstit[i].y < kamera_y || fadetekstit[i].y > kamera_y + RUUDUN_KORKEUS)
				fadetekstit[i].ajastin = 0;
		}
	}
}

#endif
