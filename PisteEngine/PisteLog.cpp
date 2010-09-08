/* INCLUDES ----------------------------------------------------------------------------------*/

#include <windows.h> 
#include <windowsx.h> 
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include "PisteLog.h"

/* VARIABLES ---------------------------------------------------------------------------------*/

bool	kirjoita_pisteloki = false;

/* METHODS ----------------------------------------------------------------------------------*/

void PisteLog_Salli_Kirjoitus() {
		kirjoita_pisteloki = true;	
}

int PisteLog_Kirjoita(char *viesti) {

	if (kirjoita_pisteloki) {
	
		int virhe = 0;

		char *filename = "log.txt";
			
		FILE *tiedosto;

		if ((tiedosto = fopen(filename, "a")) == NULL)
		{
			return(1);
		}

		char mjono[255];

		strcpy(mjono,viesti);

		fwrite(mjono,		sizeof(CHAR),	strlen(mjono),  tiedosto);

		fclose(tiedosto);
	}

	return(0);
}