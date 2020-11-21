//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "system.hpp"

#include "engine/PLog.hpp"
#include "engine/PUtils.hpp"
#include "engine/PInput.hpp"
#include "engine/PDraw.hpp"
#include "settings.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <string>

int screen_width  = 800;
int screen_height = 480;

#ifdef __ANDROID__

bool external_dir;

bool external_writable = false;
const char* External_Path = nullptr;
const char* Internal_Path = nullptr;

bool save_on_external = false;
bool save_on_internal = false;

char external_save_code[8] = "";
char internal_save_code[8] = "";

#endif

char id_code[8] = "";

std::string data_path;

int game_assets = -1;
int game_assets2 = -1;
int bg_screen = -1;

int key_delay = 0;

double cos_table[360];
double sin_table[360];
int degree = 0;
int degree_temp = 0;

bool test_level = false;
bool dev_mode = false;

bool show_fps = false;

bool PK2_error = false;
const char* PK2_error_msg = nullptr;

int PK2_Error(const char* msg) {
	
	PK2_error = true;
	PK2_error_msg = msg;

	return 0;
}

void Id_To_String(u32 id, char* string) {

	if (!string) return;

	sprintf(string, "_%06x", id & 0xFFFFFF);

}

void Calculate_SinCos(){

	for ( int i = 0; i < 360; i++ ) {
	
		cos_table[i] = cos(M_PI*2*i/180) * 33;
		sin_table[i] = sin(M_PI*2*i/180) * 33;
	
	}

}

int Clicked() {

	if (key_delay) return 0;

	if (PInput::MouseLeft())
		return 1;

	if (PInput::Keydown(PInput::SPACE) || PInput::Keydown(PInput::RETURN))
		return 2;

	if (PInput::Keydown(PInput::JOY_A) || PInput::Keydown(PInput::JOY_START))
		return 3;
	
	return 0;

}

void Draw_Cursor(int x, int y){

	PDraw::image_cutclip(game_assets,x,y,621,461,640,480);
	
}

void Prepare_DataPath() {

	PUtils::CreateDir(data_path);
	PUtils::CreateDir(data_path + "scores" PE_SEP);
	PUtils::CreateDir(data_path + "mapstore" PE_SEP);

}

void Move_DataPath(std::string new_path) {

	new_path += PE_SEP;

	PLog::Write(PLog::DEBUG, "PK2", "Renaming data from %s to %s", data_path.c_str(), new_path.c_str());

	PUtils::RemoveDir(new_path);
	PUtils::RenameDir(data_path, new_path);

	data_path = new_path;

}

//TODO - Receive Episode, organize this
bool FindAsset(PFile::Path* path, const char* default_dir) {

	if (!path->Find()) {

		PLog::Write(PLog::WARN, "PK2", "Can't find %s", path->c_str());

		path->SetPath(default_dir);
		PLog::Write(PLog::INFO, "PK2", "Trying %s", path->c_str());
		
		if (!path->Find()) {
			
			PLog::Write(PLog::WARN, "PK2", "Can't find %s", path->c_str());

			if (path->Is_Zip()) {

				PLog::Write(PLog::INFO, "PK2", "Trying outsize zip");

				std::string filename = path->GetFileName();

				*path = PFile::Path(default_dir);
				path->SetFile(filename);
				if (!path->Find()) {

					PLog::Write(PLog::ERR, "PK2", "Can't find %s", path->c_str());
					return false;

				}

			}
		
		}
		
	}

	return true;

}