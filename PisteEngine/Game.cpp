/* GAME.CPP		PisteEngine Esimerkki-ohjelma v0.1 by Janne Kivilahti 27.12.2001
				Ohjelman toiminta:
				Ruudulla on taustakuva, joka vierii vasemmalta oikealle.
				Ruudulla leijuu pallo, jota voi ohjata hiirellä tai peliohjaimella.
				A-näppäimestä ruutu feidaa mustaksi ja S-näppäimestä takaisin.
				Ohjelman suoritus loppuu ESC:stä, hiiren oikeasta napista tai peliohjaimen napista 2.
				
				Tällä hetkellä PisteEnginestä on valmiina:
				PisteInput = Hiiren, näppäimistön ja peliohjainten hallinta.
				PisteDraw  = Ruudulle piirtely. Sisältää myös PisteFontin.
				PisteWait  = Tahdistus (peli pyörii kaikissa koneissa samaa nopeutta).

				Ikkuna-sälän piilottaminen ei ollutkaan niin yksinkertaista kuin kuvittelin,
				joten kaikki ikkunan alustamiseen yms. liittyvä on "piilotettu" ihan koodin
				pohjalle.

				En ole kommentoinut noita Piste-koodeja juuri mitenkään, mutta tässä esimerkissä
				on aika paljon selityksiä. Koeta saada niistä jotakin irti :)

				Rajoituksia:
				
			-	Vapaassa piirrossa ei voi olla kuin yksi pinta lukittuna kerrallaan.
			-	Kun jokin pinta on lukittuna vapaassa piirrossa, 
				et voi käyttää PisteDraw_Font_Kirjoita()-metodia.
			-	Älä flippaa samasta pinnasta samaan pintaan.

*/


/* PRE DEFINITIONS ----------------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN  
#define INITGUID 

/* INCLUDES -----------------------------------------------------------------------------------*/

#include <windows.h>   
#include <windowsx.h> 
#include <mmsystem.h>
#include <math.h>
#include "PisteInput.h"
#include "PisteDraw.h"
#include "PisteSound.h"
#include "PisteWait.h"
#include "resource.h"

/* TYYPPIMÄÄRITTELYT ---------------------------------------------------------------------------*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

/* MÄÄRITTELYT --------------------------------------------------------------------------------*/

#define WINDOW_CLASS_NAME "WINCLASS1"
#define	GAME_NAME		  "ANY GAME"

const int SCREEN_WIDTH			= 800;	// Älä koske! Muut arvot eivät (vielä) toimi!
const int SCREEN_HEIGHT			= 600;	// Älä koske!
const int SCREEN_BPP			= 8;	// Älä koske!
const int MAX_COLORS_PALETTE	= 256;	// Älä koske!

/* GLOBAALIT MUUTTUJAT ---------------------------------------------------------------------------*/

HWND      ikkunan_kahva			= NULL; // pääikkunan kahva
HINSTANCE hinstance_app			= NULL; // hinstance?
HDC       global_dc				= NULL; // global dc?

bool DirectX_virhe				= false;// jos tämä muuttuu todeksi niin ohjelma lopetetaan
char DirectX_virhe_viesti[2]	= " ";  // ei vielä (kunnolla) käytössä

bool window_closed				= false;// onko ikkuna kiinni

int	kuvabufferi1	= NULL;				// indeksi PisteDraw:n kuvabufferitaulukkoon
int	kuvabufferi2	= NULL;				// indeksi PisteDraw:n kuvabufferitaulukkoon				

int	fontti1,							// indeksi PisteDraw:n fonttitaulukkoon
	fontti2;							// indeksi PisteDraw:n fonttitaulukkoon

int aani1;								// indeksi PisteSoundin äänitaulukkoon
int aani2;								// indeksi PisteSoundin äänitaulukkoon

int pallo_x = 320;
int pallo_y = 240;
int map_x = 0;

int key_delay = 0;

/* PELI ---------------------------------------------------------------------------------------*/

