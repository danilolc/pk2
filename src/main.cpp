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
#include "engine/PFilesystem.hpp"
#include "version.hpp"

#include "screens/screens_handler.hpp"
#include "gfx/text.hpp"
#include "game/game.hpp"
#include "episode/episodeclass.hpp"
#include "episode/mapstore.hpp"
#include "gfx/touchscreen.hpp"
#include "system.hpp"
#include "language.hpp"

#include "settings/settings.hpp"
#include "settings/config_txt.hpp"

#include <filesystem>
#include <algorithm>

#include "game/prototypes_handler.hpp"

#include <cstring>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <SDL.h>

#include "utils/file_converter.hpp"

#include "episode/save_slots.hpp"


static void start_test(const char* arg) {
	
	if (arg == NULL) return;

	/**
	 * @brief 
	 * TODO
	 * Not to load the whole episode while testing a level
	 */

	std::filesystem::path path(arg);

	//PFile::Path path(arg);
	episode_entry episode;
	episode.name = path.parent_path().string();
	episode.is_zip = false;

	Episode = new EpisodeClass("test", episode);

	Game = new GameClass(path.filename().string());

	PLog::Write(PLog::DEBUG, "PK2", "Testing episode '%s' level '%s'", episode.name.c_str(), Game->level_file.c_str());

}

static void quit() {

	PSound::stop_music();

	if (Game!=nullptr){
		delete Game;
		Game = nullptr;
	}
	
	if (Episode!=nullptr){
		delete Episode;
		Episode = nullptr;
	}
	
	if(tekstit!=nullptr){
		delete tekstit;
		tekstit = nullptr;
	}

	Piste::terminate();
	PLog::Write(PLog::DEBUG, "PK2", "Terminated");
	PLog::Exit();

}


static void log_data() {

	PLog::Write(PLog::DEBUG, "PK2", "Pekka Kana 2 started!");
	PLog::Write(PLog::DEBUG, "PK2", "Game version: %s", PK2_VERSION_STR);
	PLog::Write(PLog::DEBUG, "PK2", "Assets path - %s", PFilesystem::GetAssetsPath().c_str());
	PLog::Write(PLog::DEBUG, "PK2", "Data path - %s", PFilesystem::GetDataPath().c_str());

}

void pk2_init(){
	PFilesystem::SetDefaultPaths();
	PLog::Init(PLog::ALL, true, true);
}

void pk2_main(bool _dev_mode, bool _show_fps, bool _test_level, const std::string& test_path){

	ScreensHandler *handler = nullptr;
	dev_mode = _dev_mode;
	show_fps = _show_fps;
	test_level = _test_level;
	try{
		log_data();

		srand(time(nullptr));
		config_txt.readFile();
		Calculate_SinCos();
		Settings_Open();

		if(!_test_level){
			Search_Episodes();
			if(config_txt.save_slots){
				PK2save::LoadSaveSlots();
			}
		}	

		std::optional<PFile::Path> iconPath = PFilesystem::FindVanillaAsset("icon.bmp", PFilesystem::GFX_DIR, ".png");
		if(!iconPath.has_value()){
			throw std::runtime_error("\"icon[.bmp/.png]\" not found!");			
		}

		Piste::init(screen_width, screen_height, PK2_VERSION_STR_WINDOW, iconPath->c_str(),
		config_txt.audio_buffer_size);

		if(Settings.useJoystick && !PInput::InputSystem::instance().isJoystickAvailable()){
			Settings.useJoystick = false;
		}
		
		if (!Piste::is_ready()) {
			throw std::runtime_error("Failed to init PisteEngine!");
		}	
		handler = new ScreensHandler();
		Screen::next_screen = SCREEN_INTRO;
		if(dev_mode){
			Screen::next_screen = SCREEN_MENU;
		}
		
		if(test_level){
			start_test(test_path.c_str());
			Screen::next_screen = SCREEN_GAME;
		}

		Piste::loop(std::bind(&ScreensHandler::Loop, handler)); //The game loop
	}
	catch(const std::exception& e){
		PLog::Write(PLog::FATAL, "PK2", e.what());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal error!", e.what(), nullptr);
	}

	if(handler!=nullptr){
		delete handler;
		handler = nullptr;
	}

	quit();
}

