//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include <array>
#include "engine/types.hpp"
#include "engine/PInputKey.hpp"

#include "sprites_handler.hpp"
#include "levelclass.hpp"
#include "gifts.hpp"

const int TIME_FPS = 100; //(dec)conds * TIME_FPS = FRAMES
const int INFO_TIME = 700;

namespace sol{
	class state;
}

class GameClass {

	public:
		int gfxTexture = -1;


		LevelClass level;
		std::string level_file;	
		
		bool game_over = false;
		bool level_clear = false;
		bool repeating = false;

		u32 exit_timer = 0;

		int  timeout = 0; //timeout in frames
		bool has_time = false;
		u64  frame_count = 0; //flames elapsed

		int tiles_animation_counter = 0;

		int button_vibration = 0;
		u32 button1 = 0;
		u32 button2 = 0;
		u32 button3 = 0;

		int score = 0;
		int score_increment = 0;

		u32 apples_count = 0;
		u32 apples_got = 0;

		

		int camera_x = 0;
		int camera_y = 0;
		double dcamera_x = 0;
		double dcamera_y = 0;
		double dcamera_a = 0;
		double dcamera_b = 0;

		bool paused = false;
		bool music_stopped = false;

		int keys = 0;
		int enemies = 0;

		int info_timer = 0;
		std::string info_text;

		int item_panel_x = -215;


		bool change_skulls=false;
		bool event1 = false;
		bool event2 = false;

		PrototypesHandler spritePrototypes;
		SpriteClass* playerSprite = nullptr;
		SpriteClass* lastCheckpoint = nullptr;
	
		sol::state * lua = nullptr;
		GiftsHandler gifts;


		GameClass(int idx);
		GameClass(std::string level_file);
		~GameClass();

		void update(int& debug_active_sprites);

		void start();
		void finish();

		


		void showInfo(const std::string& text);

		void drawInfoText();

		void placeSprites();
		void selectStart(double& pos_x, double& pos_y, u32& sector);
		SpriteClass* selectTeleporter(SpriteClass* entryTelporter, PrototypeClass* exitPrototype);


		void teleportPlayer(double x, double y, LevelSector*sector);

		void openLocks();

		

		void setCamera(bool legacy_mode=false);
		void updateCamera();

		/*LevelSector* getLevelSector(u32){
			return &this->level.sectorPlaceholder;
		}*/
		bool isStarted()const{
			return this->started;
		}

		void startSupermodeMusic();

		int getLevelID()const{
			return this->level_id;
		}
		
		void saveGameState()const;
		void loadGameState();

		void vibrate(int vibration);
		int getVibration()const{
			return this->vibration;
		}

		void onKeyPressed(const PInput::Key& key);
	private:


		int vibration = 0;
		bool hasEnoughPointsToRespawn();

		void moveBlocks(){
			this->level.moveBlocks(this->button1, this->button2, this->button3);
		}

		void exposePlayerToAIs();

		nlohmann::json toJson()const;
		void fromJson(const nlohmann::json& j);


		PrototypeClass* initialPlayerPrototype = nullptr;
		int level_id = -1;
		bool started = false;
};

extern GameClass* Game;
