//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//-------------------------
//It can be started with the "dev" argument to start the
//cheats and "test" follown by the episode and level to
//open directely on the level.
//	Exemple:
//	"./pekka-kana-2 dev test rooster\ island\ 2/level13.map"
//	Starts the level13.map on dev mode
//#########################
#include "engine/Piste.hpp"
#include "version.hpp"

#include "screens/screens.hpp"
#include "gfx/text.hpp"
#include "game/game.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "save.hpp"
#include "gui.hpp"
#include "system.hpp"
#include "language.hpp"
#include "settings.hpp"

#include <cstring>
#include <algorithm>

#include <SDL.h>

const char default_config[] = 
"-- Render Method"
"\r\n-- Possible options: sdl software opengl opengles default"
"\r\n---------------"
"\r\n*render_method:        default"
"\r\n"
"\r\n"
"\r\n-- Audio Buffer Size"
"\r\n-- low value = low audio latency; high value = less cpu usage"
"\r\n-- Default is 1024"
"\r\n-- Prefer a power of 2: 512 1024 2048 4096 default"
"\r\n---------------"
"\r\n*audio_buffer_size:    default"
"\r\n"
"\r\n"
"\r\n-- Multi thread audio"
"\r\n-- Change frequency in another thread"
"\r\n-- Default is yes"
"\r\n---------------"
"\r\n*audio_multi_thread:    yes"
"\r\n";

static void read_config() {

	PLang conf = PLang();
	PFile::Path path = PFile::Path(data_path + "config.txt");
	
	bool ok = conf.Read_File(path);
	if (!ok) {
		
		PFile::RW* rw = path.GetRW("w");
		if (rw) {
			rw->write(default_config, sizeof(default_config) - 1);
			rw->close();
		}

		return;
	}

	PLog::Write(PLog::DEBUG, "PK2", "Found config file");

	// TODO - set different render method
	int idx = conf.Search_Id("render_method");
	if (idx != -1) {
		const char* txt = conf.Get_Text(idx);
		PLog::Write(PLog::DEBUG, "PK2", "Render method set to %s", txt);

		if (strcmp(txt, "default") == 0)
			render_method = PRender::RENDERER_DEFAULT;
		else if (strcmp(txt, "sdl") == 0)
			render_method = PRender::RENDERER_SDL;
		else if (strcmp(txt, "software") == 0)
			render_method = PRender::RENDERER_SDL_SOFTWARE;
		else if (strcmp(txt, "opengl") == 0)
			render_method = PRender::RENDERER_OPENGL;
		else if (strcmp(txt, "opengles") == 0)
			render_method = PRender::RENDERER_OPENGLES;

	}

	idx = conf.Search_Id("audio_buffer_size");
	if (idx != -1) {
		const char* txt = conf.Get_Text(idx);
		int val = atoi(txt);

		if (val > 0) {
			audio_buffer_size = val;
			

		}
	}
	PLog::Write(PLog::DEBUG, "PK2", "Audio buffer size set to %i", audio_buffer_size);

	idx = conf.Search_Id("audio_multi_thread");
	if (idx != -1) {
		const char* txt = conf.Get_Text(idx);

		if (strcmp(txt, "default") == 0)
			audio_multi_thread = true;
		else if (strcmp(txt, "yes") == 0)
			audio_multi_thread = true;
		else if (strcmp(txt, "no") == 0)
			audio_multi_thread = false;

		
	}
	PLog::Write(PLog::DEBUG, "PK2", "Audio multi thread is %s", audio_multi_thread? "ON" : "OFF");

}

static void start_test(const char* arg) {
	
	if (arg == NULL) return;

	PFile::Path path(arg);

	episode_entry episode;
	episode.name = path.GetDirectory();
	episode.is_zip = false;
	Episode = new EpisodeClass("test", episode);

	Game = new GameClass(path.GetFileName());

	PLog::Write(PLog::DEBUG, "PK2", "Testing episode '%s' level '%s'", episode.name.c_str(), Game->map_file.c_str());

}

static void quit() {

	Settings_Save();

	PSound::stop_music();

	if (PUtils::Is_Mobile())
		GUI_Exit();

	if (Game)
		delete Game;
	
	if (Episode && !test_level) {
		Save_Record(10); //Save #10 is the backup
		delete Episode;
	}
	
	if(tekstit)
		delete tekstit;

	Piste::terminate();
	PLog::Write(PLog::DEBUG, "PK2", "Terminated");
	PLog::Exit();

}

