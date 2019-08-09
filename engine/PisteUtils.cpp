//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################
#include <cstring>
#include <sys/stat.h>
#include <ctype.h>
#include <string>
#ifdef _WIN32
	#include <Windows.h>
	#include <io.h>
	#include "../include/winlite.h"
	#include <direct.h>
#else
	#include <dirent.h>
	#include <unistd.h>
	#include <limits.h>
#endif

#include <iostream>

#include "PisteUtils.hpp"
#include "platform.hpp"

#ifdef USE_LOCAL_SDL
#include "SDL.h"
#else
#include <SDL.h>
#endif
#include <cstring>

#define PE_PATH_SIZE 128

using namespace std;

#ifdef __ANDROID__

int PisteUtils_Setcwd() {
	char path[PE_PATH_SIZE];
	strcpy(path,"/storage/sdcard0/Pekka Kana 2/");
	return chdir(path);
}

#else
int PisteUtils_Setcwd() {
	char exepath[PE_PATH_SIZE];
	int find;

#ifdef _WIN32
	string(exepath, GetModuleFileNameA(NULL, exepath, PE_PATH_SIZE));
#else
	int count = readlink("/proc/self/exe", exepath, PE_PATH_SIZE);
	if (count > 0) exepath[count] = '\0';
#endif

	// TODO Check if this causes a weird problem... @Discord
	find = string(exepath).find_last_of("/\\");
	exepath[find] = '\0';

	chdir(exepath);
	return chdir("../res");
}
#endif

void PisteUtils_Lower(char* string){
	for(int i = 0; string[i]!='\0'; i++)
		string[i] = tolower(string[i]);
}

void PisteUtils_RemoveSpace(char* string){
	int len = strlen(string);
	while(string[len-2]==' '){
		string[len-2]='\0';
		len--;
	}
}

bool PisteUtils_File_Exists(std::string filename) {
	bool ok = true;

	std::ifstream ifs(filename.c_str());

	if (ifs.fail())
		ok = false;

	ifs.close();

	return ok;
}

bool PisteUtils_Find(char *filename){

	//printf("\n\nFinding %s\n",filename);

	char dir[PE_PATH_SIZE];
	char file[PE_PATH_SIZE];

	int find = string(filename).find_last_of("/\\");
	strcpy(dir, filename);
	dir[find+1] = '\0';

	strcpy(file, &filename[find+1]);

	//printf("\nDir %s, File %s\n",dir, file);

	char list[128][PE_PATH_SIZE];
	char list_lower[128][PE_PATH_SIZE];

	int noffiles = PisteUtils_Scandir("", dir, list, 60);

	memcpy(list_lower, list, noffiles*PE_PATH_SIZE);

	for(int i = 0; i < noffiles; i++)
		PisteUtils_Lower(list_lower[i]);

	PisteUtils_Lower(file);

	for(int i = 0; i < noffiles; i++){
		//printf("Is %s, %s?\n",list_lower[i],list[i]);
		if(strcmp(list_lower[i], file) == 0){
			strcpy(file, list[i]);
			strcpy(filename, dir);
			strcat(filename, file);
			//printf("GOOD!!\n");
			return true;
		}
	}
	//printf("Not good.\n");
	return false;
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

#ifdef _WIN32

int PisteUtils_Scandir(const char* type, char* dir, char (*list)[PE_PATH_SIZE], int length){
    struct _finddata_t map_file;
    long hFile;

	int i = 0;
	char buffer[260];
	if (type[0] == '/') //TODO in Windows
		_snprintf(buffer, sizeof(buffer), "%s/*", dir);
	else if (type[0] == '\0')
		_snprintf(buffer, sizeof(buffer), "%s/*", dir);
	else
		_snprintf(buffer, sizeof(buffer), "%s/*%s", dir, type);

	if((hFile = _findfirst(buffer, &map_file )) == -1L )
       return 1;
	else
	{
		strcpy(list[i], map_file.name);
		i++;
	}

	while (i<length && _findnext( hFile, &map_file ) == 0)
	{
		strcpy(list[i], map_file.name);
		i++;
	}

	_findclose( hFile );

	return i;
}
int PisteUtils_CreateDir(char *directory){
	CreateDirectory(directory, NULL);
	return 0;
}

#else

int PisteUtils_Scandir(const char* type, char* dir, char (*list)[PE_PATH_SIZE], int length){
	int i, numb = 0, files = 0;
	char ext[128];

	struct dirent **namelist;

	numb = scandir(dir, &namelist, 0, alphasort);
	if(numb < 1) return -1;

	for(i=0; i<numb; i++){
		strcpy(ext,namelist[i]->d_name);
		getext(ext);
		if(type[0] == '/' && namelist[i]->d_type == DT_DIR && i < length){
			strcpy(list[files], namelist[i]->d_name);
			files++;
		}
		else if(type[0] == '\0'){
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
int PisteUtils_CreateDir(char *directory){
	char shell[PE_PATH_SIZE];
	strcpy(shell,"mkdir -p ");
	strcat(shell,directory);
	system(shell);
	return 0;
}

#endif

void PisteUtils_Show_Error(const char* txt) {
	//const SDL_MessageBoxButtonData buttons[] = {{ 0, 0, "ok" }};

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", txt, NULL);

}
