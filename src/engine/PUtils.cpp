//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PUtils.hpp"

#include <SDL.h>

#include <cstring>
#include <string>
#include <sys/stat.h>

using namespace std;

bool force_mobile = false;

#ifdef __ANDROID__

int PisteUtils_Setcwd() {

	char path[PE_PATH_SIZE];
	strcpy(path,"./");
	return 0; //chdir(path);

}

#elif _WIN32

int PisteUtils_Setcwd() {

	char exepath[PE_PATH_SIZE];
	int find;

	string(exepath, GetModuleFileName(NULL, exepath, PE_PATH_SIZE));

	find = string(exepath).find_last_of("/\\");
	exepath[find] = '\0';
	chdir(exepath);
	return chdir("../res");

}

#else

int PisteUtils_Setcwd() {

	char exepath[PE_PATH_SIZE];
	int find;

	int count = readlink("/proc/self/exe", exepath, PE_PATH_SIZE);
	if (count > 0) exepath[count] = '\0';

	find = string(exepath).find_last_of("/\\");
	exepath[find] = '\0';
	chdir(exepath);

	return chdir("../res");

}

#endif

void PisteUtils_Lower(char* string) {

	for(; *string != '\0'; string++)
		*string |= ' ';
	
}

void PisteUtils_RemoveSpace(char* string) {

	int len = strlen(string);

	while(string[len-2] == ' '){
		string[len-2] = '\0';
		len--;
	}

}

bool PisteUtils_Compare(const char* a, const char* b) {

	int sa = strlen(a);
	int sb = strlen(b);

	if (sa != sb) return false;

	for (int i = 0; i < sa; i++) {

		if ((a[i] | ' ') != (b[i] | ' '))
			return false;

	}

	return true;

}

int PisteUtils_Alphabetical_Compare(const char *a, const char *b) {
	
	int a_size, b_size;
	
	int min_size = a_size < b_size? a_size : b_size;

	for (int i = 0; i < min_size; i++) {
		char ac = a[i] | ' '; //lower case
		char bc = b[i] | ' ';
		if (ac > bc) 
			return 2;
		if (ac < bc)
			return 1;
	}

	if (a_size > b_size)
		return 1;

	if (a_size < b_size)
		return 2;

	return 0;
}



#ifdef _WIN32

bool PisteUtils_Find(char* filename) {

	printf("PUtils - Find %s\n", filename);

	GetFileAttributes(filename);

	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes("C:\\MyFile.txt") && GetLastError() == ERROR_FILE_NOT_FOUND) {

		printf("PUtils - %s not found\n", filename);
		return false;

	}

	printf("PUtils - Found on %s \n", filename);
	return true;

}

std::vector<string> PisteUtils_Scandir(const char* type, const char* dir, int max) {

	std::vector<string> result;

    struct _finddata_t map_file;
    

	char buffer[PE_PATH_SIZE];
	if (type[0] == '/')
		_snprintf(buffer, sizeof(buffer), "%s/*", dir);
	else if (type[0] == '\0')
		_snprintf(buffer, sizeof(buffer), "%s/*", dir);
	else
		_snprintf(buffer, sizeof(buffer), "%s/*%s", dir, type);

	long hFile = _findfirst(buffer, &map_file);
	if (hFile == -1L) {

		return result;

	}
	else {

		result.push_back(map_file.name);
	
	}

	while (result.size() < max && _findnext( hFile, &map_file ) == 0) {

		result.push_back(map_file.name);

	}

	_findclose( hFile );

	return result;

}

int PisteUtils_CreateDir(const char *directory){
	CreateDirectory(directory, NULL);
	return 0;
}

#else

char* Get_Extension(char* string) {

	int len = strlen(string);
	char* end = string + len;
	
	for( int i = 0; i < len; i++ ) {

		if (*(end - i) == '.' 
			|| *(end - i) == '/'
			|| *(end - i) == '\\') {

			return end - i;

		}

	}

	return string;

}

char* PisteUtils_Scandir(int& count, const char* type, const char* dir, int max) {

	struct dirent **namelist;

	int numb = scandir(dir, &namelist, 0, alphasort);
	count = 0;

	if (max != -1)
		if (numb > max) 
			numb = max;

	for( int i = 0; i < numb; i++ ) {

		if( namelist[i]->d_name[0] != '.' ) {

			if(type[0] == '/' && namelist[i]->d_type == DT_DIR) {

				count++;
				continue;

			} else if(type[0] == '\0') {
			
				count++;
				continue;
			
			} else {

				char* ext = Get_Extension(namelist[i]->d_name);
				if(strcmp(ext, type) == 0) {

					count++;
					continue;

				}

			}
			
		}

		namelist[i]->d_name[0] = '\0';

	}

	if (count == 0)
		return nullptr;

	int j = 0;
	char *list = new char[count * PE_PATH_SIZE];
	if ( !list ) {

		printf("PUtils - Can't alloc files list\n");

		for( int i = 0; i < numb; i++ )
			delete namelist[i];
		
		delete namelist;
		return list;

	}
	
	for( int i = 0; i < numb; i++ ) {

		if(namelist[i]->d_name[0] != '\0') {

			strcpy(&list[j * PE_PATH_SIZE], namelist[i]->d_name);
			j++;

		}

		delete namelist[i];

	}

	delete namelist;
	return list;

}

int PisteUtils_CreateDir(const char *directory) {

	char shell[PE_PATH_SIZE];
	
	strcpy(shell, "mkdir -p ");
	strcat(shell, directory);
	system(shell);
	
	return 0;

}

bool PisteUtils_Find(char *filename) {

	printf("PUtils - Find %s\n", filename);

	char dir[PE_PATH_SIZE];
	char file[PE_PATH_SIZE];

	int find = string(filename).find_last_of("/\\");
	strcpy(dir, filename);
	dir[find+1] = '\0';

	strcpy(file, &filename[find+1]);

	int noffiles;
	char *list = PisteUtils_Scandir(noffiles, "", dir, -1);
	if( !list ) {

		printf("PUtils - No memory to find file");
		return false;

	}

	for(int i = 0; i < noffiles; i++){
		if(PisteUtils_Compare(&list[i * PE_PATH_SIZE], file)){
			strcpy(file, &list[i * PE_PATH_SIZE]);
			strcpy(filename, dir);
			strcat(filename, file);
			printf("PUtils - Found on %s \n", filename);

			delete list;
			return true;
		}
	}

	printf("PUtils - %s not found\n", filename);

	delete list;
	return false;
	
}


#endif

void PisteUtils_Show_Error(const char* txt) {
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", txt, NULL);

}

void PisteUtils_Force_Mobile() {

	force_mobile = true;

}

bool PisteUtils_Is_Mobile() {

	#ifdef __ANDROID__
		return true;
	#else
		return force_mobile;
	#endif

}

