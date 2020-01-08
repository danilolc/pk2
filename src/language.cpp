//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#include "language.hpp"

#include <cstring>
#include <string>
#include <locale>

std::vector<std::string> langlist;

PLang *tekstit;
LANGUAGE PK_txt;

const char* Language_Name() {

	// it returns just a "c" on some systems
	const char* locale = std::locale("").name().c_str();
	
	char loc[4];

	loc[0] = locale[0] | ' '; // lower
	loc[1] = locale[1] | ' '; 
	loc[2] = '\0';

	printf("Searching language from code: %s\n", loc);

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
	
	//"italiano1.txt"
	//"italiano2.txt"
	//"italiano3.txt"
	//"italiano4.txt"
	//"italiano5.txt"
	if(strcmp(loc, "it") == 0)
		return "italiano6.txt";
	
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
	
	char path[PE_PATH_SIZE];
	strcpy(path,"language" PE_SEP);
	strcat(path, language);

	if (!tekstit->Read_File(path))
		return -1;

	// Load_Fonts(tekstit);

	// Aloitusikkuna
	PK_txt.setup_options			= tekstit->Hae_Indeksi("setup options");
	PK_txt.setup_videomodes			= tekstit->Hae_Indeksi("setup video modes");
	PK_txt.setup_music_and_sounds	= tekstit->Hae_Indeksi("setup music & sounds");
	PK_txt.setup_music				= tekstit->Hae_Indeksi("setup music");
	PK_txt.setup_sounds				= tekstit->Hae_Indeksi("setup sounds");
	PK_txt.setup_language			= tekstit->Hae_Indeksi("setup language");
	PK_txt.setup_play				= tekstit->Hae_Indeksi("setup play");
	PK_txt.setup_exit				= tekstit->Hae_Indeksi("setup exit");

	// Intro
	PK_txt.intro_presents			= tekstit->Hae_Indeksi("intro presents");
	PK_txt.intro_a_game_by			= tekstit->Hae_Indeksi("intro a game by");
	PK_txt.intro_original			= tekstit->Hae_Indeksi("intro original character design");
	PK_txt.intro_tested_by			= tekstit->Hae_Indeksi("intro tested by");
	PK_txt.intro_thanks_to			= tekstit->Hae_Indeksi("intro thanks to");
	PK_txt.intro_translation		= tekstit->Hae_Indeksi("intro translation");
	PK_txt.intro_translator			= tekstit->Hae_Indeksi("intro translator");

	// P��valikko
	PK_txt.mainmenu_new_game		= tekstit->Hae_Indeksi("main menu new game");
	PK_txt.mainmenu_continue		= tekstit->Hae_Indeksi("main menu continue");
	PK_txt.mainmenu_load_game		= tekstit->Hae_Indeksi("main menu load game");
	PK_txt.mainmenu_save_game		= tekstit->Hae_Indeksi("main menu save game");
	PK_txt.mainmenu_controls		= tekstit->Hae_Indeksi("main menu controls");
	PK_txt.mainmenu_graphics		= tekstit->Hae_Indeksi("main menu graphics");
	PK_txt.mainmenu_sounds			= tekstit->Hae_Indeksi("main menu sounds");
	PK_txt.mainmenu_exit			= tekstit->Hae_Indeksi("main menu exit game");

	PK_txt.mainmenu_return			= tekstit->Hae_Indeksi("back to main menu");

	// Lataus
	PK_txt.loadgame_title			= tekstit->Hae_Indeksi("load menu title");
	PK_txt.loadgame_info			= tekstit->Hae_Indeksi("load menu info");
	PK_txt.loadgame_episode			= tekstit->Hae_Indeksi("load menu episode");
	PK_txt.loadgame_level			= tekstit->Hae_Indeksi("load menu level");

	// Tallennus
	PK_txt.savegame_title			= tekstit->Hae_Indeksi("save menu title");
	PK_txt.savegame_info			= tekstit->Hae_Indeksi("save menu info");
	PK_txt.savegame_episode			= tekstit->Hae_Indeksi("save menu episode");
	PK_txt.savegame_level			= tekstit->Hae_Indeksi("save menu level");

	// Kontrollit
	PK_txt.controls_title			= tekstit->Hae_Indeksi("controls menu title");
	PK_txt.controls_moveleft		= tekstit->Hae_Indeksi("controls menu move left");
	PK_txt.controls_moveright		= tekstit->Hae_Indeksi("controls menu move right");
	PK_txt.controls_jump			= tekstit->Hae_Indeksi("controls menu jump");
	PK_txt.controls_duck			= tekstit->Hae_Indeksi("controls menu duck");
	PK_txt.controls_walkslow		= tekstit->Hae_Indeksi("controls menu walk slow");
	PK_txt.controls_eggattack		= tekstit->Hae_Indeksi("controls menu egg attack");
	PK_txt.controls_doodleattack	= tekstit->Hae_Indeksi("controls menu doodle attack");
	PK_txt.controls_useitem			= tekstit->Hae_Indeksi("controls menu use item");
	PK_txt.controls_edit			= tekstit->Hae_Indeksi("controls menu edit");
	PK_txt.controls_kbdef			= tekstit->Hae_Indeksi("controls menu keyboard def");
	PK_txt.controls_gp4def			= tekstit->Hae_Indeksi("controls menu gamepad4");
	PK_txt.controls_gp6def			= tekstit->Hae_Indeksi("controls menu gamepad6");

	PK_txt.gfx_title				= tekstit->Hae_Indeksi("graphics menu title");
	PK_txt.gfx_tfx_on				= tekstit->Hae_Indeksi("graphics menu transparency fx on");
	PK_txt.gfx_tfx_off				= tekstit->Hae_Indeksi("graphics menu transparency fx off");
	PK_txt.gfx_tmenus_on			= tekstit->Hae_Indeksi("graphics menu menus are transparent");
	PK_txt.gfx_tmenus_off			= tekstit->Hae_Indeksi("graphics menu menus are not transparent");
	PK_txt.gfx_items_on				= tekstit->Hae_Indeksi("graphics menu item bar is visible");
	PK_txt.gfx_items_off			= tekstit->Hae_Indeksi("graphics menu item bar is not visible");
	PK_txt.gfx_weather_on			= tekstit->Hae_Indeksi("graphics menu weather fx on");
	PK_txt.gfx_weather_off			= tekstit->Hae_Indeksi("graphics menu weather fx off");
	PK_txt.gfx_bgsprites_on			= tekstit->Hae_Indeksi("graphics menu bg sprites on");
	PK_txt.gfx_bgsprites_off		= tekstit->Hae_Indeksi("graphics menu bg sprites off");
	PK_txt.gfx_speed_normal			= tekstit->Hae_Indeksi("graphics menu game speed normal");
	PK_txt.gfx_speed_double			= tekstit->Hae_Indeksi("graphics menu game speed double");

	PK_txt.sound_title				= tekstit->Hae_Indeksi("sounds menu title");
	PK_txt.sound_sfx_volume			= tekstit->Hae_Indeksi("sounds menu sfx volume");
	PK_txt.sound_music_volume		= tekstit->Hae_Indeksi("sounds menu music volume");
	PK_txt.sound_more				= tekstit->Hae_Indeksi("sounds menu more");
	PK_txt.sound_less				= tekstit->Hae_Indeksi("sounds menu less");

	PK_txt.playermenu_type_name		= tekstit->Hae_Indeksi("player screen type your name");
	PK_txt.playermenu_continue		= tekstit->Hae_Indeksi("player screen continue");
	PK_txt.playermenu_clear			= tekstit->Hae_Indeksi("player screen clear");
	PK_txt.player_default_name		= tekstit->Hae_Indeksi("player default name");

	PK_txt.episodes_choose_episode	= tekstit->Hae_Indeksi("episode menu choose episode");
	PK_txt.episodes_no_maps			= tekstit->Hae_Indeksi("episode menu no maps");
	PK_txt.episodes_get_more		= tekstit->Hae_Indeksi("episode menu get more episodes at");

	PK_txt.map_total_score			= tekstit->Hae_Indeksi("map screen total score");
	PK_txt.map_next_level			= tekstit->Hae_Indeksi("map screen next level");
	PK_txt.map_episode_best_player	= tekstit->Hae_Indeksi("episode best player");
	PK_txt.map_episode_hiscore		= tekstit->Hae_Indeksi("episode hiscore");
	PK_txt.map_level_best_player	= tekstit->Hae_Indeksi("level best player");
	PK_txt.map_level_hiscore		= tekstit->Hae_Indeksi("level hiscore");
	PK_txt.map_level_fastest_player = tekstit->Hae_Indeksi("level fastest player");
	PK_txt.map_level_best_time		= tekstit->Hae_Indeksi("level best time");

	PK_txt.score_screen_title		= tekstit->Hae_Indeksi("score screen title");
	PK_txt.score_screen_level_score	= tekstit->Hae_Indeksi("score screen level score");
	PK_txt.score_screen_bonus_score	= tekstit->Hae_Indeksi("score screen bonus score");
	PK_txt.score_screen_time_score	= tekstit->Hae_Indeksi("score screen time score");
	PK_txt.score_screen_energy_score= tekstit->Hae_Indeksi("score screen energy score");
	PK_txt.score_screen_item_score	= tekstit->Hae_Indeksi("score screen item score");
	PK_txt.score_screen_total_score	= tekstit->Hae_Indeksi("score screen total score");
	PK_txt.score_screen_new_level_hiscore	= tekstit->Hae_Indeksi("score screen new level hiscore");
	PK_txt.score_screen_new_level_best_time= tekstit->Hae_Indeksi("score screen new level best time");
	PK_txt.score_screen_new_episode_hiscore= tekstit->Hae_Indeksi("score screen new episode hiscore");
	PK_txt.score_screen_continue		= tekstit->Hae_Indeksi("score screen continue");

	PK_txt.game_score				= tekstit->Hae_Indeksi("score");
	PK_txt.game_time				= tekstit->Hae_Indeksi("game time");
	PK_txt.game_energy				= tekstit->Hae_Indeksi("energy");
	PK_txt.game_items				= tekstit->Hae_Indeksi("items");
	PK_txt.game_attack1				= tekstit->Hae_Indeksi("attack 1");
	PK_txt.game_attack2				= tekstit->Hae_Indeksi("attack 2");
	PK_txt.game_keys				= tekstit->Hae_Indeksi("keys");
	PK_txt.game_clear				= tekstit->Hae_Indeksi("level clear");
	PK_txt.game_timebonus			= tekstit->Hae_Indeksi("time bonus");
	PK_txt.game_ko					= tekstit->Hae_Indeksi("knocked out");
	PK_txt.game_timeout				= tekstit->Hae_Indeksi("time out");
	PK_txt.game_tryagain			= tekstit->Hae_Indeksi("try again");
	PK_txt.game_locksopen			= tekstit->Hae_Indeksi("locks open");
	PK_txt.game_newdoodle			= tekstit->Hae_Indeksi("new doodle attack");
	PK_txt.game_newegg				= tekstit->Hae_Indeksi("new egg attack");
	PK_txt.game_newitem				= tekstit->Hae_Indeksi("new item");
	PK_txt.game_loading				= tekstit->Hae_Indeksi("loading");
	PK_txt.game_paused				= tekstit->Hae_Indeksi("game paused");

	PK_txt.end_congratulations	= tekstit->Hae_Indeksi("end congratulations");
	PK_txt.end_chickens_saved	= tekstit->Hae_Indeksi("end chickens saved");
	PK_txt.end_the_end			= tekstit->Hae_Indeksi("end the end");

	PK_txt.info01					= tekstit->Hae_Indeksi("info01");
	PK_txt.info02					= tekstit->Hae_Indeksi("info02");
	PK_txt.info03					= tekstit->Hae_Indeksi("info03");
	PK_txt.info04					= tekstit->Hae_Indeksi("info04");
	PK_txt.info05					= tekstit->Hae_Indeksi("info05");
	PK_txt.info06					= tekstit->Hae_Indeksi("info06");
	PK_txt.info07					= tekstit->Hae_Indeksi("info07");
	PK_txt.info08					= tekstit->Hae_Indeksi("info08");
	PK_txt.info09					= tekstit->Hae_Indeksi("info09");
	PK_txt.info10					= tekstit->Hae_Indeksi("info10");
	PK_txt.info11					= tekstit->Hae_Indeksi("info11");
	PK_txt.info12					= tekstit->Hae_Indeksi("info12");
	PK_txt.info13					= tekstit->Hae_Indeksi("info13");
	PK_txt.info14					= tekstit->Hae_Indeksi("info14");
	PK_txt.info15					= tekstit->Hae_Indeksi("info15");
	PK_txt.info16					= tekstit->Hae_Indeksi("info16");
	PK_txt.info17					= tekstit->Hae_Indeksi("info17");
	PK_txt.info18					= tekstit->Hae_Indeksi("info18");
	PK_txt.info19					= tekstit->Hae_Indeksi("info19");

	return 0;
}