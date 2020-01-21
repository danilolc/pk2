//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//-------------------------
//It can be started with the "dev" argument to start the
//cheats and "test" follown by the episode and level to
//open directely on the level.
//	Exemple:
//	"./PK2 dev test rooster\ island\ 2/level13.map"
//	Starts the level13.map on dev mode
//#########################
#include "engine/Piste.hpp"

#include "screens/screens.hpp"
#include "gfx/text.hpp"
#include "game/game.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "gui.hpp"
#include "system.hpp"
#include "language.hpp"
#include "settings.hpp"

#include <cstring>

#define GAME_NAME         "Pekka Kana 2"
#define GAME_VERSION      "1.3.1"
#define GAME_VERSION_NAME "(r3-pre)"

#ifdef NO_ZIP
#define GAME_ZIP "(no-zip)"
#else
#define GAME_ZIP "(zip)"
#endif

#ifdef __ANDROID__
#define GAME_PLATFORM "(Android)"
#elif _WIN32
#define GAME_PLATFORM "(Windows)"
#else
#define GAME_PLATFORM "(Linux)"
#endif

#define GAME_VERSION_STR GAME_VERSION " " GAME_VERSION_NAME " " GAME_PLATFORM " " GAME_ZIP

//#define SDL_MAIN_HANDLED
#include <SDL.h>

void start_test(const char* arg) {
	
	if (arg == NULL) return;

	std::string buffer(arg);

	int sep = buffer.find_last_of(PE_SEP);

	episode_entry episode;
	episode.name = buffer.substr(0, sep);
	episode.is_zip = false;
	Episode = new EpisodeClass("test", episode);

	Game = new GameClass(buffer.substr(sep + 1));

	PLog::Write(PLog::DEBUG, "PK2", "Testing episode '%s' level '%s'", episode.name.c_str(), Game->map_file.c_str());

}

void quit() {

	Settings_Save();

	PSound::stop_music();
	GUI_Exit();

	if (Game)
		delete Game;
	
	if (Episode)
		delete Episode;
	
	if(tekstit)
		delete tekstit;

	Piste::terminate();
	PLog::Write(PLog::DEBUG, "PK2", "Pekka Kana 2 terminated");
	PLog::Exit();

}

int main(int argc, char *argv[]) {

	char* test_path = NULL;
	bool path_set = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "version") == 0) {
			printf(GAME_VERSION_STR "\n");
			exit(0);
		}
		if (strcmp(argv[i], "dev") == 0) {
			dev_mode = true;
			Piste::set_debug(true);
		}
		else if (strcmp(argv[i], "test") == 0) {
			if (argc <= i + 1) {
				printf("Please set a level to test\n");
				exit(1);
			}
			else {
				i++;
				test_path = argv[i];
				test_level = true;
				continue;
			}
		}
		else if (strcmp(argv[i], "path") == 0) {
			if (argc <= i + 1) {
				printf("Please set a path\n");
				exit(1);
			}
			else {
				i++;
				chdir(argv[i]);
				path_set = true;
				continue;
			}
		}
		else if (strcmp(argv[i], "fps") == 0) {
			show_fps = true;
			continue;
		}
		else if (strcmp(argv[i], "mobile") == 0) {
			PUtils::Force_Mobile();
		}
		else {
			printf("Invalid arg\n");
			exit(1);
		}

	}

	PLog::Init(PLog::ALL, PFile::Path(""));

	if(!path_set)
		PUtils::Setcwd();

	char* data_path_p = SDL_GetPrefPath(NULL, GAME_NAME);
	if (data_path_p == NULL) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init data path");
		quit();
		return 1;

	}

	data_path = data_path_p; //SDL_AndroidGetExternalStoragePath();
	SDL_free(data_path_p);

	PLog::Init(PLog::ALL, PFile::Path(data_path + "log.txt"));

	PLog::Write(PLog::DEBUG, "PK2", "Pekka Kana 2 started!");
	PLog::Write(PLog::DEBUG, "PK2", "Game version: %s", GAME_VERSION_STR);

	PLog::Write(PLog::DEBUG, "PK2", "Data path - %s", data_path.c_str());
	PUtils::CreateDir(data_path);
	PUtils::CreateDir(data_path + "scores" PE_SEP);
	PUtils::CreateDir(data_path + "mapstore" PE_SEP);

	Settings_Open();

	if (!PUtils::Is_Mobile())
		screen_width = Settings.isWide ? 800 : 640;

	Piste::init(screen_width, screen_height, GAME_NAME, "gfx" PE_SEP "icon.bmp");
	if (!Piste::is_ready()) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init PisteEngine");
		quit();
		return 1;

	}

	next_screen = SCREEN_INTRO;
	if (dev_mode)
		next_screen = SCREEN_MENU;
	if (test_level) {
		start_test(test_path);
		next_screen = SCREEN_GAME;
	}

	Screen_First_Start();

	Piste::loop(Screen_Loop); //The game loop

	if(PK2_error) {

		PLog::Write(PLog::ERR, "PK2", PK2_error_msg);
		PUtils::Show_Error(PK2_error_msg);
		
	}

	quit();
	return 0;
}