int Game_Init(void)
{

	// Alustetaan PisteDraw - Ruudulle piirtely
	if ((PisteSound_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 1, 22050, 8)) == PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}	
	
	// Alustetaan PisteInput - Hiiren ja näppäimistön hallinta
	if ((PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app)) == PI_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Alustetaan PisteDraw - Ruudulle piirtely
	if ((PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
								  SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Luodaan ensimmäinen kuvabufferi:
	// SCREEN_WIDTH ja SCREEN_HEIGHT: koko ruudun korkeus ja leveys (melko itsestäänselvää?)
	// true: Bufferi sijaitsee videomuistissa (VRAM). Jos systeemimuistissa niin false.
	// 255: Läpinäkyvyysväri.
	// Metodi palauttaa integerin, joka on indeksiavain bufferitaulukkoon. Eli ota avain talteen. 
	if ((kuvabufferi1 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	
	// Tehdään toinen samanlainen
	if ((kuvabufferi2 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255)) == PD_VIRHE)
	{
		DirectX_virhe = true;	
		return 1;
	}


	// Ladataan kuva ensimmäiseen bufferiin. Tässä tapauksessa taustakuva.  
	// kuvabufferi1: indeksi PisteDraw:n taulukkoon, jossa on kuvabuffereita.
	// "wormz.bmp": Kuva joka ladataan.
	// true: Ladataan ja määrätään paletti. Jos ei ladata palettia, niin false
	
	if (PisteDraw_Lataa_Kuva(kuvabufferi1,"wormz.bmp", true) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}


	// Ladataan toinen kuva tokaan bufferiin. Ei ladata palettia. Kuvassa on spritejä yms...
	if (PisteDraw_Lataa_Kuva(kuvabufferi2,"wormz2.bmp", false) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}


	// Luodaan uusi fontti. Vähän kinkkisempi juttu: Joutuu laskeskelemaan pikseleitä kuvassa.
	// kuvabufferi2: Bufferi, josta fontin grafiikat otetaan.
	// 1,422: X- ja Y-kordinaatit josta kohtaa bufferista fontin grafiikat alkavat. Vasen- ja yläkulma.
	// 14,14: Yhden kirjaimen leveys ja korkeus. 
	// 29: Kuinka monta eri kirjainta tässä fontissa on.
	if ((fontti1 = PisteDraw_Font_Uusi(kuvabufferi2,1,422,14,14,29)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	// Ja sama juttu...
	if ((fontti2 = PisteDraw_Font_Uusi(kuvabufferi2,1,467,6,12,46)) == PD_VIRHE)
	{
		DirectX_virhe = true;
		return 1;
	}

	// Asetetaan taustabufferille reunat joiden yli ei voi piirtää. PD_TAUSTABUFFER viittaa
	// siihen kuvabufferiin, joka flipataan näytölle kerran framessa. Jos halutaan asettaa
	// reunat jollekin muulle kuvabufferille, käytetään indeksiavainta: esim. kuvabufferi1.
	// HUOM.! Jokaiselle PisteDraw_Buffer_Uusi() metodilla luodulle kuvabufferille asetetaan
	// automaattisesti reunat: ruudun leveys ja korkeus. Mutta tällä siis voidaan erikseen
	// asettaa ne.
	// PD_TAUSTABUFFER: kuvabufferi, jolle reunat asetetaan.
	// 20,20,620,460: Vasen reuna, yläreuna, oikea reuna, alareuna.
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,20,20,620,460);


	if ((aani1 = PisteSound_SFX_Uusi("sfx1.wav"))==PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;		
	}

	if ((aani2 = PisteSound_SFX_Uusi("sfx2.wav"))==PS_VIRHE)
	{
		DirectX_virhe = true;
		return 1;		
	}	

	// Toimii kuin TickCount (tai itseasiassa on se). Aloitetaan ajanotto ekan kerran.
	PisteWait_Start();


	
/*	Tai koko homma lyhyesti ilman virhetarkistuksia ja höpinöitä...

	PisteInput_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &)hinstance_app);
	PisteDraw_Alusta((HWND &)ikkunan_kahva, (HINSTANCE &) hinstance_app, 
	 			     SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, MAX_COLORS_PALETTE);
	kuvabufferi1 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	kuvabufferi2 = PisteDraw_Buffer_Uusi(SCREEN_WIDTH,SCREEN_HEIGHT, true, 255);
	PisteDraw_Lataa_Kuva(kuvabufferi1,"wormz.bmp", true);
	PisteDraw_Lataa_Kuva(kuvabufferi2,"wormz2.bmp", false);
	PisteDraw_Aseta_Klipperi(PD_TAUSTABUFFER,20,20,620,460);
	fontti1 = PisteDraw_Font_Uusi(kuvabufferi2,1,422,14,14,29);
	PisteWait_Start();
*/

	return 0;
}

int Game_Main_Piirra(void)
{
	int x, y;
	map_x = 1 + map_x%640;

	/* TÄYTETÄÄN RUUTU HARMAALLA VÄRILLÄ */

	// Kuvaruudun täyttö värillä:
	// PD_TAUSTABUFFER: bufferi joka täytetään
	// 10: Paletin väri jolla täytetään.
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,10);
	// Tai:
	// PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER, 0, 0, 640, 480, 10);

	/* TEHDÄÄN TAUSTAN RULLAUS */

	// KOKONAISTEN buffereiden kopiointi toisiin buffereihin:
	// kuvabufferi1: lähdebufferi, josta kopioidaan
	// PD_TAUSTABUFFER: kohdebufferi johon kopioidaan.
	// map_x,0: X ja Y eli mihin kohtaan kohdebufferia kopioidaan

	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x,0);
	// Sama juttu kuin edellisessä
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x-640,0);

	/* PIIRRETÄÄN PALLO HIIREN KURSORIN KOHDALLE */

	// Bufferin OSAN kopiointi toiseen bufferiin tiettyyn kohtaan:
	// kuvabufferi2: Lähdebufferi josta kopioidaan
	// PD_TAUSTABUFFER: kohdebufferi johon kopioidaan (tässä tapauksessa pinta joka lopussa flipataan näytölle)
	// pallo_x, pallo_y: X- ja Y-kordinaatit mihin kohtaan kohdebufferia kopioidaan (vasen yläkulma)
	// 200,141,219,160: Alue lähdebufferista, jolta kopioidaan.
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,200,141,219,160);
	//PisteDraw_Buffer_Flip(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,true,true);


	/* PIIRRETÄÄN LUMISADE KUVABUFFERIIN JA TAUSTABUFFERIIN*/ 
	
	// Valmistellaan Vapaa piirto bufferiin
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	// Ennen kuin voidaan sorkkia mitään kuvabufferia niin se pitää lukita ensin.
	// kuvabufferi1: Bufferi joka lukitaan piirtämistä varten
	// buffer: Pointteri kuvabufferin sisältöön
	// tod_leveys: Kuvabufferin todellinen leveys 
	// Vaikka ruudun leveys olisi asetettu 640x480, se voi oikeasti olla leveämpi
	PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (DWORD &)tod_leveys);
	
	// Piirretään kuvabufferiin lumisadetta

	for (x=100;x<200;x++)
		for (y=100;y<200;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	PisteDraw_Piirto_Lopeta(kuvabufferi1); //Tässä välissä mahdollinen, mutta ei pakollinen
	
	// Valmistellaan toinen bufferi piirtämistä varten. Sitä ennen tarkistetaan,
	// onko jokin toinen bufferi lukittu. Jos on niin vapautetaan se ensin.
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);
	
	for (x=300;x<350;x++)
		for (y=300;y<350;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	// Lopetaan piirtäminen vapauttamalla lukittu bufferi.
	PisteDraw_Piirto_Lopeta(PD_TAUSTABUFFER);

	/* PIIRRETÄÄN "TESTI"-TEKSTI RUUDULLE */

	// Kirjoitetaan teksti ruudulle aikaisemmin varatulla fontilla //
	// fontti1: indeksiavain luotuun fonttiin jolla kirjoitetaan
	// "testi": oiskohan teksti joka ruudulle kirjoitetaan :)
	// 10,10  : mihin kirjoitetaan.
	// VARO! Fontin piirto ei reunoista piittaa vaan kirjoittaa surutta
	// myös ruudun ulkopuoliseen muistiin jos sinne pääsee käsiksi.
	// Paluuarvona saadaan kirjoitetun tekstin leveys pikseleinä
	PisteDraw_Font_Kirjoita(fontti1,"piste engine testi",PD_TAUSTABUFFER,10,10);
	
	/* PIIRRETÄÄN OHJETEKSTEJÄ-TEKSTEJÄ RUUDULLE */

	int tekstin_leveys = 0;
	tekstin_leveys += PisteDraw_Font_Kirjoita(fontti2,"lopetus: esc, oikea hiiren nappi tai peliohjaimen nappi 2.",PD_TAUSTABUFFER,10,40);
	PisteDraw_Font_Kirjoita(fontti2,"a ja s: feidaus.",PD_TAUSTABUFFER,10,60);

	/* PIIRRETÄÄN RUUDULLE VAPAAN VIDEOMUISTIN MÄÄRÄ*/

	char  vram[30];
	ltoa(PisteDraw_Videomuistia(), vram, 10);
	PisteDraw_Font_Kirjoita(fontti2,vram,PD_TAUSTABUFFER,300,10);

	
	/* PIIRRETÄÄN RUUDULLE TEKSTI JOS PELIOHJAIMEN NAPPIA 2 ON PAINETTU */

	if (PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1, PI_OHJAIN_NAPPI_1))
		PisteDraw_Font_Kirjoita(fontti2,"peliohjaimen nappi 1 painettu",PD_TAUSTABUFFER,10,75);

	/* ODOTETAAN JOS LIIAN NOPEA KONE */
	
	// Jos oltiin liian nopeita, niin venataan hetki. Sama kuin esim. TickWait.
	// 10 sadasosasekuntia (muistaakseni)
	PisteWait_Wait(10);

	/* FLIPATAAN TAUSTABUFFERI (PD_TAUSTABUFFER) NÄYTÖLLE */
	
	// Dumpataan kaksoispuskurin PD_TAUSTABUFFER sisältö näyttömuistiin
	PisteDraw_Paivita_Naytto();

	// Käynnistetään taas ajastin, jota tutkitaan taas PisteWait()-metodilla.
	PisteWait_Start();



	/* Kaikki edellinen ilman turhaa sälää...

	int x, y;
	map_x = 1 + map_x%640;
	PisteDraw_Buffer_Tayta(PD_TAUSTABUFFER,10);
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x,0);
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi1,PD_TAUSTABUFFER,map_x-640,0);	
	PisteDraw_Buffer_Flip_Nopea(kuvabufferi2,PD_TAUSTABUFFER,pallo_x,pallo_y,200,141,219,160);  
	UCHAR *buffer = NULL;
	DWORD tod_leveys;
	PisteDraw_Piirto_Aloita(kuvabufferi1, *&buffer, (DWORD &)tod_leveys);

	for (x=100;x<200;x++)
		for (y=100;y<200;y++)
			buffer[x+y*tod_leveys] = rand()%30;
		
	PisteDraw_Piirto_Aloita(PD_TAUSTABUFFER, *&buffer, (DWORD &)tod_leveys);

	for (x=300;x<350;x++)
		for (y=300;y<350;y++)
			buffer[x+y*tod_leveys] = rand()%30;

	PisteDraw_Piirto_Lopeta();
	PisteDraw_Font_Kirjoita(fontti1,"testi",PD_TAUSTABUFFER,10,10);
	int tekstin_leveys = 0;
	tekstin_leveys += PisteDraw_Font_Kirjoita(fontti2,"lopetus: esc tai oikea hiiren nappi.",PD_TAUSTABUFFER,10,40);
	PisteDraw_Font_Kirjoita(fontti2,"a ja s: feidaus.",PD_TAUSTABUFFER,10,60);
	char  vram[30];
	ltoa(PisteDraw_Videomuistia(), vram, 10);
	PisteDraw_Font_Kirjoita(fontti2,vram,PD_TAUSTABUFFER,300,10);	
	PisteWait_Wait(10);
	PisteDraw_Paivita_Naytto();
	PisteWait_Start();
	*/

	return 0;
}