int main(int argc, char *argv[]) {

	char* test_path = NULL;
	bool path_set = false;
	bool print_logs = true;

	#ifdef _WIN32
		print_logs = false;
	#endif

	// Read args
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "version") == 0) {
			printf(PK2_VERSION_STR "\n");
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
				if (chdir(argv[i]) != 0) {
					printf("Invalid path\n");
					exit(1);
				}
				path_set = true;
				continue;
			}
		}
		else if (strcmp(argv[i], "fps") == 0) {
			show_fps = true;
			continue;
		}
		else if (strcmp(argv[i], "speedrun") == 0) {
			speedrun_mode = true;
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

	PLog::Init(PLog::ALL, PFile::Path(""), print_logs);

	if(!path_set)
		PUtils::Setcwd();

	// Set data_path

	#ifndef __ANDROID__

	#ifdef PK2_PORTABLE

	data_path = "." PE_SEP "data" PE_SEP;
	PUtils::CreateDir(data_path);

	#else

	char* data_path_p = SDL_GetPrefPath(NULL, PK2_NAME);
	if (data_path_p == NULL) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init data path");
		quit();
		return 1;

	}

	data_path = data_path_p;
	SDL_free(data_path_p);

	#endif //PK2_PORTABLE

	// Read redirect file to change data directory
	PFile::Path redirect = PFile::Path(data_path, "redirect.txt");
	if (redirect.Find()) {

		PLog::Write(PLog::DEBUG, "PK2", "Found redirect");
		
		PFile::RW *redirect_rw = redirect.GetRW("r");

		char* buffer;
		int size = redirect_rw->to_buffer((void**) &buffer);
		redirect_rw->close();
		
		if (size > 0) {

			buffer[size - 1] = '\0';
			PLog::Write(PLog::DEBUG, "PK2", "Changing path to %s", buffer);

			data_path = buffer;
			SDL_free(buffer);
		
		}

	}

	#else //__ANDROID__

	const char* ptr = SDL_AndroidGetExternalStoragePath();
	if (ptr) {
		External_Path = ptr;
		SDL_free((void*)ptr);
	} else {
		PLog::Write(PLog::ERR, "PK2", "Couldn't find External Path");
	}

	ptr = SDL_AndroidGetInternalStoragePath();
	if (ptr) {
		Internal_Path = ptr;
		SDL_free((void*)ptr);
	} else {
		PLog::Write(PLog::ERR, "PK2", "Couldn't find Internal Path");
	}

	External_Path += PE_SEP;
	Internal_Path += PE_SEP;

	PLog::Write(PLog::DEBUG, "PK2", "External %s", External_Path.c_str());
	PLog::Write(PLog::DEBUG, "PK2", "Internal %s", Internal_Path.c_str());

	PLog::Write(PLog::DEBUG, "PK2", "Allow %i", SDL_AndroidGetExternalStorageState);


	// Choose between internal or external path on Android
	// Prioritize internal
	if (PUtils::ExternalWriteable()) {

		PLog::Write(PLog::DEBUG, "PK2", "External access allowed");

		PFile::Path settings_f = PFile::Path("settings.ini");
		settings_f.SetPath(Internal_Path);
		PLog::Write(PLog::DEBUG, "PK2", "%s", settings_f.c_str());
		if (!settings_f.Find()) {

			PLog::Write(PLog::DEBUG, "PK2", "Settings not found on internal");

			settings_f.SetPath(External_Path);
			if (settings_f.Find()) {

				PLog::Write(PLog::DEBUG, "PK2", "Settings found on external");
				external_dir = true;
			
			}
			else {

				PLog::Write(PLog::DEBUG, "PK2", "Settings not found on external");
				external_dir = false;

			}
		} else {

			PLog::Write(PLog::DEBUG, "PK2", "Settings found on internal");
			external_dir = false;

		}
	} else {

		PLog::Write(PLog::DEBUG, "PK2", "External access not allowed");
		external_dir = false;

	}

	if (external_dir)
		data_path = External_Path;
	else
		data_path = Internal_Path;

	#endif //__ANDROID__

	//Now data_path is set

	PLog::Init(PLog::ALL, PFile::Path(data_path + "log.txt"), print_logs);

	PLog::Write(PLog::DEBUG, "PK2", "Pekka Kana 2 started!");
	PLog::Write(PLog::DEBUG, "PK2", "Game version: %s", PK2_VERSION_STR);
	PLog::Write(PLog::DEBUG, "PK2", "Number: 0x%x", PK2_VERNUM);

	#ifdef COMMIT_HASH
	PLog::Write(PLog::DEBUG, "PK2", "Cammit hash: " COMMIT_HASH);
	#endif

	PLog::Write(PLog::DEBUG, "PK2", "Data path - %s", data_path.c_str());
	Prepare_DataPath();

	Settings_Open();
	read_config();

	if (Settings.shader_type == SETTINGS_MODE_CRT) {
		screen_width = 640;
		screen_height = 480;
	}

	Piste::init(screen_width, screen_height, PK2_NAME, "gfx" PE_SEP "icon.bmp", render_method, audio_buffer_size, audio_multi_thread);
	if (!Piste::is_ready()) {

		PLog::Write(PLog::FATAL, "PK2", "Failed to init PisteEngine");
		quit();
		return 1;

	}

	Screen_First_Start();

	next_screen = SCREEN_INTRO;
	if (dev_mode)
		next_screen = SCREEN_MENU;
	if (test_level) {
		start_test(test_path);
		next_screen = SCREEN_GAME;
	}

	Piste::loop(Screen_Loop); //The game loop

	if(PK2_error) {

		PLog::Write(PLog::ERR, "PK2", PK2_error_msg);
		PUtils::Show_Error(PK2_error_msg);
		
	}

	quit();
	return 0;
}
