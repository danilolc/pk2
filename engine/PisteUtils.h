//#########################
//PisteEngine - PisteUtils
//by Janne Kivilahti from Piste Gamez
//#########################

#ifndef PISTELUTILS
#define PISTELUTILS

void PisteUtils_Lower(char* string);
void PisteUtils_RemoveSpace(char* string);

//type:
// "/" - directory
// ".exe" - *.exe
int  PisteUtils_Scandir(const char* type, char* dir, char (*list)[128], int length);

#endif
