
	PisteEngine asennus (for dummies? :)

	-----------------------------------

	Sori jos tämä vaikuttaa vähän palikoille tehdyltä tämä ohje, mutta erheiden välttämiseksi 
	päätin tehdä tästä aika rautalangasta väännetyn. Kaikki fileet pitää olla listattu oikein
	FileViewiin ja kaikki polut pitää säätää oikein. Vedin itse tämän ohjeen kertaalleen läpi
	ja ainakin minulla se toimi. Ainahan on tosin jotakin voinut unohtua...
	Jos, jokin mättää, on todennäköisesti syynä se, että jokin polku koodissa 
	osoittaa väärää hakemistoon. Joku tiedosto voi myös puuttua, jos olen tumpeloinut 
	pakatessa. Ja tätäkin kirjoittaessa olisi pitänyt lukea kokeisiin...

	-------------------
	Eli näin tee:
	-------------------

	00. 	Vedä syvään henkeä (ei pakollinen toimenpide :)
	01.	Käynnistä Visual C++.
	02.	Valitse File - New...
	03.	Valitse Projects - Win32 Application.
	04.	Anna sille nimi (joku kiva).
	05.	Valitse "An empty project". Niksauta Finish-nippulaa.

	06.	Pura PisteEngine-paketin kaikki tiedostot projektisi kansioon
		(esim. Microsoft Visual Studio\MyProjects\peliproggis).
		Huom! sinne pitäisi tulla myös debug-niminen kansio, jossa on
		kaksi kuvaa.

	07. Valitse VC++ vasemmanpuoleisen ikkunan FileView -välilehti.
	08. Avaa kaikki välilehden kansiot.
	09. Klikkaa hiiren oikealla Source Files-kansiota ja valitse "Add files to folder".
	10. Valitse kaikki .cpp tiedostot projektisi kansiosta:
			- Game.cpp
			- PisteDraw.cpp
			- PisteFont.cpp
			- PisteInput.cpp
			- PisteSound.cpp
			- PisteWait.cpp

	11. Tee sama Header Files -kansiolle eli lisää sinne projektikansiostasi tiedostot:
			- PisteDraw.h
			- PisteFont.h
			- PisteInput.h
			- PisteSound.h
			- PisteWait.h

	12. Valitse File - New - Resource Script. Anna sille nimeksi vaikka "Resource".
	13. Klikkaa hiiren oikealla korvalla Resource.rc -kuvaketta ja valitse import.
	14. Valitse GameIcon.ico. Lisää samalla tavalla myös cursor1.cur
		Muista kursoria ladatessasi vaihtaa pääte .cur että tiedosto löytyy...
		Voit toki piirrellä omankin kursorin ja ikonin.

	15. Metsästetään DirectX:n header fileet. Omani löytyvät sellaisesta hakemistosta kuin:
		mssdk\directx7\include\
		Eli etsi tiedostot ddraw.h, dinput.h ja dsound.h ja lisää ne Header Files -kansioon. 
		Lisää myös resource.h -tiedosto omasta projektikansiostasi.

	16. Tarvitaan .lib tiedostoja:
			- ddraw.lib
			- dinput.lib
			- dsound.lib
		Tiedostot löytyvät itselläni hakemistosta mssdk\directx7\lib
		Lisäksi tarvitset tiedoston Winmm.lib (Windows multimedia tai jotakin),
		joka löytyy itseltäni hakemistosta:
		Microsoft Visual Studio\Vc98\Lib\Winmm.lib

	17. Tiedostolistan pitäisi näyttää seuraavalta:
		
		Source Files
			- Game.cpp
			- PisteDraw.cpp
			- PisteFont.cpp
			- PisteInput.cpp
			- PisteSound.cpp
			- PisteWait.cpp
			- Recource.rc
		
		Header Files
			- PisteDraw.h
			- PisteFont.h
			- PisteInput.h
			- PisteSound.h
			- PisteWait.h
			- ddraw.h
			- dinput.h
			- dsound.h
			- resource.h

		Resource Files
			- ddraw.lib
			- dinput.lib
			- dsound.lib
			- Winmm.lib

	18. Korjaa DirectX -header tiedostojen polut oikein PisteDraw.h, PisteInput.h ja 
		PisteSound.h tiedostoihin. Siellä lukee nyt esim. "g:\mssdk\directx7\include\ddraw.h".

	19. Rebuildaa kaikki: Build - Rebuild All.
	20. Varmista, että projektisi Debug-kansiosta löytyy kaksi kuvatiedostoa wormz.bmp ja 
	    wormz2.bmp.
	21. Aja (ohjelma) ja nauti (älä alkoholia): Build -> Execute Peliproggis.exe
	
	22. Korvaa Game.cpp omalla oikealla pelilläsi ;)

	Lue selostukset jotka ovat Game.cpp:ssä. Muuta dokumentaatiota ei vielä ole (eikä ehkä tule).
	PisteSound:sta ei ole käytännössä vielä mitään valmiina ja muihinkin tulee vielä lisäyksiä.
	Se mikä Game.cpp:ssä on tärkeää, on Game_Init, Game_Main(), Game_Main_Piirra() ja Game_Quit().
	Jos mieleesi tulee jotakin fiksua lisättävää, tai löydät bugin, pistä noottia. Meanwhile...
	Happy coding. 

	---------------------------
