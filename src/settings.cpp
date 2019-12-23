
#include "settings.hpp"
#include "PisteInput.hpp"

#include <cstring>

#define SETTINGS_PATH "data/settings.ini"

void settings_init(PK2SETTINGS* settings) {
	settings->ladattu = false;

	strcpy(settings->kieli,"english.txt");

	settings->lapinakyvat_objektit = true;
	settings->lapinakyvat_menutekstit = false;
	settings->saa_efektit = true;
	settings->nayta_tavarat = true;
	settings->tausta_spritet = true;

	settings->aanet = true;
	settings->musiikki = true;

	settings->control_left      = PI_LEFT;
	settings->control_right     = PI_RIGHT;
	settings->control_jump      = PI_UP;
	settings->control_down      = PI_DOWN;
	settings->control_walk_slow = PI_RALT;
	settings->control_attack1   = PI_RCONTROL;
	settings->control_attack2   = PI_RSHIFT;
	settings->control_open_gift = PI_SPACE;

	settings->isFiltered = true;
	settings->isFit = true;
	settings->isFullScreen = true;
	settings->isWide = true;

	settings->music_max_volume = 64;
	settings->sfx_max_volume = 90;
}

int settings_open(PK2SETTINGS* settings) {
	ifstream *file = new ifstream(SETTINGS_PATH, ios::binary);

	if (file->fail()){
		delete file;
		settings_init(settings);
		return 1;
	}

	file->read(settings->versio, 4); // Read the version from settings file

	if (strcmp(settings->versio, "1.2") != 0) { // If settings version isn't 1.2
		delete file;
		settings_init(settings);
        settings_save(settings);
		return 2;
	}
	file->read((char*)settings, sizeof(PK2SETTINGS));
	delete file;

	settings->ladattu = true;

	return 0;
}

int settings_save(PK2SETTINGS* settings) {
    //PisteUtils_CreateDir("data");
	
	ofstream *tiedosto = new ofstream(SETTINGS_PATH, ios::binary);
	tiedosto->write ("1.2", 4);
	tiedosto->write ((char*)settings, sizeof(PK2SETTINGS));

	delete (tiedosto);
	return 0;
}