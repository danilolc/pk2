//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "system.hpp"

#include <cmath>

int screen_width  = 800;
int screen_height = 480;

char tyohakemisto[PE_PATH_SIZE] = ".";

int mouse_x = 10;
int mouse_y = 10;
int key_delay = 0;

double cos_table[360];
double sin_table[360];
int degree = 0;
int degree_temp = 0;

bool test_level = false;
bool dev_mode = false;

bool doublespeed = false;
bool skip_frame = false;

bool PK2_error = false;
const char* PK2_error_msg = nullptr;

int PK2_Error(const char* msg) {
	PK2_error = true;
	PK2_error_msg = msg;
}

void Calculate_SinCos(){
	int i;
	for (i=0; i<360; i++) cos_table[i] = cos(M_PI*2* (i%360)/180)*33;
	for (i=0; i<360; i++) sin_table[i] = sin(M_PI*2* (i%360)/180)*33;
}

//PisteUtils
bool PK_Check_File(char *filename){ //TODO - If isn't Windows - List directory, set lower case, test, and change "char *filename".
	struct stat st;
	bool ret = (stat(filename, &st) == 0);
	if(!ret) printf("PK2    - asked about non-existing file: %s\n", filename);
	return ret;
}