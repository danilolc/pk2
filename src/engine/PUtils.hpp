//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "engine/platform.hpp"

#include <vector>
#include <string>

namespace PUtils {

int Setcwd();
void Lower(char* string);
void RemoveSpace(char* string);
bool Find(char *filename); // TODO - use PFile::Path

int  CreateDir(const char *path, const char* dir);
void Show_Error(const char* txt);

void GetLanguage(char* lang);
//type:
// ""  - all files and directories
// "/" - directory
// ".exe" - *.exe
std::vector<std::string> Scandir(const char* type, const char* dir); // TODO - use PFile::Path

void Force_Mobile();
bool Is_Mobile();
int Alphabetical_Compare(const char *a, const char *b);

}