//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef PISTELUTILS
#define PISTELUTILS

#include "platform.hpp"

int PisteUtils_Setcwd();
void  PisteUtils_Lower(char* string);
void  PisteUtils_RemoveSpace(char* string);
bool  PisteUtils_Find(char *filename); // Replaced by File_Exists
int   PisteUtils_CreateDir(char *directory);
void  PisteUtils_Show_Error(const char* txt);

bool PisteUtils_File_Exists(std::string filename);

//type:
// ""  - all files and directories
// "/" - directory
// ".exe" - *.exe
int  PisteUtils_Scandir(const char* type, char* dir, char (*list)[PE_PATH_SIZE], int length);

#endif
