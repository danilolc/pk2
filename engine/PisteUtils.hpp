//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "platform.hpp"

int PisteUtils_Setcwd();
void  PisteUtils_Lower(char* string);
void  PisteUtils_RemoveSpace(char* string);
bool  PisteUtils_Find(char *filename);

int   PisteUtils_CreateDir(const char *directory);
void  PisteUtils_Show_Error(const char* txt);

//type:
// ""  - all files and directories
// "/" - directory
// ".exe" - *.exe
int  PisteUtils_Scandir(const char* type, const char* dir, char (*list)[PE_PATH_SIZE], int length);

void PisteUtils_Force_Mobile();
bool PisteUtils_Is_Mobile();
int PisteUtils_Alphabetical_Compare(const char *a, const char *b);