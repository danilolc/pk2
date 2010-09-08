/* PisteEngine PisteDraw v1.3 */

/*
Versio 1.2
----------

	Maksimimäärä buffereita korotettu 200 => 2000.

	PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);
	Klipperi lisätty.

	PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros)
	Lisätty. Toimii vain 256-värisessä, 32:n osiin pilkotussa paletissa. 
	pros -parametri on prosentti (0-100) 0= ei näy, 100 = ei läpinäkyvä.

	PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y)
	Piirtorutiini ottaa nyt leikkurit huomioon, joten se ei enää kirjoita ruudun ulkopuolelle.

	PisteDraw_Piirto_Lopeta(int i);
	Lisätty kuormitettu versio, jossa on int muotoinen parametri. 
	Mahdollistaa useiden pintojen lukitsemisen yhtäaikaa. Vanha versio toimii myös.
	Vanha versio käy kaikki mahdolliset pinnat läpi ja tarkistaa onko ne lukittu. Hidasta. Älä käytä.

	PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
	Lisätty kuormitettu versio. 
	Mahdollistaa bittikartan piirron, joka on peilattu sivusuunnassa ja/tai ylösalaisin.

	PD_VIRHE muutettu 1000 => -1000

	PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
	Nyt kaiken kokoisten kuvien lataus on mahdollista. Ennen pystyi lataamaan vain 640x480 kokoisia.

Versio 1.3
----------

	char *PisteDraw_Virheilmoitus() lisätty. Helpottaa virheen etsintää.

	PisteDraw_Lataa_Kuva(int index, char *filename, bool lataa_paletti)
	Eri kokoisten kuvien lataus on nyt mahdollista (ilman bugeja).

	int PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari)
	Mahdollistaa värien pyörittämisen paletissa.

Versio 1.4
----------

	int		PisteDraw_Font_Uusi(char *polku, char *tiedosto)
	Fontin parametrien lataus tekstitiedostosta.

*/

#ifndef P_DRAW
#define P_DRAW

/* INCLUDES ----------------------------------------------------------------------------------*/
#define DIRECTDRAW_VERSION 0x0700
#include "D:\DXSDK\Include\ddraw.h" // MUISTA KORJATA POLKU OIKEIN!!!!
#include <windows.h> 

/* DEFINES -----------------------------------------------------------------------------------*/

#define		PD_VIRHE		 -1000

#define		PD_TAUSTABUFFER	 0
#define		PD_TAUSTABUFFER2 1

#define		PD_FADE_NOPEA	 5
#define		PD_FADE_NORMAALI 2
#define		PD_FADE_HIDAS	 1


/* PROTOTYPES --------------------------------------------------------------------------------*/

int		PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
						 int leveys, int korkeus, int bpp,
						 int max_colors);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Marginaali_Vasen(int vasen);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Marginaali_Yla(int yla);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Aseta_Videomode(int leveys, int korkeus, int bpp, int max_colors);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
									int vasen, int yla, int oikea, int ala);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
