//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################

#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <dirent.h>

#include "PisteUtils.h"

void PisteUtils_Lower(char* string){
	int i;
	for(i=0; string[i]!='\0'; i++)
		tolower(string[i]);
}

void PisteUtils_RemoveSpace(char* string){
	int len = strlen(string);
	while(string[len-2]==' '){
		string[len-2]='\0';
		len--;
	}
}

//This need to receive just the filename, not the path
void getext(char* string){
	int i, b = -1, len;
	for(i=0; string[i]!='\0'; i++)
		if(string[i]=='.') b = i;

	if (b == -1) len = -1;
	else len = i - b;

	for(i=0; i<len; i++)
		string[i] = string[b+i];
	string[i] = '\0';
}

int PisteUtils_Scandir(const char* type, char* dir, char (*list)[128], int length){
	int i, numb = 0, files = 0;
	char ext[128];

	struct dirent **namelist;

	numb = scandir(dir, &namelist, 0, alphasort);

	for(i=0; i<numb; i++){
		strcpy(ext,namelist[i]->d_name);
		getext(ext);
		if(type[0] == '/' && namelist[i]->d_type == DT_DIR && i < length){
			strcpy(list[files], namelist[i]->d_name);
			files++;
		}
		else if(strcmp(ext, type) == 0 && i < length){
			strcpy(list[files], namelist[i]->d_name);
			files++;
		}
		delete namelist[i];
	}
	delete namelist;

	return files;
}
