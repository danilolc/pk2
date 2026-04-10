//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "language.hpp"

#include "engine/PUtils.hpp"
#include "engine/PLog.hpp"
#include "engine/PFilesystem.hpp"

#include <filesystem>
#include "exceptions.hpp"
#include "gfx/text.hpp"


#include <cstring>
#include <string>
#include <sstream>

std::vector<std::string> langlist;

PLang *tekstit = nullptr;
LANGUAGE PK_txt;

const char* GetDefaultLanguageName() {

	char loc[4];
	PUtils::GetLanguage(loc);

	PLog::Write(PLog::DEBUG, "PK2", "Searching language from code: %s", loc);

	if(strcmp(loc, "en") == 0)
		return "english.txt";

	else if(strcmp(loc, "pl") == 0)
		return "polski.txt";

	else if(strcmp(loc, "de") == 0)
		return "deutsch.txt";

	else if(strcmp(loc, "es") == 0)
		return "espanol.txt";

	else if(strcmp(loc, "pt") == 0)
		return "portugues brasil.txt";

	else if(strcmp(loc, "fi") == 0)
		return "suomi.txt";

	else if(strcmp(loc, "it") == 0)
		return "italiano.txt";
	
	else if(strcmp(loc, "fr") == 0)
		return "francais.txt";

	else if(strcmp(loc, "nl") == 0)
		return "nederlands.txt";


	/*;
	if(strcmp(loc, "gl") == 0)
		return "galego.txt";
	
	//"srpski.txt"
	if(strcmp(loc, "hr") == 0)
		return "hrvatski.txt";
	
	if(strcmp(loc, "hu") == 0)
		return "hungarian.txt";
	
	if(strcmp(loc, "id") == 0)
		return "indonesian.txt";
	
	
	
	if(strcmp(loc, "mk") == 0)
		return "macedonian.txt";
	
	//"slovak.txt"
	if(strcmp(loc, "sk") == 0)
		return "slovenscina.txt";
	
	//"savo.txt"
	//"slangi.txt"
	//"tervola.txt"	
	
	
	if(strcmp(loc, "sv") == 0)
		return "swedish.txt";

	if(strcmp(loc, "tr") == 0)
		return "turkish.txt";*/
	
	return "english.txt";

}

