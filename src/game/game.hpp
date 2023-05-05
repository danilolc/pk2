//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"

#include "game/mapclass.hpp"

const int TIME_FPS = 100; //(dec)conds * TIME_FPS = FRAMES
const int INFO_TIME = 700;

class GameClass {

	public:

		u32 level_id = -1;
		MapClass map;
		std::string map_file;

		PK2BLOCK     lasketut_palikat[150];//150
		PK2BLOCKMASK palikkamaskit[BLOCK_MAX_MASKS];

		bool game_over = false;
		bool level_clear = false;
		bool repeating = false;

		bool chick_mode = false;

		u32 exit_timer = 0;

		int  timeout = 0; //timeout in frames
		bool has_time = false;
		u64  frame_count = 0; //flames elapsed

		int palikka_animaatio = 0;

		int button_vibration = 0;
		u32 button1 = 0;
		u32 button2 = 0;
		u32 button3 = 0;

		u32 score = 0;
		u32 score_increment = 0;

		u32 apples_count = 0;
		u32 apples_got = 0;

		int vibration = 0;

		int camera_x;
		int camera_y;
		double dcamera_x;
		double dcamera_y;
		double dcamera_a;
		double dcamera_b;

		bool paused = false;
		bool music_stopped = false;

		int keys = 0;

		int info_timer = 0;
		std::string info_text;

		int item_pannel_x = -215;

		GameClass(int idx);
		GameClass(std::string map_file);
		~GameClass();

		int Start();
		int Finnish();

		int Move_Blocks();
		void Show_Info(const char *text);

		bool isStarted();

		void Place_Sprites();
		void Select_Start();
		int Count_Keys();
		void Change_SkullBlocks();
		void Open_Locks();

	private:
		
		bool started = false;
		
		int Calculete_TileMasks();
		int Clean_TileBuffer();

		int Calculate_Tiles();
		int Open_Map();

};

extern GameClass* Game;