int Game_Main(void)
{
	if (window_closed)
		return(0);

	/* HAETAAN NÄPPÄIMISTÖN, HIIREN JA PELIOHJAINTEN TÄMÄNHETKISET TILAT */

	// Näppäimistö 
	if (!PisteInput_Hae_Nappaimet())		//Haetaan näppäinten tilat
		DirectX_virhe = true;
	
	// Hiirulainen
	if (!PisteInput_Hae_Hiiri())			//Haetaan hiiren tila
		DirectX_virhe = true;

	// Kaikki peliohjaimet
	if (!PisteInput_Hae_Ohjaimet()){}
		//DirectX_virhe = true;

	// Lisätään pallon x- ja y-muuttujiin hiiren kursorin sijainnin muutos

	pallo_x = PisteInput_Hiiri_X(pallo_x);	//Tai: pallo_x += PisteInput_Hiiri_X(0);
	pallo_y = PisteInput_Hiiri_Y(pallo_y);
	
	// Lisätään pallon x- ja y-muuttujiin peliohjaimen 1 liikkeet

	pallo_x += PisteInput_Ohjain_X(PI_PELIOHJAIN_1)/50;
	pallo_y += PisteInput_Ohjain_Y(PI_PELIOHJAIN_1)/50;

	if (pallo_x > SCREEN_WIDTH)
		pallo_x = SCREEN_WIDTH;

	if (pallo_y > SCREEN_HEIGHT)
		pallo_y = SCREEN_HEIGHT;

	if (pallo_x < 0)
		pallo_x = 0;

	if (pallo_y < 0)
		pallo_y = 0;

	Game_Main_Piirra();

	
	/* FEIDATAAN PALETTIA JOS KÄYTTÄJÄ PAINELEE A- JA S-NAPPULOITA */

	// Luetaan onko käyttäjä painanut A-näppäintä
	if (PisteInput_Keydown(DIK_A))
		PisteDraw_Fade_Paletti_Out(PD_FADE_HIDAS);
		// Jos on, niin aloitetaan paletin feidaus mustaksi.
		// Valmiita nopeuksia: PD_FADE_HIDAS (=1), PD_FADE_NORMAALI (=2), PD_FADE_NOPEA (=5)
		// Parametriksi voi antaa oikeastaan minkä tahansa positiivisen arvon.
		// Feidaus tapahtuu arvosta 100 - 0. 
	if (PisteInput_Keydown(DIK_S))
		PisteDraw_Fade_Paletti_In(PD_FADE_NORMAALI);

	if (key_delay == 0)
	{

		if (PisteInput_Keydown(DIK_1))
		{
			if (PisteSound_SFX_Soita(aani1) == PS_VIRHE)
				DirectX_virhe = true;
			key_delay = 30;
		}
		
		if (PisteInput_Keydown(DIK_2))
		{
			if (PisteSound_SFX_Soita(aani2) == PS_VIRHE)
				DirectX_virhe = true;
			key_delay = 30;
		}
	}
	else
		key_delay--;
	
	/* LOPETETAAN OHJELMA JOS KÄYTTÄJÄ PAINAA ESC:Ä TAI HIIREN OIKEAA NAPPIA */

	// Jos käyttäjä painaa ESC:ä tai Hiiren oikeaa namiskuukkelia, tai peliohjaimen nappia 2,
	// poistutaan ohjelmasta.
	// PisteInputHiiriVasen() = hiiren vasen nappi
	
	if (PisteInput_Keydown(DIK_ESCAPE) 
		|| PisteInput_Hiiri_Oikea() 
		|| PisteInput_Ohjain_Nappi(PI_PELIOHJAIN_1, PI_OHJAIN_NAPPI_2))
	{
		SendMessage(ikkunan_kahva, WM_CLOSE,0,0);
		window_closed = true;
	}

	return 0;
}

