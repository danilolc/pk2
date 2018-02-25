//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef PISTELUTILS
#define PISTELUTILS

#include "platform.h"

int PisteUtils_Setcwd();
void  PisteUtils_Lower(char* string);
void  PisteUtils_RemoveSpace(char* string);
bool  PisteUtils_Find(char *filename);
int   PisteUtils_CreateDir(char *directory);

//type:
// ""  - all files and directories
// "/" - directory
// ".exe" - *.exe
int  PisteUtils_Scandir(const char* type, char* dir, char (*list)[PE_PATH_SIZE], int length);

#endif
