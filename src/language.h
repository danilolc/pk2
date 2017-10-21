#ifndef PK2_LANGUAGE
#define PK2_LANGUAGE

//LANGUAGE AND TEXTS OF THE GAME
PisteLanguage *tekstit;
char langlist[60][_MAX_PATH];
char langmenulist[10][_MAX_PATH];
int langlistindex = 0;
int totallangs = 0;
int txt_setup_options,
	txt_setup_videomodes,
	txt_setup_music_and_sounds,
	txt_setup_music,
	txt_setup_sounds,
	txt_setup_language,
	txt_setup_play,
	txt_setup_exit,

	txt_intro_presents,
	txt_intro_a_game_by,
	txt_intro_original,
	txt_intro_tested_by,
	txt_intro_thanks_to,
	txt_intro_translation,
	txt_intro_translator,

	txt_mainmenu_new_game,
	txt_mainmenu_continue,
	txt_mainmenu_load_game,
	txt_mainmenu_save_game,
	txt_mainmenu_controls,
	txt_mainmenu_graphics,
	txt_mainmenu_sounds,
	txt_mainmenu_exit,

	txt_mainmenu_return,

	txt_loadgame_title,
	txt_loadgame_info,
	txt_loadgame_episode,
	txt_loadgame_level,

	txt_savegame_title,
	txt_savegame_info,
	txt_savegame_episode,
	txt_savegame_level,

	txt_controls_title,
	txt_controls_moveleft,
	txt_controls_moveright,
	txt_controls_jump,
	txt_controls_duck,
	txt_controls_walkslow,
	txt_controls_eggattack,
	txt_controls_doodleattack,
	txt_controls_useitem,
	txt_controls_edit,
	txt_controls_kbdef,
	txt_controls_gp4def,
	txt_controls_gp6def,

	txt_gfx_title,
	txt_gfx_tfx_on,
	txt_gfx_tfx_off,
	txt_gfx_tmenus_on,
	txt_gfx_tmenus_off,
	txt_gfx_items_on,
	txt_gfx_items_off,
	txt_gfx_weather_on,
	txt_gfx_weather_off,
	txt_gfx_bgsprites_on,
	txt_gfx_bgsprites_off,
	txt_gfx_speed_normal,
	txt_gfx_speed_double,

	txt_sound_title,
	txt_sound_sfx_volume,
	txt_sound_music_volume,
	txt_sound_more,
	txt_sound_less,

	txt_playermenu_type_name,
	txt_playermenu_continue,
	txt_playermenu_clear,
	txt_player_default_name,

	txt_episodes_choose_episode,
	txt_episodes_no_maps,
	txt_episodes_get_more,

	txt_map_total_score,
	txt_map_next_level,
	txt_map_episode_best_player,
	txt_map_episode_hiscore,
	txt_map_level_best_player,
	txt_map_level_hiscore,
	txt_map_level_fastest_player,
	txt_map_level_best_time,

	txt_score_screen_title,
	txt_score_screen_level_score,
	txt_score_screen_bonus_score,
	txt_score_screen_time_score,
	txt_score_screen_energy_score,
	txt_score_screen_item_score,
	txt_score_screen_total_score,
	txt_score_screen_new_level_hiscore,
	txt_score_screen_new_level_best_time,
	txt_score_screen_new_episode_hiscore,
	txt_score_screen_continue,

	txt_game_score,
	txt_game_time,
	txt_game_energy,
	txt_game_items,
	txt_game_attack1,
	txt_game_attack2,
	txt_game_keys,
	txt_game_clear,
	txt_game_timebonus,
	txt_game_ko,
	txt_game_timeout,
	txt_game_tryagain,
	txt_game_locksopen,
	txt_game_newdoodle,
	txt_game_newegg,
	txt_game_newitem,
	txt_game_loading,
	txt_game_paused,

	txt_end_congratulations,
	txt_end_chickens_saved,
	txt_end_the_end,

	txt_info01,
	txt_info02,
	txt_info03,
	txt_info04,
	txt_info05,
	txt_info06,
	txt_info07,
	txt_info08,
	txt_info09,
	txt_info10,
	txt_info11,
	txt_info12,
	txt_info13,
	txt_info14,
	txt_info15,
	txt_info16,
	txt_info17,
	txt_info18,
	txt_info19;





    //PK_Load_InfoText
    void PK_Lataa_Lisainfot(){
    	PisteLanguage* temp;
    	char infofile[_MAX_PATH] = "infosign.txt";
    	char otsikko[] = "info00";
    	int indeksi1, indeksi2, i;

    	temp = new PisteLanguage();
    	PK_Lisaa_Episodin_Hakemisto(infofile);

    	if (PisteUtils_CheckFile(infofile)){
    		if (temp->Read_File(infofile)){

    			for (i = 0 ; i<19 ; i++){
    				if(i+1 >= 10) otsikko[4] = '1'; //Make "info" + itos(i)
    				otsikko[5] = '1' + (char)(i%10);

    				indeksi1 = tekstit->Hae_Indeksi(otsikko);
    				indeksi2 = temp->Hae_Indeksi(otsikko);

    				if (indeksi1 != -1 && indeksi2 != -1)
    					tekstit->Korvaa_Teksti(indeksi1,temp->Hae_Teksti(indeksi2));
    			}
    		}
    	}

    	delete (temp);
    }
    //PK_Load_Language
    bool PK_Lataa_Kieli(){
    	char tiedosto[_MAX_PATH];
    	int i;

    	strcpy(tiedosto,"language/");

    	if(totallangs == 0){
    		totallangs = PisteUtils_Scandir(".txt", tiedosto, langlist, 60);
    		for(i=0;i<10;i++)
    			strcpy(langmenulist[i],langlist[i]);
    	}

    	strcat(tiedosto,settings.kieli);

    	if (!tekstit->Read_File(tiedosto)){
    		//PK2_virhe = true;
    		return false;
    	}
    	// Aloitusikkuna
    	txt_setup_options			= tekstit->Hae_Indeksi("setup options");
    	txt_setup_videomodes		= tekstit->Hae_Indeksi("setup video modes");
    	txt_setup_music_and_sounds	= tekstit->Hae_Indeksi("setup music & sounds");
    	txt_setup_music				= tekstit->Hae_Indeksi("setup music");
    	txt_setup_sounds			= tekstit->Hae_Indeksi("setup sounds");
    	txt_setup_language			= tekstit->Hae_Indeksi("setup language");
    	txt_setup_play				= tekstit->Hae_Indeksi("setup play");
    	txt_setup_exit				= tekstit->Hae_Indeksi("setup exit");

    	// Intro
    	txt_intro_presents			= tekstit->Hae_Indeksi("intro presents");
    	txt_intro_a_game_by			= tekstit->Hae_Indeksi("intro a game by");
    	txt_intro_original			= tekstit->Hae_Indeksi("intro original character design");
    	txt_intro_tested_by			= tekstit->Hae_Indeksi("intro tested by");
    	txt_intro_thanks_to			= tekstit->Hae_Indeksi("intro thanks to");
    	txt_intro_translation		= tekstit->Hae_Indeksi("intro translation");
    	txt_intro_translator		= tekstit->Hae_Indeksi("intro translator");

    	// P��valikko
    	txt_mainmenu_new_game		= tekstit->Hae_Indeksi("main menu new game");
    	txt_mainmenu_continue		= tekstit->Hae_Indeksi("main menu continue");
    	txt_mainmenu_load_game		= tekstit->Hae_Indeksi("main menu load game");
    	txt_mainmenu_save_game		= tekstit->Hae_Indeksi("main menu save game");
    	txt_mainmenu_controls		= tekstit->Hae_Indeksi("main menu controls");
    	txt_mainmenu_graphics		= tekstit->Hae_Indeksi("main menu graphics");
    	txt_mainmenu_sounds			= tekstit->Hae_Indeksi("main menu sounds");
    	txt_mainmenu_exit			= tekstit->Hae_Indeksi("main menu exit game");

    	txt_mainmenu_return			= tekstit->Hae_Indeksi("back to main menu");

    	// Lataus
    	txt_loadgame_title			= tekstit->Hae_Indeksi("load menu title");
    	txt_loadgame_info			= tekstit->Hae_Indeksi("load menu info");
    	txt_loadgame_episode		= tekstit->Hae_Indeksi("load menu episode");
    	txt_loadgame_level			= tekstit->Hae_Indeksi("load menu level");

    	// Tallennus
    	txt_savegame_title			= tekstit->Hae_Indeksi("save menu title");
    	txt_savegame_info			= tekstit->Hae_Indeksi("save menu info");
    	txt_savegame_episode		= tekstit->Hae_Indeksi("save menu episode");
    	txt_savegame_level			= tekstit->Hae_Indeksi("save menu level");

    	// Kontrollit
    	txt_controls_title			= tekstit->Hae_Indeksi("controls menu title");
    	txt_controls_moveleft		= tekstit->Hae_Indeksi("controls menu move left");
    	txt_controls_moveright		= tekstit->Hae_Indeksi("controls menu move right");
    	txt_controls_jump			= tekstit->Hae_Indeksi("controls menu jump");
    	txt_controls_duck			= tekstit->Hae_Indeksi("controls menu duck");
    	txt_controls_walkslow		= tekstit->Hae_Indeksi("controls menu walk slow");
    	txt_controls_eggattack		= tekstit->Hae_Indeksi("controls menu egg attack");
    	txt_controls_doodleattack	= tekstit->Hae_Indeksi("controls menu doodle attack");
    	txt_controls_useitem		= tekstit->Hae_Indeksi("controls menu use item");
    	txt_controls_edit			= tekstit->Hae_Indeksi("controls menu edit");
    	txt_controls_kbdef			= tekstit->Hae_Indeksi("controls menu keyboard def");
    	txt_controls_gp4def			= tekstit->Hae_Indeksi("controls menu gamepad4");
    	txt_controls_gp6def			= tekstit->Hae_Indeksi("controls menu gamepad6");

    	txt_gfx_title				= tekstit->Hae_Indeksi("graphics menu title");
    	txt_gfx_tfx_on				= tekstit->Hae_Indeksi("graphics menu transparency fx on");
    	txt_gfx_tfx_off				= tekstit->Hae_Indeksi("graphics menu transparency fx off");
    	txt_gfx_tmenus_on			= tekstit->Hae_Indeksi("graphics menu menus are transparent");
    	txt_gfx_tmenus_off			= tekstit->Hae_Indeksi("graphics menu menus are not transparent");
    	txt_gfx_items_on			= tekstit->Hae_Indeksi("graphics menu item bar is visible");
    	txt_gfx_items_off			= tekstit->Hae_Indeksi("graphics menu item bar is not visible");
    	txt_gfx_weather_on			= tekstit->Hae_Indeksi("graphics menu weather fx on");
    	txt_gfx_weather_off			= tekstit->Hae_Indeksi("graphics menu weather fx off");
    	txt_gfx_bgsprites_on		= tekstit->Hae_Indeksi("graphics menu bg sprites on");
    	txt_gfx_bgsprites_off		= tekstit->Hae_Indeksi("graphics menu bg sprites off");
    	txt_gfx_speed_normal		= tekstit->Hae_Indeksi("graphics menu game speed normal");
    	txt_gfx_speed_double		= tekstit->Hae_Indeksi("graphics menu game speed double");

    	txt_sound_title				= tekstit->Hae_Indeksi("sounds menu title");
    	txt_sound_sfx_volume		= tekstit->Hae_Indeksi("sounds menu sfx volume");
    	txt_sound_music_volume		= tekstit->Hae_Indeksi("sounds menu music volume");
    	txt_sound_more				= tekstit->Hae_Indeksi("sounds menu more");
    	txt_sound_less				= tekstit->Hae_Indeksi("sounds menu less");

    	txt_playermenu_type_name	= tekstit->Hae_Indeksi("player screen type your name");
    	txt_playermenu_continue		= tekstit->Hae_Indeksi("player screen continue");
    	txt_playermenu_clear		= tekstit->Hae_Indeksi("player screen clear");
    	txt_player_default_name		= tekstit->Hae_Indeksi("player default name");

    	txt_episodes_choose_episode	= tekstit->Hae_Indeksi("episode menu choose episode");
    	txt_episodes_no_maps		= tekstit->Hae_Indeksi("episode menu no maps");
    	txt_episodes_get_more		= tekstit->Hae_Indeksi("episode menu get more episodes at");

    	txt_map_total_score			= tekstit->Hae_Indeksi("map screen total score");
    	txt_map_next_level			= tekstit->Hae_Indeksi("map screen next level");
    	txt_map_episode_best_player	= tekstit->Hae_Indeksi("episode best player");
    	txt_map_episode_hiscore		= tekstit->Hae_Indeksi("episode hiscore");
    	txt_map_level_best_player	= tekstit->Hae_Indeksi("level best player");
    	txt_map_level_hiscore		= tekstit->Hae_Indeksi("level hiscore");
    	txt_map_level_fastest_player= tekstit->Hae_Indeksi("level fastest player");
    	txt_map_level_best_time		= tekstit->Hae_Indeksi("level best time");

    	txt_score_screen_title			= tekstit->Hae_Indeksi("score screen title");
    	txt_score_screen_level_score	= tekstit->Hae_Indeksi("score screen level score");
    	txt_score_screen_bonus_score	= tekstit->Hae_Indeksi("score screen bonus score");
    	txt_score_screen_time_score		= tekstit->Hae_Indeksi("score screen time score");
    	txt_score_screen_energy_score	= tekstit->Hae_Indeksi("score screen energy score");
    	txt_score_screen_item_score		= tekstit->Hae_Indeksi("score screen item score");
    	txt_score_screen_total_score	= tekstit->Hae_Indeksi("score screen total score");
    	txt_score_screen_new_level_hiscore	= tekstit->Hae_Indeksi("score screen new level hiscore");
    	txt_score_screen_new_level_best_time= tekstit->Hae_Indeksi("score screen new level best time");
    	txt_score_screen_new_episode_hiscore= tekstit->Hae_Indeksi("score screen new episode hiscore");
    	txt_score_screen_continue		= tekstit->Hae_Indeksi("score screen continue");

    	txt_game_score				= tekstit->Hae_Indeksi("score");
    	txt_game_time				= tekstit->Hae_Indeksi("game time");
    	txt_game_energy				= tekstit->Hae_Indeksi("energy");
    	txt_game_items				= tekstit->Hae_Indeksi("items");
    	txt_game_attack1			= tekstit->Hae_Indeksi("attack 1");
    	txt_game_attack2			= tekstit->Hae_Indeksi("attack 2");
    	txt_game_keys				= tekstit->Hae_Indeksi("keys");
    	txt_game_clear				= tekstit->Hae_Indeksi("level clear");
    	txt_game_timebonus			= tekstit->Hae_Indeksi("time bonus");
    	txt_game_ko					= tekstit->Hae_Indeksi("knocked out");
    	txt_game_timeout			= tekstit->Hae_Indeksi("time out");
    	txt_game_tryagain			= tekstit->Hae_Indeksi("try again");
    	txt_game_locksopen			= tekstit->Hae_Indeksi("locks open");
    	txt_game_newdoodle			= tekstit->Hae_Indeksi("new doodle attack");
    	txt_game_newegg				= tekstit->Hae_Indeksi("new egg attack");
    	txt_game_newitem			= tekstit->Hae_Indeksi("new item");
    	txt_game_loading			= tekstit->Hae_Indeksi("loading");
    	txt_game_paused				= tekstit->Hae_Indeksi("game paused");

    	txt_end_congratulations	= tekstit->Hae_Indeksi("end congratulations");
    	txt_end_chickens_saved	= tekstit->Hae_Indeksi("end chickens saved");
    	txt_end_the_end			= tekstit->Hae_Indeksi("end the end");

    	txt_info01					= tekstit->Hae_Indeksi("info01");
    	txt_info02					= tekstit->Hae_Indeksi("info02");
    	txt_info03					= tekstit->Hae_Indeksi("info03");
    	txt_info04					= tekstit->Hae_Indeksi("info04");
    	txt_info05					= tekstit->Hae_Indeksi("info05");
    	txt_info06					= tekstit->Hae_Indeksi("info06");
    	txt_info07					= tekstit->Hae_Indeksi("info07");
    	txt_info08					= tekstit->Hae_Indeksi("info08");
    	txt_info09					= tekstit->Hae_Indeksi("info09");
    	txt_info10					= tekstit->Hae_Indeksi("info10");
    	txt_info11					= tekstit->Hae_Indeksi("info11");
    	txt_info12					= tekstit->Hae_Indeksi("info12");
    	txt_info13					= tekstit->Hae_Indeksi("info13");
    	txt_info14					= tekstit->Hae_Indeksi("info14");
    	txt_info15					= tekstit->Hae_Indeksi("info15");
    	txt_info16					= tekstit->Hae_Indeksi("info16");
    	txt_info17					= tekstit->Hae_Indeksi("info17");
    	txt_info18					= tekstit->Hae_Indeksi("info18");
    	txt_info19					= tekstit->Hae_Indeksi("info19");

    	return true;
    }





#endif
