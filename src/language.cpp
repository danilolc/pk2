//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "language.hpp"

#include "engine/PUtils.hpp"
#include "engine/PLog.hpp"

#include <cstring>
#include <string>

std::vector<std::string> langlist;

PLang *tekstit;
LANGUAGE PK_txt;

const char* Language_Name() {

	char loc[4];
	PUtils::GetLanguage(loc);

	PLog::Write(PLog::DEBUG, "PK2", "Searching language from code: %s", loc);

	if(strcmp(loc, "bg") == 0)
		return "bulgarian.txt";
	
	if(strcmp(loc, "ca") == 0)
		return "catala.txt";
	
	//"cesky.txt"
	if(strcmp(loc, "cs") == 0)
		return "czech.txt";

	if(strcmp(loc, "da") == 0)
		return "danish.txt";
	
	//"deutsch.txt"
	//"deutsch2.txt"
	//"deutsch3.txt"
	if(strcmp(loc, "nl") == 0)
		return "deutsch5.txt";
	
	if(strcmp(loc, "en") == 0)
		return "english.txt";
	
	//"espanol castellano.txt"
	//"castellano.txt"
	//"spanish.txt"
	if(strcmp(loc, "es") == 0)
		return "espanol.txt";
	
	//"francais.txt"
	if(strcmp(loc, "fr") == 0)
		return "francais2.txt";
	
	if(strcmp(loc, "gl") == 0)
		return "galego.txt";
	
	//"srpski.txt"
	if(strcmp(loc, "hr") == 0)
		return "hrvatski.txt";
	
	if(strcmp(loc, "hu") == 0)
		return "hungarian.txt";
	
	if(strcmp(loc, "id") == 0)
		return "indonesian.txt";
	
	if(strcmp(loc, "it") == 0)
		return "italiano.txt";
	
	if(strcmp(loc, "mk") == 0)
		return "macedonian.txt";
	
	if(strcmp(loc, "nl") == 0)
		return "nederlands.txt";
	
	if(strcmp(loc, "pl") == 0)
		return "polski.txt";
	
	//"portugues brasil.txt"
	if(strcmp(loc, "pt") == 0)
		return "portugues brasil2.txt";
	
	//"russian.txt"
	if(strcmp(loc, "ru") == 0)
		return "russkii russian.txt";
	
	//"slovak.txt"
	if(strcmp(loc, "sk") == 0)
		return "slovenscina.txt";
	
	//"savo.txt"
	//"slangi.txt"
	//"tervola.txt"	
	if(strcmp(loc, "fi") == 0)
		return "suomi.txt";
	
	if(strcmp(loc, "sv") == 0)
		return "swedish.txt";

	if(strcmp(loc, "tr") == 0)
		return "turkish.txt";
	
	return "english.txt";

}

