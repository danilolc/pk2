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
#include "episode.hpp"
#include "gui.hpp"
#include "system.hpp"
#include "language.hpp"
#include "settings.hpp"

#include <cstring>

#define GAME_NAME    "Pekka Kana 2"
#define GAME_VERSION "r3"

void Start_Test(const char* arg) {
	
	if (arg == NULL) return;

	char buffer[PE_PATH_SIZE];
	char *episode_path, *map_path;

	strcpy(buffer, arg);

	int i;
	for (i = 0; i < PE_PATH_SIZE; i++)
		if(buffer[i]=='/' || buffer[i] == '\\')
			break;
	buffer[i] = '\0';

	episode_path = buffer;
	Episode = new EpisodeClass("test", episode_path);

	map_path = buffer + i + 1;
	Game = new GameClass(map_path);

	printf("PK2    - testing episode_path '%s' level '%s'\n", episode_path, map_path);

}

void quit(int ret) {

	Settings_Save();

	PSound::stop_music();

	if (Game) {
		delete Game;
		Game = nullptr;
	}
	if (Episode) {
		delete Episode;
		Episode = nullptr;
	}
	
	delete tekstit;

	Piste::terminate();

	if (!ret) printf("Exited correctely\n");
	exit(ret);

}

int main(int argc, char *argv[]) {

	char* test_path = NULL;
	bool path_set = false;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "version") == 0) {
			printf(GAME_VERSION);
			printf("\n");
			exit(0);
		}
		if (strcmp(argv[i], "dev") == 0) {
			dev_mode = true;
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
				printf("PK2    - Path set to %s\n", argv[i]);
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
			PisteUtils_Force_Mobile();
		}
		else {
			printf("Invalid arg\n");
			exit(0);
		}

	}

	printf("PK2 Started!\n");

	if(!path_set)
		PisteUtils_Setcwd();

	Settings_Open();

	screen_width = Settings.isWide ? 800 : 640;

	if (PisteUtils_Is_Mobile())
		screen_width = 800;

	Piste::init(screen_width, screen_height, GAME_NAME, "gfx/icon.bmp");

	if (!Piste::is_ready()) {
		printf("PK2    - Failed to init PisteEngine.\n");
		return 0;
	}

	if (dev_mode) Piste::set_debug(true);

	tekstit = new PLang();

	if (!Load_Language()){
		printf("PK2    - Could not find %s!\n",Settings.kieli);
		strcpy(Settings.kieli, "english.txt");
		if(!Load_Language()){
			printf("PK2    - Could not find the default language file!\n");
			return 0;
		}
	}
	Load_Fonts(tekstit);

	if(PisteUtils_Is_Mobile())
		GUI_Load();

	next_screen = SCREEN_INTRO;
	if (dev_mode)
		next_screen = SCREEN_MENU;
	if (test_level) {
		Start_Test(test_path);
		next_screen = SCREEN_GAME;
	}

	Screen_First_Start();
	
	Piste::loop(Screen_Loop); //The game loop

	if(PK2_error){
		printf("PK2    - Error!\n");
		PisteUtils_Show_Error(PK2_error_msg);
		quit(1);
	}
	
	quit(0);

	return 0;
}
