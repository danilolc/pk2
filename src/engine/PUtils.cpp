//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "engine/PUtils.hpp"

#include <SDL.h>

#include <cstring>
#include <string>
#include <sys/stat.h>

namespace PUtils {

bool force_mobile = false;

#ifdef __ANDROID__

int Setcwd() {

	char path[PE_PATH_SIZE];
	strcpy(path,"./");
	return 0; //chdir(path);

}

#else

int Setcwd() {

	char* path = SDL_GetBasePath();
	
	if (path != NULL) {
		
		chdir(path);
		SDL_free(path);
	
	}

	return chdir("../res");

}

#endif

void Lower(char* string) {

	for(; *string != '\0'; string++)
		*string |= ' ';
	
}

void RemoveSpace(char* string) {

	int len = strlen(string);

	while(string[len-2] == ' '){
		string[len-2] = '\0';
		len--;
	}

}

bool NoCaseCompare(const char* a, const char* b) {

	for (int i = 0;; i++) {

		if ((a[i] | ' ') != (b[i] | ' '))
			return false;
		if (a[i] == '\0')
			return true;

	}

}

int Alphabetical_Compare(const char *a, const char *b) {
	
	int a_size = strlen(a);
	int b_size = strlen(b);
	
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

bool Find(char* filename) {

	printf("PUtils - Find %s\n", filename);

	int sz = strlen(filename);

	for (int i = 0; i < sz; i++)
		if (filename[i] == '/')
			filename[i] = '\\';


	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(filename) && GetLastError() == ERROR_FILE_NOT_FOUND) {

		printf("PUtils - %s not found\n", filename);
		return false;

	}

	printf("PUtils - Found on %s \n", filename);
	return true;

}

std::vector<std::string> Scandir(const char* type, const char* dir, int max) {
	
	std::vector<std::string> result;
    struct _finddata_t map_file;

	char buffer[PE_PATH_SIZE];
	if (type[0] == '/' || type[0] == '\\')
		_snprintf(buffer, sizeof(buffer), "%s\\*", dir);
	else if (type[0] == '\0')
		_snprintf(buffer, sizeof(buffer), "%s\\*", dir);
	else
		_snprintf(buffer, sizeof(buffer), "%s\\*%s", dir, type);

	long hFile = _findfirst(buffer, &map_file);
	if (hFile == -1L) {

		return result;

	}
	else {

		if (map_file.name[0] != '.') //No hidden files
			result.push_back(map_file.name);
	
	}

	while (result.size() < max || max == -1 ) {

		if( _findnext( hFile, &map_file ) != 0 ) //TODO - test if works
			break;

		if (map_file.name[0] != '.')
			result.push_back(map_file.name);

	}

	_findclose( hFile );

	return result;

}

int CreateDir(const char *path, const char* dir){
	
	std::string complete_path(path);
	if (dir)
		complete_path += dir;
	
	return CreateDirectory(complete_path.c_str(), NULL);
	
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

std::vector<std::string> Scandir(const char* type, const char* dir, int max) {

	std::vector<std::string> result;
	struct dirent **namelist;

	int numb = scandir(dir, &namelist, 0, alphasort);
	
	if (numb == -1)
		return result;

	if (max != -1)
		if (numb > max) 
			numb = max;

	for( int i = 0; i < numb; i++ ) {

		if( namelist[i]->d_name[0] != '.' ) {

			if(type[0] == '/' && namelist[i]->d_type == DT_DIR) {

				result.push_back(namelist[i]->d_name);
				continue;

			} else if(type[0] == '\0') {
			
				result.push_back(namelist[i]->d_name);
				continue;
			
			} else {

				char* ext = Get_Extension(namelist[i]->d_name);
				if(strcmp(ext, type) == 0) {

					result.push_back(namelist[i]->d_name);
					continue;

				}

			}
			
		}

		free(namelist[i]);

	}
	free(namelist);

	printf("Scanned \"%s\" for \"%s\" and found %i matches\n", dir, type, (int)result.size());
	return result;

}

int CreateDir(const char *path, const char* dir) {

	/*std::string shell("mkdir -p \"");
	
	shell += path;
	shell += dir;
	shell += "\"";
	system(shell.c_str());

	printf("shell - %s\n", shell.c_str());*/

	std::string complete_path(path);
	if (dir)
		complete_path += dir;
	
	return mkdir(complete_path.c_str(), 0700);
	
}

//Scans directory to find file based on case
bool NoCaseFind(char *filename) {

	char dir[PE_PATH_SIZE];
	char file[PE_PATH_SIZE];

	int find = std::string(filename).find_last_of("/");
	strcpy(dir, filename);
	dir[find+1] = '\0';

	strcpy(file, &filename[find+1]);

	std::vector<std::string> list = Scandir("", dir, -1);

	int sz = list.size();
	for(int i = 0; i < sz; i++) {
		
		const char* name = list[i].c_str();
		
		if(NoCaseCompare(name, file)) {

			strcpy(filename, dir);
			strcat(filename, name);
			printf("PUtils - Found on %s \n", filename);

			return true;
		}

	}

	printf("PUtils - %s not found\n", filename);

	return false;

}

bool Find(char *filename) {

	printf("PUtils - Find %s\n", filename);

	struct stat buffer;
	if(stat(filename, &buffer) == 0) {

		printf("PUtils - Found on %s \n", filename);
		return true;

	}

	printf("PUtils - %s not found, trying different cAsE\n", filename);

	return NoCaseFind(filename);
	
}


#endif

void Show_Error(const char* txt) {
	
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", txt, NULL);

}

void Force_Mobile() {

	force_mobile = true;

}

bool Is_Mobile() {

	#ifdef __ANDROID__
		return true;
	#else
		return force_mobile;
	#endif

}

}