int main(int argc, char **argv) {

	bool test_level = false;
	bool dev_mode = false;
	bool show_fps = false;
	bool converting_sprite = false;
	bool updating_sprites = false;

	std::string filename_in;
	std::string filename_out;
	std::string test_path;

	int state = 0;
	for(int i=1;i<argc;++i){
		std::string arg = argv[i];
		switch (state)
		{
		case 0:{
			if(arg=="--help" || arg=="-h"){
				printf("Pekka Kana 2 (Pekka the Rooster 2) is a jump 'n run game made "
				"in the spirit of classic platformers such as Super Mario, SuperTux, "
				" Jazz Jackrabbit, Super Frog and so on.\n"
				"Available command arguments are:\n"
				"-h / --help -> print help,\n"
				"-v / --version -> print version string,\n"
				"-d / --dev -> enable the cheats and the debug tools,\n"
				"--fps -> enable the FPS counter.\n"
				"-t / --test \"episode/level\" -> test/play particular level\n"
				"(e.g ./pekka-kana-2 --test \"rooster island 2/level13.map\"),\n"

				"--assets-path -> set a custom assets path (default sprites, rooster islands and so on)\n"
				"(e.g ./pekka-kana-2 --assets-path \"path/my_pk2assets\")\n"

				"--data-path -> set a custom data path (saves, mapstore, etc)\n"
				"(e.g ./pekka-kana-2 --data-path \"path/my_saves\")\n"
				"USE \"./pekka-kana-2 --data-path PREF_PATH\" if you want to use the .local/share dir\n"

				"--convert -> convert an old sprite to the new .spr2 format\n"
				"(e.g ./pekka-kana-2 --convert \"path/old.spr\")\n"
				
				);
				return 0;
			}
			else if(arg=="--version" || arg=="-v"){
				printf("%s\n", PK2_VERSION_STR);
				return 0;
			}
			else if(arg=="--dev" || arg=="-d" || arg=="dev"){
				dev_mode = true;
				//Piste::set_debug(true);
			}
			else if(arg=="--test" || arg=="-t" || arg=="test"){
				state = 1;				
			}
			else if(arg=="--assets-path"){
				state = 2;
			}
			else if(arg=="--data-path"){
				state = 6;
			}

			else if(arg=="--fps"){
				show_fps= true;
			}
			else if	(arg=="--convert"){
				filename_in = "";
				filename_out = "";
				state=3;
				converting_sprite = true;
			}
			else if (arg=="--update-sprites"){
				filename_in = ".";
				filename_out = "";
				state=5;
				updating_sprites = true;
			}
			else {
				printf("Invalid arg \"%s\"\n", arg.c_str());
				return 1;
			}
		}
		break;
		case 1:{
			test_path = arg;
			test_level = true;
			state = 0;
		}
		break;
		case 2:{
			
			try{
				PFilesystem::SetAssetsPath(arg);
			}
			catch(const std::exception& e){
				printf("%s\n", e.what());
				return 1;
			}

			state = 0;
		}
		break;
		case 3:{
			filename_in = arg;
			state = 4;			
		}
		break;
		case 4:{
			filename_out = arg;
			state = 0;
		}
		break;
		case 5:{
			filename_in = arg;
			state = 0;
		}
		break;
		case 6:{
			try{
				if(arg=="PREF_PATH"){
					PFilesystem::SetPrefDataPath();
				}
				else{
					PFilesystem::SetDataPath(arg);
				}
			}
			catch(const std::exception& e){
				printf("%s\n", e.what());
				return 1;
			}
			state=0;
		}
		break;
		default:
			printf("Invalid state: %i\n", state);
			return 1;
		}
	}

	pk2_init();

	if(updating_sprites){
		pk2_updateSprites(filename_in);
	}
	else if(converting_sprite){
		pk2_convertToNewFormat(filename_in, filename_out);
	}
	else{
		pk2_main(dev_mode, show_fps, test_level, test_path);
	}
	return 0;
}