int Load_Language(const char* language) {
	
	PFile::Path path(std::string("language" PE_SEP) + language);
	
	PLog::Write(PLog::DEBUG, "PK2", "Loading language from %s", path.c_str());

	if (!tekstit->Read_File(path))
		return -1;	
	
	// Load_Fonts(tekstit);

	// Aloitusikkuna
	PK_txt.setup_options			= tekstit->Search_Id("setup options");
	PK_txt.setup_videomodes			= tekstit->Search_Id("setup video modes");
	PK_txt.setup_music_and_sounds	= tekstit->Search_Id("setup music & sounds");
	PK_txt.setup_music				= tekstit->Search_Id("setup music");
	PK_txt.setup_sounds				= tekstit->Search_Id("setup sounds");
	PK_txt.setup_language			= tekstit->Search_Id("setup language");
	PK_txt.setup_play				= tekstit->Search_Id("setup play");
	PK_txt.setup_exit				= tekstit->Search_Id("setup exit");

	// Intro
	PK_txt.intro_presents			= tekstit->Search_Id("intro presents");
	PK_txt.intro_a_game_by			= tekstit->Search_Id("intro a game by");
	PK_txt.intro_original			= tekstit->Search_Id("intro original character design");
	PK_txt.intro_tested_by			= tekstit->Search_Id("intro tested by");
	PK_txt.intro_thanks_to			= tekstit->Search_Id("intro thanks to");
	PK_txt.intro_translation		= tekstit->Search_Id("intro translation");
	PK_txt.intro_translator			= tekstit->Search_Id("intro translator");

	// P��valikko
	PK_txt.mainmenu_new_game		= tekstit->Search_Id("main menu new game");
	PK_txt.mainmenu_continue		= tekstit->Search_Id("main menu continue");
	PK_txt.mainmenu_load_game		= tekstit->Search_Id("main menu load game");
	PK_txt.mainmenu_save_game		= tekstit->Search_Id("main menu save game");
	PK_txt.mainmenu_controls		= tekstit->Search_Id("main menu controls");
	PK_txt.mainmenu_graphics		= tekstit->Search_Id("main menu graphics");
	PK_txt.mainmenu_sounds			= tekstit->Search_Id("main menu sounds");
	PK_txt.mainmenu_exit			= tekstit->Search_Id("main menu exit game");

	PK_txt.mainmenu_return			= tekstit->Search_Id("back to main menu");

	// charging_timer
	PK_txt.loadgame_title			= tekstit->Search_Id("load menu title");
	PK_txt.loadgame_info			= tekstit->Search_Id("load menu info");
	PK_txt.loadgame_episode			= tekstit->Search_Id("load menu episode");
	PK_txt.loadgame_level			= tekstit->Search_Id("load menu level");

	// Tallennus
	PK_txt.savegame_title			= tekstit->Search_Id("save menu title");
	PK_txt.savegame_info			= tekstit->Search_Id("save menu info");
	PK_txt.savegame_episode			= tekstit->Search_Id("save menu episode");
	PK_txt.savegame_level			= tekstit->Search_Id("save menu level");

	// Kontrollit
	PK_txt.controls_title			= tekstit->Search_Id("controls menu title");
	PK_txt.controls_moveleft		= tekstit->Search_Id("controls menu move left");
	PK_txt.controls_moveright		= tekstit->Search_Id("controls menu move right");
	PK_txt.controls_jump			= tekstit->Search_Id("controls menu jump");
	PK_txt.controls_duck			= tekstit->Search_Id("controls menu duck");
	PK_txt.controls_walkslow		= tekstit->Search_Id("controls menu walk slow");
	PK_txt.controls_eggattack		= tekstit->Search_Id("controls menu egg attack");
	PK_txt.controls_doodleattack	= tekstit->Search_Id("controls menu doodle attack");
	PK_txt.controls_useitem			= tekstit->Search_Id("controls menu use item");
	PK_txt.controls_edit			= tekstit->Search_Id("controls menu edit");
	PK_txt.controls_kbdef			= tekstit->Search_Id("controls menu keyboard def");
	PK_txt.controls_gp4def			= tekstit->Search_Id("controls menu gamepad4");
	PK_txt.controls_gp6def			= tekstit->Search_Id("controls menu gamepad6");

	PK_txt.gfx_title				= tekstit->Search_Id("graphics menu title");
	PK_txt.gfx_tfx_on				= tekstit->Search_Id("graphics menu transparency fx on");
	PK_txt.gfx_tfx_off				= tekstit->Search_Id("graphics menu transparency fx off");
	PK_txt.gfx_tmenus_on			= tekstit->Search_Id("graphics menu menus are transparent");
	PK_txt.gfx_tmenus_off			= tekstit->Search_Id("graphics menu menus are not transparent");
	PK_txt.gfx_items_on				= tekstit->Search_Id("graphics menu item bar is visible");
	PK_txt.gfx_items_off			= tekstit->Search_Id("graphics menu item bar is not visible");
	PK_txt.gfx_weather_on			= tekstit->Search_Id("graphics menu weather fx on");
	PK_txt.gfx_weather_off			= tekstit->Search_Id("graphics menu weather fx off");
	PK_txt.gfx_bgsprites_on			= tekstit->Search_Id("graphics menu bg sprites on");
	PK_txt.gfx_bgsprites_off		= tekstit->Search_Id("graphics menu bg sprites off");
	PK_txt.gfx_speed_normal			= tekstit->Search_Id("graphics menu game speed normal");
	PK_txt.gfx_speed_double			= tekstit->Search_Id("graphics menu game speed double");

	PK_txt.sound_title				= tekstit->Search_Id("sounds menu title");
	PK_txt.sound_sfx_volume			= tekstit->Search_Id("sounds menu sfx volume");
	PK_txt.sound_music_volume		= tekstit->Search_Id("sounds menu music volume");
	PK_txt.sound_more				= tekstit->Search_Id("sounds menu more");
	PK_txt.sound_less				= tekstit->Search_Id("sounds menu less");

	PK_txt.playermenu_type_name		= tekstit->Search_Id("player screen type your name");
	PK_txt.playermenu_continue		= tekstit->Search_Id("player screen continue");
	PK_txt.playermenu_clear			= tekstit->Search_Id("player screen clear");
	PK_txt.player_default_name		= tekstit->Search_Id("player default name");

	PK_txt.episodes_choose_episode	= tekstit->Search_Id("episode menu choose episode");
	PK_txt.episodes_no_maps			= tekstit->Search_Id("episode menu no maps");
	PK_txt.episodes_get_more		= tekstit->Search_Id("episode menu get more episodes at");

	PK_txt.map_total_score			= tekstit->Search_Id("map screen total score");
	PK_txt.map_next_level			= tekstit->Search_Id("map screen next level");
	PK_txt.map_episode_best_player	= tekstit->Search_Id("episode best player");
	PK_txt.map_episode_hiscore		= tekstit->Search_Id("episode hiscore");
	PK_txt.map_level_best_player	= tekstit->Search_Id("level best player");
	PK_txt.map_level_hiscore		= tekstit->Search_Id("level hiscore");
	PK_txt.map_level_fastest_player = tekstit->Search_Id("level fastest player");
	PK_txt.map_level_best_time		= tekstit->Search_Id("level best time");

	PK_txt.score_screen_title		= tekstit->Search_Id("score screen title");
	PK_txt.score_screen_level_score	= tekstit->Search_Id("score screen level score");
	PK_txt.score_screen_bonus_score	= tekstit->Search_Id("score screen bonus score");
	PK_txt.score_screen_time_score	= tekstit->Search_Id("score screen time score");
	PK_txt.score_screen_energy_score= tekstit->Search_Id("score screen energy score");
	PK_txt.score_screen_item_score	= tekstit->Search_Id("score screen item score");
	PK_txt.score_screen_total_score	= tekstit->Search_Id("score screen total score");
	PK_txt.score_screen_new_level_hiscore	= tekstit->Search_Id("score screen new level hiscore");
	PK_txt.score_screen_new_level_best_time= tekstit->Search_Id("score screen new level best time");
	PK_txt.score_screen_new_episode_hiscore= tekstit->Search_Id("score screen new episode hiscore");
	PK_txt.score_screen_continue		= tekstit->Search_Id("score screen continue");

	PK_txt.game_score				= tekstit->Search_Id("score");
	PK_txt.game_time				= tekstit->Search_Id("game time");
	PK_txt.game_energy				= tekstit->Search_Id("energy");
	PK_txt.game_items				= tekstit->Search_Id("items");
	PK_txt.game_attack1				= tekstit->Search_Id("attack 1");
	PK_txt.game_attack2				= tekstit->Search_Id("attack 2");
	PK_txt.game_keys				= tekstit->Search_Id("keys");
	PK_txt.game_clear				= tekstit->Search_Id("level clear");
	PK_txt.game_timebonus			= tekstit->Search_Id("time bonus");
	PK_txt.game_ko					= tekstit->Search_Id("knocked out");
	PK_txt.game_timeout				= tekstit->Search_Id("time out");
	PK_txt.game_tryagain			= tekstit->Search_Id("try again");
	PK_txt.game_locksopen			= tekstit->Search_Id("locks open");
	PK_txt.game_newdoodle			= tekstit->Search_Id("new doodle attack");
	PK_txt.game_newegg				= tekstit->Search_Id("new egg attack");
	PK_txt.game_newitem				= tekstit->Search_Id("new item");
	PK_txt.game_loading				= tekstit->Search_Id("loading");
	PK_txt.game_paused				= tekstit->Search_Id("game paused");

	PK_txt.end_congratulations	= tekstit->Search_Id("end congratulations");
	PK_txt.end_chickens_saved	= tekstit->Search_Id("end chickens saved");
	PK_txt.end_the_end			= tekstit->Search_Id("end the end");

	std::string index;
	for (uint i = 1; i < MAX_INFOS; i++) {

		index = "info"; //info + number
		if (i < 10) index += '0';
		index += std::to_string(i);

		PK_txt.infos[i] = tekstit->Search_Id(index.c_str());

	}

	return 0;
}