//int		PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);
int		PisteDraw_Buffer_Tayta(int i, const int vasen, const int yla, const int oikea, const int ala, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
bool	PisteDraw_Buffer_Tuhoa(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Fade_Paletti(void);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_In(int laskuri);
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_Out(int laskuri);
//-----------------------------------------------------------------------------------------------------------
bool	PisteDraw_Fade_Paletti_Valmis(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(char *polku, char *tiedosto);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, 
										   int x, int y, int pros);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Lopeta();
//-----------------------------------------------------------------------------------------------------------
void	PisteDraw_Paivita_Naytto();
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Palauta_Pinnat();
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Get(PALETTEENTRY *&paletti);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Set(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(void);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(int i);
//-----------------------------------------------------------------------------------------------------------
int		PisteDraw_Reset_Paletti(void);
//-----------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia();
//-----------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia_Max();
//-----------------------------------------------------------------------------------------------------------
char   *PisteDraw_Virheilmoitus();
//-----------------------------------------------------------------------------------------------------------
#endif

/*

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Alusta(HWND &main_window_handle, HINSTANCE &hinstance_app, 
						 int leveys, int korkeus, int bpp,
						 int max_colors);

Tehtävä:	Alustaa PD:n. 
	
Muuttujat:	main_window_handle:		ikkunan kahva
			hinstance_app:			?
			leveys:					ruudun leveys
			korkeus:				ruudun korkeus
			bpp:					bytes per bixel (ainoastaan arvo 8 toimii)
			max_colors:				max värejä (vain 256 toimii)

Palauttaa:	PD_VIHE, jos epäonnistuu, muuten nolla

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Aseta_Klipperi(int i, int vasen, int yla, int oikea, int ala);

Tehtävä:	Asettaa bufferille rajat, joiden yli ei voi piirtää.
			Jokaisen bufferin alustuksen yhteydessä sille asetetaan klipperi, jonka
			reunat ovat samat kuin bufferin reunat. Tällä voi jälkikäteen muokata
			niitä arvoja.
	
Muuttujat:	i:						halutun bufferin indeksi
			vasen, yla, oikea, ala:	reunat

Palauttaa:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int i);

Tehtävä:	Kopioi koko bufferin sisällön PD_TAUSTABUFFERI-bufferiin.

Muuttujat:	i:						halutun bufferin indeksi

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip(int lahde_index, int kohde_index, int x, int y, bool peilaa_x, bool peilaa_y);

Tehtävä:	Kopioi lähdebufferin sisällön toiseen bufferiin kohtaan x,y. Leikkaa piirrettävän 
			alueen kohdebufferin klipperin mukaan. Kääntää piirrettävän alueen haluattaessa peilikuvaksi
			ja / tai ylösalaisin.

Muuttujat:	lahde_index:			mistä bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y						mihin kohtaan kohdebufferia piirretään (vasen- ja yläreuna)
			peilaa_x				jos true, niin käännetään sivusuunnassa
			peilaa_y				jos true, -"-			  pystysuunnassa 

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y, 
									int vasen, int yla, int oikea, int ala);
									
Tehtävä:	Kopioi lähdebufferin sisällön toiseen bufferiin kohtaan x,y. 
			Leikkaa piirrettävän alueen kohdebufferin klipperin mukaan. 

Muuttujat:	lahde_index:			mistä bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirretään (vasen- ja yläreuna)
			vasen,yla,oikea,ala:	kopioitava alue lähdebufferista.

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Flip_Nopea(int lahde_index, int kohde_index, int x, int y);

Tehtävä:	Kopioi koko lähdebufferin sisällön toiseen bufferiin kohtaan x,y. 
			Leikkaa piirrettävän alueen kohdebufferin klipperin mukaan.

Muuttujat:	lahde_index:			mistä bufferista otetaan
			kohde index:			mihin bufferiin laitetaan
			x,y:					mihin kohtaan kohdebufferia piirretään (vasen- ja yläreuna)

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, UCHAR color);

Tehtävä:	Täyttää halutun bufferin yhdellä värillä.

Muuttujat:	i:						kohdebufferin indeksi
			color:					väri jolla täytetään (0-255)

Palauttaa:	Palauttaa nollan tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Tayta(int i, int vasen, int yla, int oikea, int ala, UCHAR color);

Tehtävä:	Sama kuin edellinen, mutta tässä määritellään tarkka alue, joka täytetään.
			Ottaa myös klipperin huomioon.

Muuttujat:	i:						kohdebufferin indeksi
			vasen,yla...			alue joka täytetään
			color:					väri jolla täytetään (0-255)

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Buffer_Tuhoa(int i);

Tehtävä:	Vapauttaa varatun bufferin.

Muuttujat:	i:						Vapautettavan bufferin indeksi

Palauttaa:	true = onnistui, false = ei onnistunut (bufferia ei ehkä ollut)

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Buffer_Uusi(int leveys, int korkeus, bool video_muisti, UCHAR color);

Tehtävä:	Varaa uuden bufferin, asettaa sille klipperin ja palauttaa bufferin indeksin.

Muuttujat:	leveys, korkeus:		bufferin leveys ja korkeus
			videomuisti:			jos true, bufferi varataan näyttämuistista,	jos ei niin
									työmuistista. Jos videomuisti loppuu, niin bufferit varataan
									automaattisesti työmuistista. Työmuisti on hidasta, videomuisti
									on nopeaa.
			color:					Läpinäkyvyysväri (0-255)

Palauttaa:	Varatun bufferin indeksi tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Fade_Paletti(void);

Tehtävä:	Suorittaa paletin sisään tai ulos feidausta. Feidauksen suunta riippuu
			allaolevista aliohjelmista.

Muuttujat:	-

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_In(int laskuri);

Tehtävä:	Aloittaan paletin feidaamisen mustasta normaaliksi.

Muuttujat:	laskuri:				feidauksen nopeus

Palauttaa:

-------------------------------------------------------------------------------------------------------------
void	PisteDraw_Fade_Paletti_Out(int laskuri);

Tehtävä:	Aloittaan paletin feidaamisen normaalista mustaksi.

Muuttujat:	laskuri:				feidauksen nopeus

Palauttaa:

-------------------------------------------------------------------------------------------------------------
bool	PisteDraw_Fade_Paletti_Valmis(void);

Tehtävä:	Ilmoittaa, onko paletin feidaus kesken vai onko se loppunut.

Muuttujat:	-

Palauttaa:	true jos paletin feidaus on loppunut.

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(int buffer_index, int buffer_x, int buffer_y, int leveys, int korkeus, int lkm);

Tehtävä:	Luo uuden fontin ja palauttaa sen indeksin. Merkkien pitää olla bitmäpissä seuraavassa
			järjestyksessä: 

  a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,t,s,u,v,w,x,y,z,å,ä,ö,0,1,2,3,4,5,6,7,8,9,.,!,?,:,-,pilkku,+,=,(,),/,"
			

Muuttujat:	buffer_index:			bufferi, jossa fontin bitmappi majailee.
			buffer_x:				fontti-bitmapin vasen reuna
			buffer_y:				fontti-bitmapin ylareuna
			leveys:					yhden fontin leveys
			korkeus:				yhden fontin korkeus
			lkm:					fonttien lukumäärä

Palauttaa:	PD_VIRHE tai varatun fontin indeksi

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Uusi(char *polku, char* file);

Tehtävä:	Luo uuden fontin ja palauttaa sen indeksin. Fontin parametrit luetaan tiedostosta: 

Esim:
	*image: 	bigfont.pcx
	*image x:	0
	*image y:	0
	*letters: 	abcdefghijklmnopqrstuvwxyzåäö0123456789.!?
	*letter width:	15
	*letter height:	21			

Muuttujat:	polku:					tiedoston hakemisto. fontin bitmapin pitää olla samassa hakemistossa.
			file:					tiedston nimi
			
Palauttaa:	PD_VIRHE tai varatun fontin indeksi


-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita(int font_index, char *merkkijono, int buffer_index, int x, int y);

Tehtävä:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Font_Kirjoita_Lapinakyva(int font_index, char *merkkijono, int buffer_index, int x, int y, int pros);

Tehtävä:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Lataa_Kuva(int i, char *filename, bool lataa_paletti);

Tehtävä:	Lataa BMP-muotoisen kuvan bufferiin.

Muuttujat:	i:						kohdebufferin indeksi
			filename:				tiedoston polku ja nimi
			lataa_paletti:			true = lataa paletti kuvasta, false = säilytä vanha paletti
			
Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Lopeta();

Tehtävä:	Lopettaa PD:n.

Muuttujat:

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Pyorita(UCHAR eka_vari, UCHAR vika_vari);

Tehtävä:	Pyörittää haluttua osaa paletista.

Muuttujat:	eka_vari:				ensimmäinen väri, jota pyöritetään.
			vika_vari:				viimeinen vari, jota pyöritetään.

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------  
void	PisteDraw_Paivita_Naytto();

Tehtävä:	Kopioi PD_TAUSTABUFFER-puskurin sisällön näyttömuistiin.

Muuttujat:	-

Palauttaa:	-

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Get(PALETTEENTRY *&paletti);

Tehtävä:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Paletti_Set(void);

Tehtävä:

Muuttujat:

Palauttaa:

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Aloita(int i, UCHAR *&back_buffer, DWORD &lPitch);

Tehtävä:	Lukitsee halutun bufferin, jotta siihen voitaisiin piirtää "manuaalisesti".

Muuttujat:	i:						lukittavan bufferin indeksi
			back_buffer:			viittaus taulukkoon, joka sisältää bufferissa olevat pikselit
			lPitch:					bufferin (oikea) leveys pikseleinä

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(void);

Tehtävä:	Vapauttaa kaikki bufferit, jotka on lukittu. Älä käytä! Hidas!

Muuttujat:

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Piirto_Lopeta(int i);

Tehtävä:	Vapauttaa halutun bufferin lukituksen (jos se on lukittu)

Muuttujat:	i:						halutun bufferin indeksi

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
int		PisteDraw_Reset_paletti(void);

Tehtävä:	Palauttaa alkuperäisen kuvasta ladatun paletin.

Muuttujat:	-

Palauttaa:	Nolla tai PD_VIRHE

-------------------------------------------------------------------------------------------------------------
DWORD	PisteDraw_Videomuistia();

Tehtävä:	Palauttaa vapaana olevan videomuistin määrän bitteinä

Muuttujat:

Palauttaa:	Vapaan videomuistin määrä.

-------------------------------------------------------------------------------------------------------------
char   *PisteDraw_Virheilmoitus();

Tehtävä:	Palauttaa (aina) viimeisimmän virheilmoituksen. 

Muuttujat:

Palauttaa:	Vapaan videomuistin määrä.

-------------------------------------------------------------------------------------------------------------

*/