int Game_Quit(void)
{

	PisteSound_Lopeta();
	PisteDraw_Lopeta();
	PisteInput_Lopeta();

	// Jos on ilmennyt virhe niin näytetään tekstiboksi jossa lukee "Virhe!".
	// Kyllä se käyttäjä yllättyy tästä informatiivisesta viestistä :)

	if (DirectX_virhe)
	{
		MessageBox(ikkunan_kahva, PisteDraw_Virheilmoitus(),"Virhe!",
				   MB_OK | MB_ICONEXCLAMATION);
		
		MessageBox(ikkunan_kahva, PisteSound_Virheilmoitus(),"Virhe!",
				   MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}













/* IKKUNA -------------------------------------------------------------------------------------*/

// Seuraa ikkuna-koodia. Allaoleva on ikkunan "tapahtumakuuntelija".


LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	PAINTSTRUCT		ps;		
	HDC				hdc;	

	switch(msg)
	{	
		case WM_CREATE: 
	    {
		
			return(0);
		}	break;
   
		case WM_PAINT: 
		{

			hdc = BeginPaint(hwnd,&ps);	 

			EndPaint(hwnd,&ps);

			return(0);
		}	break;

		case WM_DESTROY: 
		{

			PostQuitMessage(0);
			return(0);
		}	break;

		default:break;

    } 

	return (DefWindowProc(hwnd, msg, wparam, lparam));

} 

// Kaiken alku ja juuri: WinMain. Tästä se kaikki alkaa ja tämän sisällä peli pyörii.

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

	WNDCLASSEX winclass; 
	HWND	   hwnd;	 
	MSG		   msg;		 

	winclass.cbSize         = sizeof(WNDCLASSEX);
	winclass.style			= CS_DBLCLKS | CS_OWNDC | 
							  CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor		= LoadCursor(hinstance, MAKEINTRESOURCE(IDC_CURSOR1)); 
	winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= WINDOW_CLASS_NAME;
	winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);


	hinstance_app = hinstance;

	if (!RegisterClassEx(&winclass))
		return(0);

	if (!(hwnd = CreateWindowEx(NULL,                  
		                        WINDOW_CLASS_NAME,     
							    GAME_NAME, 
							    WS_POPUP | WS_VISIBLE,
						 	    0,0,	  
							    SCREEN_WIDTH, SCREEN_HEIGHT,  
							    NULL,	   
							    NULL,	  
							    hinstance,
							    NULL)))	
		return(0);

	ikkunan_kahva = hwnd;

	Game_Init();

	ShowCursor(FALSE);

	while(!DirectX_virhe)
	{
    
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
	   
			if (msg.message == WM_QUIT)
				break;
	   
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		} 
    
		Game_Main();

	} 

	Game_Quit();

	ShowCursor(TRUE);

	return(msg.wParam);
} 