void Load_Language(const std::string& language) {

	namespace fs = std::filesystem;

	std::optional<PFile::Path> path = PFilesystem::FindVanillaAsset(language, PFilesystem::LANGUAGE_DIR);
	if(!path.has_value()){
		throw PExcept::FileNotFoundException(language, PExcept::MISSING_LANG);
	}
	
	PLog::Write(PLog::DEBUG, "PK2", "Loading language from %s", path->c_str());

	if (!tekstit->Read_File(*path)){
		std::ostringstream os;
		os<<"Cannot open the language file: \""<<language;
		throw std::runtime_error(os.str());
	}
	
	Load_Fonts(tekstit);

	// Setup
	PK_txt.setup_options			= tekstit->searchLocalizedText("setup options");
	PK_txt.setup_play				= tekstit->searchLocalizedText("setup play");

	// Intro
	PK_txt.intro_presents			= tekstit->searchLocalizedText("intro presents");
	PK_txt.intro_original			= tekstit->searchLocalizedText("intro original character design");
	PK_txt.intro_tested_by			= tekstit->searchLocalizedText("intro tested by");
	PK_txt.intro_thanks_to			= tekstit->searchLocalizedText("intro thanks to");
	PK_txt.intro_translation		= tekstit->searchLocalizedText("intro translation");
	PK_txt.intro_translator			= tekstit->searchLocalizedText("intro translator");

	PK_txt.intro_game_created_by    = tekstit->searchLocalizedText("intro game created by");
	PK_txt.intro_game_improved_by   = tekstit->searchLocalizedText("intro game improved by");
	PK_txt.intro_community          = tekstit->searchLocalizedText("intro community");

	// Menu
	PK_txt.mainmenu_new_game		= tekstit->searchLocalizedText("main menu new game");
	PK_txt.mainmenu_continue		= tekstit->searchLocalizedText("main menu continue");
	PK_txt.mainmenu_load_game		= tekstit->searchLocalizedText("main menu load game");
	PK_txt.mainmenu_save_game		= tekstit->searchLocalizedText("main menu save game");
	PK_txt.mainmenu_controls		= tekstit->searchLocalizedText("main menu controls");
	PK_txt.mainmenu_graphics		= tekstit->searchLocalizedText("main menu graphics");
	PK_txt.mainmenu_sounds			= tekstit->searchLocalizedText("main menu sounds");
	PK_txt.mainmenu_exit			= tekstit->searchLocalizedText("main menu exit game");

	PK_txt.mainmenu_map  			= tekstit->searchLocalizedText("main menu map");
	PK_txt.mainmenu_links			= tekstit->searchLocalizedText("main menu links");
	PK_txt.mainmenu_back			= tekstit->searchLocalizedText("main menu back");
	PK_txt.mainmenu_more			= tekstit->searchLocalizedText("main menu more");

	PK_txt.mainmenu_return			= tekstit->searchLocalizedText("back to main menu");

	PK_txt.settingsmenu_return      = tekstit->searchLocalizedText("back to settings menu");
	

	// load menu
	PK_txt.loadgame_title			= tekstit->searchLocalizedText("load menu title");
	PK_txt.loadgame_info			= tekstit->searchLocalizedText("load menu info");

	// save menu
	PK_txt.savegame_title			= tekstit->searchLocalizedText("save menu title");
	PK_txt.savegame_info			= tekstit->searchLocalizedText("save menu info");
	PK_txt.savegame_episode			= tekstit->searchLocalizedText("save menu episode");
	PK_txt.savegame_level			= tekstit->searchLocalizedText("save menu level");

	PK_txt.savegame_empty			= tekstit->searchLocalizedText("save menu empty slot");
	PK_txt.savegame_completed   	= tekstit->searchLocalizedText("save menu episode completed");

	// controls
	PK_txt.controls_title			= tekstit->searchLocalizedText("controls menu title");
	PK_txt.controls_moveleft		= tekstit->searchLocalizedText("controls menu move left");
	PK_txt.controls_moveright		= tekstit->searchLocalizedText("controls menu move right");
	PK_txt.controls_jump			= tekstit->searchLocalizedText("controls menu jump");
	PK_txt.controls_duck			= tekstit->searchLocalizedText("controls menu duck");
	PK_txt.controls_walkslow		= tekstit->searchLocalizedText("controls menu walk slow");
	PK_txt.controls_eggattack		= tekstit->searchLocalizedText("controls menu egg attack");
	PK_txt.controls_doodleattack	= tekstit->searchLocalizedText("controls menu doodle attack");
	PK_txt.controls_useitem			= tekstit->searchLocalizedText("controls menu use item");
	PK_txt.controls_edit			= tekstit->searchLocalizedText("controls menu edit");

	PK_txt.controls_get_default     = tekstit->searchLocalizedText("controls menu default");
	PK_txt.controls_use_keyboard    = tekstit->searchLocalizedText("controls menu use keyboard");
	PK_txt.controls_use_controller  = tekstit->searchLocalizedText("controls menu use controller");

	PK_txt.controls_vibration_on    = tekstit->searchLocalizedText("controls menu vibration on");
	PK_txt.controls_vibration_off   = tekstit->searchLocalizedText("controls menu vibration off");

	PK_txt.gfx_title				= tekstit->searchLocalizedText("graphics menu title");


	//

	PK_txt.gfx_gui_on				= tekstit->searchLocalizedText("graphics menu gui on");
	PK_txt.gfx_gui_off				= tekstit->searchLocalizedText("graphics menu gui off");

	PK_txt.gfx_fullscreen_on  	    = tekstit->searchLocalizedText("graphics menu fullscreen on");
	PK_txt.gfx_fullscreen_off       = tekstit->searchLocalizedText("graphics menu fullscreen off");

	PK_txt.gfx_touchscreen_on       = tekstit->searchLocalizedText("graphics menu touchscreen on");
	PK_txt.gfx_touchscreen_off      = tekstit->searchLocalizedText("graphics menu touchscreen off");

	PK_txt.gfx_texture_filtering    = tekstit->searchLocalizedText("graphics menu texture filtering");
	PK_txt.gfx_game_speed           = tekstit->searchLocalizedText("graphics menu game speed");

	//

	PK_txt.gfx_tmenus_on			= tekstit->searchLocalizedText("graphics menu menus are transparent");
	PK_txt.gfx_tmenus_off			= tekstit->searchLocalizedText("graphics menu menus are not transparent");

	PK_txt.gfx_speed_normal			= tekstit->searchLocalizedText("graphics menu game speed normal");
	PK_txt.gfx_speed_double			= tekstit->searchLocalizedText("graphics menu game speed double");

	PK_txt.sound_title				= tekstit->searchLocalizedText("sounds menu title");
	PK_txt.sound_sfx_volume			= tekstit->searchLocalizedText("sounds menu sfx volume");
	PK_txt.sound_music_volume		= tekstit->searchLocalizedText("sounds menu music volume");
	PK_txt.sound_more				= tekstit->searchLocalizedText("sounds menu more");
	PK_txt.sound_less				= tekstit->searchLocalizedText("sounds menu less");

	PK_txt.playermenu_type_name		= tekstit->searchLocalizedText("player screen type your name");
	PK_txt.playermenu_continue		= tekstit->searchLocalizedText("player screen continue");
	PK_txt.playermenu_clear			= tekstit->searchLocalizedText("player screen clear");

	PK_txt.episodes_choose_episode	= tekstit->searchLocalizedText("episode menu choose episode");
	PK_txt.episodes_no_maps			= tekstit->searchLocalizedText("episode menu no maps");

	PK_txt.map_total_score			= tekstit->searchLocalizedText("map screen total score");
	PK_txt.map_next_level			= tekstit->searchLocalizedText("map screen next level");
	PK_txt.map_episode_best_player	= tekstit->searchLocalizedText("episode best player");
	PK_txt.map_episode_hiscore		= tekstit->searchLocalizedText("episode hiscore");
	PK_txt.map_level_best_player	= tekstit->searchLocalizedText("level best player");
	PK_txt.map_level_hiscore		= tekstit->searchLocalizedText("level hiscore");
	PK_txt.map_level_fastest_player = tekstit->searchLocalizedText("level fastest player");
	PK_txt.map_level_best_time		= tekstit->searchLocalizedText("level best time");

	PK_txt.score_screen_title		= tekstit->searchLocalizedText("score screen title");
	PK_txt.score_screen_level_score	= tekstit->searchLocalizedText("score screen level score");
	PK_txt.score_screen_bonus_score	= tekstit->searchLocalizedText("score screen bonus score");
	PK_txt.score_screen_time_score	= tekstit->searchLocalizedText("score screen time score");
	PK_txt.score_screen_energy_score= tekstit->searchLocalizedText("score screen energy score");
	PK_txt.score_screen_item_score	= tekstit->searchLocalizedText("score screen item score");
	PK_txt.score_screen_total_score	= tekstit->searchLocalizedText("score screen total score");
	PK_txt.score_screen_new_level_hiscore	= tekstit->searchLocalizedText("score screen new level hiscore");
	PK_txt.score_screen_new_level_best_time= tekstit->searchLocalizedText("score screen new level best time");
	PK_txt.score_screen_new_episode_hiscore= tekstit->searchLocalizedText("score screen new episode hiscore");
	PK_txt.score_screen_continue		= tekstit->searchLocalizedText("score screen continue");

	PK_txt.game_score				= tekstit->searchLocalizedText("score");
	PK_txt.game_time				= tekstit->searchLocalizedText("game time");
	PK_txt.game_energy				= tekstit->searchLocalizedText("energy");
	PK_txt.game_items				= tekstit->searchLocalizedText("items");
	PK_txt.game_attack1				= tekstit->searchLocalizedText("attack 1");
	PK_txt.game_attack2				= tekstit->searchLocalizedText("attack 2");
	PK_txt.game_keys				= tekstit->searchLocalizedText("keys");
	PK_txt.game_clear				= tekstit->searchLocalizedText("level clear");
	//PK_txt.game_timebonus			= tekstit->searchLocalizedText("time bonus");
	PK_txt.game_ko					= tekstit->searchLocalizedText("knocked out");
	PK_txt.game_timeout				= tekstit->searchLocalizedText("time out");
	PK_txt.game_tryagain			= tekstit->searchLocalizedText("try again");
	PK_txt.game_locksopen			= tekstit->searchLocalizedText("locks open");
	PK_txt.game_newdoodle			= tekstit->searchLocalizedText("new doodle attack");
	PK_txt.game_newegg				= tekstit->searchLocalizedText("new egg attack");
	PK_txt.game_newitem				= tekstit->searchLocalizedText("new item");
	PK_txt.game_loading				= tekstit->searchLocalizedText("loading");
	PK_txt.game_paused				= tekstit->searchLocalizedText("game paused");

	//

	PK_txt.game_invisible			= tekstit->searchLocalizedText("invisibility");
	PK_txt.game_supermode			= tekstit->searchLocalizedText("supermode");

	//

	PK_txt.end_congratulations	= tekstit->searchLocalizedText("end congratulations");
	PK_txt.end_chickens_saved	= tekstit->searchLocalizedText("end chickens saved");
	PK_txt.end_the_end			= tekstit->searchLocalizedText("end the end");

	std::string index;
	for (uint i = 1; i < MAX_INFOS; i++) {

		index = "info"; //info + number
		if (i < 10) index += '0';
		index += std::to_string(i);

		if(i<20){
			PK_txt.infos[i] = tekstit->searchLocalizedText(index.c_str());
		}
		else{
			PK_txt.infos[i] = tekstit->Search_Id(index.c_str());
		}
	}
}