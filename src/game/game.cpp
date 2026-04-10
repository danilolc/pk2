// #########################
// Pekka Kana 2
// Copyright (c) 2003 Janne Kivilahti
// #########################
#include "game/game.hpp"

#include "game/physics.hpp"
#include "game/gifts.hpp"
#include "system.hpp"
#include "gfx/text.hpp"
#include "gfx/particles.hpp"
#include "gfx/effect.hpp"
#include "gfx/bg_particles.hpp"

#include "episode/episodeclass.hpp"

#include "settings/settings.hpp"
#include "settings/config_txt.hpp"

#include "gfx/touchscreen.hpp"
#include "language.hpp"
#include "exceptions.hpp"

#include "engine/PSound.hpp"
#include "engine/PLog.hpp"
#include "engine/PDraw.hpp"
#include "engine/PInput.hpp"
#include "engine/PFilesystem.hpp"


#include "lua/pk2_lua.hpp"
#include "lua/lua_game_events.hpp"

#include <optional>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>


static const char * CHECKPOINT_DIR_NAME = "checkpoint";

namespace fs = std::filesystem;

GameClass *Game = nullptr;

GameClass::GameClass(int idx) : spritePrototypes(Episode)
{

	this->level_id = idx;
	this->level_file = Episode->getLevelFilename(idx, true);

	if (Episode->getLevelStatus(idx) & LEVEL_PASSED)
	{
		this->repeating = true;
	}
}

GameClass::GameClass(std::string level_file) : spritePrototypes(Episode)
{

	this->repeating = true;

	this->level_file = level_file;

	if (!test_level)
	{
		int level_id_tmp = Episode->findLevelbyFilename(level_file);
		if (level_id_tmp == -1)
		{
			PLog::Write(PLog::FATAL, "PK2", "Couldn't find %s on episode", level_file.c_str());
			throw PExcept::PException("Couldn't find test level on episode");
		}
		this->level_id = level_id_tmp;
	}
	else
	{
		this->level_id = -1;
	}
}

GameClass::~GameClass()
{

	PSound::stop_music();
	PDraw::palette_set(default_palette);
	level.clear();
	this->spritePrototypes.clear();

	if (this->lua != nullptr)
	{
		PK2lua::DestroyGameLuaVM(this->lua);
		this->lua = nullptr;
	}
}

void GameClass::exposePlayerToAIs(){
	if (this->playerSprite != nullptr && this->playerSprite->energy > 0)
	{
		AI_Functions::player_invisible = this->playerSprite;

		if (this->playerSprite->invisible_timer > 0)
		{
			AI_Functions::player = nullptr;
		}
		else
		{
			AI_Functions::player = this->playerSprite;
		}
	}
	else
	{
		AI_Functions::player = nullptr;
		AI_Functions::player_invisible = nullptr;
	}
}


void GameClass::onKeyPressed(const PInput::Key& key){

	const InputSettings& input = Settings.getInput();

	if(this->game_over || this->level_clear){
		if (this->exit_timer > 4)
			this->exit_timer = 4;

	} else {
		if(key==input.open_gift){
			this->gifts.use(this->playerSprite->level_sector->sprites, this->playerSprite);		
		}
		else if(key==input.pauseGame){
			this->paused = !this->paused;
		}
		else if(key==input.commitSuicide && !this->paused){
			if (!config_txt.silent_suicide){
				this->playerSprite->damage_taken = this->playerSprite->energy;
				this->playerSprite->damage_taken_type = DAMAGE_SELF_DESTRUCTION;
				this->playerSprite->self_destruction = true;
			}else{
				this->playerSprite->energy = 0;
				this->playerSprite->removed = true;
			}
		}
		else if(key==input.cycleGifts){
			this->gifts.changeOrder();
		}
	}

	if (dev_mode){

		if(key==PInput::Key(SDL_SCANCODE_F)){
			show_fps = !show_fps;
		}
		else if(key==PInput::Key(SDL_SCANCODE_Z)){
			if (this->button1 < this->level.button1_time - 64)
				this->button1 = this->level.button1_time;
			if (this->button2 < this->level.button2_time - 64)
				this->button2 = this->level.button2_time;
			if (this->button3 < this->level.button3_time - 64)
				this->button3 = this->level.button3_time;
		}
		else if(key==PInput::Key(SDL_SCANCODE_X)){
			if (this->button1 > 64)
				this->button1 = 64;
			if (this->button2 > 64)
				this->button2 = 64;
			if (this->button3 > 64)
				this->button3 = 64;
		}
		else if(key==PInput::Key(SDL_SCANCODE_T)){
			Settings.double_speed = !Settings.double_speed;
		}
		else if(key==PInput::Key(SDL_SCANCODE_L)){
			this->keys = 0;
			this->openLocks();
		}
		else if(key==PInput::Key(SDL_SCANCODE_K)){
			this->change_skulls = true;
		}

		else if(key==PInput::Key(SDL_SCANCODE_R)){
			this->playerSprite->energy = 10;
			this->playerSprite->removed = false;
			this->game_over = false;

			double pos_x = 0;
			double pos_y = 0;

			u32 sector_id = 0;

			this->selectStart(pos_x, pos_y, sector_id);

			if (Episode->legacy_start_offset)
			{
				pos_y -= playerSprite->prototype->height / 2;
			}

			this->teleportPlayer(pos_x, pos_y, this->level.sectors[sector_id]);
		}

		else if(key==PInput::Key(SDL_SCANCODE_END)){
			this->finish();
		}
		else if(key==PInput::Key(SDL_SCANCODE_A)){
			if (this->initialPlayerPrototype != nullptr){
				PrototypeClass *ammo1 = this->playerSprite->ammo1;
				PrototypeClass *ammo2 = this->playerSprite->ammo2;

				this->playerSprite->energy = 10;
				this->playerSprite->transformTo(this->initialPlayerPrototype);
				Effect_Stars(this->playerSprite->x, this->playerSprite->y, COLOR_VIOLET);

				if (this->playerSprite->ammo1 == nullptr)
				{
					this->playerSprite->ammo1 = ammo1;
				}

				if (this->playerSprite->ammo2 == nullptr)
				{
					this->playerSprite->ammo2 = ammo2;
				}
			}
		}

		else if(key==PInput::Key(SDL_SCANCODE_V)){
			if (this->playerSprite->invisible_timer > 1){
				this->playerSprite->invisible_timer = 1;
			} else {
				this->playerSprite->invisible_timer = 3000;
			}
		}

		else if(key==PInput::Key(SDL_SCANCODE_S)){
			if (this->playerSprite->super_mode_timer > 1){
				this->playerSprite->super_mode_timer = 1;
			} else {
				if (Episode->supermode_music) {
					this->startSupermodeMusic();
				}
				this->playerSprite->super_mode_timer = 3000;
			}
		}
	}
}

void GameClass::update(int &debug_active_sprites)
{
	this->exposePlayerToAIs();

	if(dev_mode){

		const InputSettings& input = Settings.getInput();

		if(input.dev_fly.isPressed()){
			this->playerSprite->b = -10;
		}

		if(input.dev_heal.isPressed()){

			PrototypeClass* playerProto = this->playerSprite->prototype;

			int energy = playerProto->energy;
			if(playerProto->hasAI(AI_TRANSFORM_WHEN_ENERGY_OVER_1) && playerProto->transformation!=nullptr){
				energy = playerProto->transformation->energy;
			}


			this->playerSprite->energy = energy;
			this->game_over = false;
			this->exit_timer = 0;
		}
	}

	if(Settings.touchscreen_mode){

		static bool useGiftWasPressed = false;
		if(TouchScreenControls.gift && !useGiftWasPressed){
			this->gifts.use(this->playerSprite->level_sector->sprites, this->playerSprite);				
		}
		useGiftWasPressed = TouchScreenControls.gift;


		static bool cycleGiftsWasPressed = false;
		if(TouchScreenControls.tab && !cycleGiftsWasPressed){
			this->gifts.changeOrder();
		}
		cycleGiftsWasPressed = TouchScreenControls.tab;
	}

	if (!this->level_clear && (!this->has_time || this->timeout > 0))
	{
		this->level.setTilesAnimations(degree, this->tiles_animation_counter / 7, this->button1, this->button2, this->button3);
		this->tiles_animation_counter = 1 + this->tiles_animation_counter % 34;
	}

	this->updateCamera();
	Update_GameSFX();

	/***
	 * Execute events
	 */

	if (this->change_skulls)
	{
		this->change_skulls = false;

		this->vibrate(90);

		for (LevelSector *sector : this->level.sectors)
		{
			sector->changeSkulls(sector == this->playerSprite->level_sector);
			sector->sprites.onSkullBlocksChanged();
		}

		if (this->lua != nullptr)
		{
			PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_SKULL_BLOCKS_CHANGED);
		}
	}

	if (this->event1)
	{
		this->event1 = false;

		this->vibrate(90);

		for (LevelSector *sector : this->level.sectors)
		{
			sector->sprites.onEvent1();
		}

		if (this->lua != nullptr)
		{
			PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_1);
		}
	}

	if (this->event2)
	{
		this->event2 = false;

		for (LevelSector *sector : this->level.sectors)
		{
			sector->sprites.onEvent2();
		}

		if (this->lua != nullptr)
		{
			PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_2);
		}
	}

	if (!this->paused)
	{

		/**
		 * @brief
		 * Kill all the enemies mode
		 */

		if (this->level.game_mode == GAME_MODE_KILL_ALL &&
			this->enemies <= 0 &&
			this->playerSprite->damage_taken == 0 &&
			this->playerSprite->damage_timer == 0 &&
			this->playerSprite->energy > 0)
		{

			this->finish();
		}

		/**
		 * @brief
		 * Update Lua
		 */

		if (this->lua != nullptr)
		{
			PK2lua::UpdateLua();
		}

		/**
		 * @brief
		 * Update particles
		 */

		BG_Particles::Update(this->camera_x, this->camera_y);
		Particles_Update();

		/***
		 * Update sprites
		 */

		if (!this->level_clear && (!this->has_time || this->timeout > 0))
		{
			debug_active_sprites = this->playerSprite->level_sector->sprites.onTickUpdate(this->camera_x, this->camera_y);
			this->frame_count++;
		}

		Fadetext_Update();

		this->moveBlocks();

		degree = (1 + degree) % 360;

		if (this->button1 > 0)
			this->button1--;

		if (this->button2 > 0)
			this->button2--;

		if (this->button3 > 0)
			this->button3--;

		if (this->info_timer > 0)
			this->info_timer--;

		if (this->score_increment > 0)
		{
			this->score++;
			this->score_increment--;
		}

		if (this->has_time && !this->level_clear)
		{
			if (this->timeout > 0)
				this->timeout--;
			else
				this->game_over = true;
		}
	}

	//SpriteClass *Player_Sprite = this->playerSprite;

	if (this->playerSprite->energy < 1)
	{
		this->game_over = true;
	}

	if (this->level_clear || this->game_over) {

		if (this->exit_timer == 0){
			this->exit_timer = 700; // 800;//2000;
		}
		else if (this->exit_timer > 1){
			this->exit_timer--;

			if(this->exit_timer==2) {
				if (this->game_over) {

					if (this->lastCheckpoint != nullptr && this->hasEnoughPointsToRespawn()) {

						Fade_in(FADE_NORMAL);
						this->game_over = false;
						this->exit_timer = 0;

						int t_score = this->score;
						this->loadGameState();
						if(this->score > t_score)this->score = t_score;

						SpriteClass*player = Game->playerSprite;
						if(player->player_c==1){
							player->a = 0;
							player->b = 0;
						}

					} else {
						Fade_out(FADE_NORMAL);
						PSound::set_musicvolume(0);
					}
				} else {
					Fade_out(FADE_NORMAL);
				}
			}
		}
	}

	
}

void GameClass::startSupermodeMusic()
{
	std::optional<PFile::Path> p = PFilesystem::FindAsset("super.xm", PFilesystem::MUSIC_DIR, ".ogg");
	if (p.has_value())
	{
		PSound::start_music(*p);
	}
	else
	{
		PLog::Write(PLog::ERR, "\"super.xm\" not found!");
	}
}

void GameClass::start()
{
	if (this->started)
		return;

	if (this->lua != nullptr)
	{
		PK2lua::DestroyGameLuaVM(this->lua);
		this->lua = nullptr;
	}

	Fadetext_Init(); // Reset fade text
	TouchScreenControls.reset();	
	
	std::optional<PFile::Path> levelPath = PFilesystem::FindEpisodeAsset(level_file, "");
	if (!levelPath.has_value())
	{
		throw PExcept::PException("Cannot find the level file: \"" + level_file + "\"!");
	}
	level.load(*levelPath, false);

	/**
	 * @brief
	 * Load lua
	 */
	if (this->level.lua_script != "")
	{
		this->lua = PK2lua::CreateGameLuaVM(this->level.lua_script);
	}
	else
	{
		PLog::Write(PLog::INFO, "PK2lua", "No Lua scripting in this level");
	}

	this->timeout = level.map_time * TIME_FPS;

	if (timeout > 0)
		this->has_time = true;
	else
		this->has_time = false;

	this->level.button1_time = SWITCH_INITIAL_VALUE;
	this->level.button2_time = SWITCH_INITIAL_VALUE;
	this->level.button3_time = SWITCH_INITIAL_VALUE;
	this->moveBlocks();

	this->placeSprites();

	Particles_Clear();

	LevelSector *sector = this->playerSprite->level_sector;

	BG_Particles::Init(sector->weather, sector->rain_color);
	sector->startMusic();

	//this->moveBlocks();

	PSound::set_musicvolume(Settings.music_max_volume);

	if (this->lua != nullptr)
	{
		PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_GAME_STARTED);
	}

	this->started = true;
}

void GameClass::finish()
{

	if (this->level_clear)
		return;

	this->level_clear = true;

	std::optional<PFile::Path> music_path = PFilesystem::FindAsset("hiscore.xm", PFilesystem::MUSIC_DIR, ".ogg");

	if (!music_path.has_value())
	{
		throw PExcept::PException("\"hiscore.xm\" not found!");
	}

	if (PSound::start_music(*music_path) == -1)
	{
		PLog::Write(PLog::ERR, "Can't play \"hiscore.xm\"");
	}

	if (!test_level)
	{
		u8 status = Episode->getLevelStatus(this->level_id);

		status |= LEVEL_PASSED;
		if (this->apples_count > 0)
		{
			status |= LEVEL_HAS_BIG_APPLES;
			if (this->apples_got >= this->apples_count)
			{
				status |= LEVEL_ALLAPPLES;
			}
		}

		Episode->updateLevelStatus(this->level_id, status);
	}

	PSound::set_musicvolume_now(Settings.music_max_volume);
}

void GameClass::placeSprites()
{
	std::array<PrototypeClass *, 255> mapping;

	// Load prototypes
	std::size_t prototypes_number = this->level.sprite_prototype_names.size();
	if (prototypes_number > mapping.size())
	{
		std::ostringstream os;
		os << "Too many sprite prototypes: " << prototypes_number << std::endl;
		os << mapping.size() << " is the current limit." << std::endl;
		os << "Dependency sprites (ammo, transformation and so on)"
			  " do not count into the limit";

		throw std::runtime_error(os.str());
	}

	for (std::size_t i = 0; i < mapping.size(); ++i)
	{
		mapping[i] = nullptr;
	}

	for (std::size_t i = 0; i < prototypes_number; ++i)
	{
		const std::string &name = level.sprite_prototype_names[i];
		if (!name.empty())
		{
			mapping[i] = this->spritePrototypes.loadPrototype(level.sprite_prototype_names[i]);
		}
	}

	// Load sprite assets
	this->spritePrototypes.loadSpriteAssets();

	// Player prototype
	int player_index = level.player_sprite_index;
	if (player_index < 0 || player_index >= int(mapping.size()))
	{
		std::ostringstream os;
		os << "Incorrect player sprite index: " << player_index;
		throw PExcept::PException(os.str());
	}

	PrototypeClass *player_prototype = mapping[player_index];
	if (player_prototype == nullptr)
	{
		throw PExcept::PException("Null player prototype is quite serious error!");
	}

	this->initialPlayerPrototype = player_prototype;

	// Add player
	{
		double pos_x = 0;
		double pos_y = 0;
		u32 sector_id = 0;

		this->selectStart(pos_x, pos_y, sector_id);
		this->playerSprite = this->level.sectors[sector_id]->sprites.addPlayer(player_prototype, pos_x, pos_y);

		if (Episode->legacy_start_offset)
		{
			this->playerSprite->y -= player_prototype->height / 2;
		}

		this->level.sectors[sector_id]->background->setPalette();
		this->setCamera(Episode->legacy_camera_offset);
	}

	// set GFX
	this->gfxTexture = this->playerSprite->level_sector->gfxTexture;

	// Add other sprites
	for (LevelSector *sector : this->level.sectors)
	{

		for (u32 x = 0; x < sector->getWidth(); x++)
		{
			for (u32 y = 0; y < sector->getHeight(); y++)
			{

				int sprite = sector->sprite_tiles[x + y * sector->getWidth()];
				if (sprite < 0 || sprite >= 255)
					continue;

				PrototypeClass *prototype = mapping[sprite];
				if (prototype == nullptr)
					continue;

				/**
				 * @brief
				 * Count big apples
				 */
				if (prototype->big_apple)
				{
					this->apples_count++;
				}

				/**
				 * @brief
				 * Count big apples in boxes
				 */
				if (prototype->bonus != nullptr && prototype->bonus->big_apple && prototype->bonus_always)
				{
					this->apples_count += prototype->bonuses_number;
				}

				/**
				 * @brief
				 * Count keys
				 */
				if (prototype->can_open_locks &&
					!prototype->indestructible)
				{
					this->keys++;
				}

				/**
				 * @brief
				 * Count enemies
				 */
				if (prototype->type == TYPE_GAME_CHARACTER && !prototype->indestructible && prototype->damage > 0 // to ignore switches, boxes and so on
					&& prototype->enemy)
				{
					this->enemies++;
				}

				sector->sprites.addLevelSprite(prototype, x * 32, y * 32 - prototype->height + 32);
			}
		}

		sector->sprites.sortBg();
	}
}

void GameClass::selectStart(double &pos_x, double &pos_y, u32 &sector)
{

	pos_x = 320;
	pos_y = 196;

	std::vector<BlockPosition> startSigns;

	for (u32 i = 0; i < level.sectors.size(); ++i)
	{
		level.sectors[i]->countStartSigns(startSigns, i);
	}

	int selected_start = 0;
	if (startSigns.size() > 1)
	{
		selected_start = rand() % startSigns.size();
	}

	if (startSigns.size() > 0)
	{
		pos_x = startSigns[selected_start].x * 32 + 17;
		pos_y = startSigns[selected_start].y * 32;
		sector = startSigns[selected_start].sector;
	}
}

SpriteClass *GameClass::selectTeleporter(SpriteClass *entryTelporter, PrototypeClass *exitPrototype)
{
	std::vector<SpriteClass *> teleporters;
	for (LevelSector *sector : this->level.sectors)
	{
		for (SpriteClass *sprite : sector->sprites.Sprites_List)
		{
			if (sprite->prototype == exitPrototype && sprite != entryTelporter)
			{
				teleporters.push_back(sprite);
			}
		}
	}

	if (teleporters.size() == 0)
		return nullptr;
	else if (teleporters.size() == 1)
		return teleporters[0];
	else
		return teleporters[rand() % teleporters.size()];
}

void GameClass::teleportPlayer(double x, double y, LevelSector *sector)
{
	this->playerSprite->x = x;
	this->playerSprite->y = y;

	/**
	 * @brief
	 * Change sector
	 */
	if (sector != nullptr && this->playerSprite->level_sector != sector)
	{

		LevelSector *previous_sector = this->playerSprite->level_sector;
		previous_sector->sprites.Sprites_List.remove(this->playerSprite);

		sector->sprites.Sprites_List.push_front(this->playerSprite);
		this->playerSprite->level_sector = sector;

		// Change palette
		sector->background->setPalette();

		// Change weather
		BG_Particles::Init(sector->weather, sector->rain_color);
		Particles_Clear();

		// Change music
		if (sector->music_name != previous_sector->music_name)
		{
			sector->startMusic();
		}

		// Change GFX texture
		this->gfxTexture = sector->gfxTexture;
	}

	Fade_in(FADE_NORMAL);

	this->setCamera();
}

void GameClass::openLocks()
{
	this->vibrate(90);

	showInfo(tekstit->Get_Text(PK_txt.game_locksopen));

	for (LevelSector *sector : this->level.sectors)
	{
		sector->openKeylocks(sector == this->playerSprite->level_sector);
	}

	if (this->lua != nullptr)
	{
		PK2lua::TriggerEventListeners(PK2lua::LUA_EVENT_KEYLOCKS_OPENED);
	}
}

void GameClass::showInfo(const std::string &text)
{

	if (info_text.compare(text) != 0 || info_timer == 0)
	{

		info_text = text;
		info_timer = INFO_TIME;
	}
}

void GameClass::drawInfoText()
{
	if (this->info_timer > 0)
	{
		std::pair<int, int> box_size = PDraw::font_get_text_size(fontti1, this->info_text);

		box_size.first += 8;
		box_size.second += 8;

		PDraw::RECT infoBG(screen_width / 2 - (box_size.first / 2), 60,
						   screen_width / 2 + (box_size.first / 2), 60 + box_size.second);

		int tmp = (box_size.second - this->info_timer) / 2;

		if (tmp > 0)
		{

			infoBG.y += tmp;
			infoBG.h -= tmp;
		}
		else if (this->info_timer > INFO_TIME - box_size.second)
		{
			// int tmp = 10 - (INFO_TIME - this->info_timer) / 2;
			tmp = (box_size.second - INFO_TIME + this->info_timer) / 2;

			infoBG.y += tmp;
			infoBG.h -= tmp;
		}

		PDraw::screen_fill(infoBG.x - 1, infoBG.y - 1, infoBG.w + 1, infoBG.h + 1, 51);
		PDraw::screen_fill(infoBG.x, infoBG.y, infoBG.w, infoBG.h, 38);

		// tmp = this->info_timer - 11
		tmp = this->info_timer - 1 - box_size.second;

		if (tmp >= 100)
			PDraw::font_write(fontti1, this->info_text, infoBG.x + 4, infoBG.y + 4);
		else if (tmp > 0)
			PDraw::font_writealpha_s(fontti1, this->info_text, infoBG.x + 4, infoBG.y + 4, tmp);
	}
}

void GameClass::setCamera(bool legacy_mode)
{

	LevelSector *sector = this->playerSprite->level_sector;

	if (legacy_mode)
	{
		this->camera_x = (int)this->playerSprite->x;
		this->camera_y = (int)this->playerSprite->y;
	}
	else
	{
		this->camera_x = (int)this->playerSprite->x - screen_width / 2;
		this->camera_y = (int)this->playerSprite->y - screen_height / 2;
	}

	if (this->camera_x < 0)
		this->camera_x = 0;

	if (this->camera_y < 0)
		this->camera_y = 0;

	if (this->camera_x > int(sector->getWidth() - screen_width / 32) * 32)
		this->camera_x = int(sector->getWidth() - screen_width / 32) * 32;

	if (this->camera_y > int(sector->getHeight() - screen_height / 32) * 32)
		this->camera_y = int(sector->getHeight() - screen_height / 32) * 32;

	this->dcamera_x = this->camera_x;
	this->dcamera_y = this->camera_y;

	this->dcamera_a = 0;
	this->dcamera_b = 0;
}

void GameClass::vibrate(int vibration){
	this->vibration = vibration;
	Settings.vibrateController(vibration);
}

void GameClass::updateCamera()
{
	this->camera_x = (int)this->playerSprite->x - screen_width / 2;
	this->camera_y = (int)this->playerSprite->y - screen_height / 2;

	LevelSector *sector = playerSprite->level_sector;

	if (dev_mode && PInput::Key::MOUSE_LEFT.isPressed() && !Settings.touchscreen_mode){

		const Point2D& mousePos = PInput::InputSystem::instance().getMousePos();
		this->camera_x += mousePos.x - screen_width / 2;
		this->camera_y += mousePos.y - screen_height / 2;
	}

	if (this->vibration > 0)
	{
		this->dcamera_x += (rand() % this->vibration - rand() % this->vibration) / 5;
		this->dcamera_y += (rand() % this->vibration - rand() % this->vibration) / 5;

		this->vibration--;
	}

	if (this->button_vibration > 0)
	{
		this->dcamera_x += (rand() % 9 - rand() % 9); // 3
		this->dcamera_y += (rand() % 9 - rand() % 9);

		this->button_vibration--;
	}

	if (this->dcamera_x != this->camera_x)
		this->dcamera_a = (this->camera_x - this->dcamera_x) / 15;

	if (this->dcamera_y != this->camera_y)
		this->dcamera_b = (this->camera_y - this->dcamera_y) / 15;

	if(Episode->legacy_camera_offset){
		if (this->dcamera_a > 6)
			this->dcamera_a = 6;

		if (this->dcamera_a < -6)
			this->dcamera_a = -6;

		if (this->dcamera_b > 6)
			this->dcamera_b = 6;

		if (this->dcamera_b < -6)
			this->dcamera_b = -6;
	}


	this->dcamera_x += this->dcamera_a;
	this->dcamera_y += this->dcamera_b;

	this->camera_x = (int)this->dcamera_x;
	this->camera_y = (int)this->dcamera_y;

	if (this->camera_x < 0)
		this->camera_x = 0;

	if (this->camera_y < 0)
		this->camera_y = 0;

	if (this->camera_x > int(sector->getWidth() - screen_width / 32) * 32)
		this->camera_x = int(sector->getWidth() - screen_width / 32) * 32;

	if (this->camera_y > int(sector->getHeight() - screen_height / 32) * 32)
		this->camera_y = int(sector->getHeight() - screen_height / 32) * 32;
}


void GameClass::saveGameState()const{

	PLog::Write(PLog::INFO, "PK2", "Saving checkpoint...");

	fs::path dataPath = PFilesystem::GetDataPath();
	fs::path p1 = dataPath / CHECKPOINT_DIR_NAME / "level.map";
	fs::path p2 = dataPath / CHECKPOINT_DIR_NAME / "game.dat";
	fs::path p3 = dataPath / CHECKPOINT_DIR_NAME / "score.dat";

	this->level.saveVersion15(PFile::Path(p1.string()));

	PFile::RW file2 = PFile::Path(p2.string()).GetRW2("w");
	file2.writeCBOR(this->toJson());
	file2.close();
	
	PLog::Write(PLog::DEBUG, "PK2", "Checkpoint saved!");

	PFile::RW file3 = PFile::Path(p3.string()).GetRW2("w");
	file3.write(this->score);
	file3.close();
}

bool GameClass::hasEnoughPointsToRespawn(){

	if(Episode->checkpointPenalty<=0)return true;

	fs::path dataPath = PFilesystem::GetDataPath();
	fs::path p3 = dataPath / CHECKPOINT_DIR_NAME / "score.dat";

	PFile::RW in = PFile::Path(p3.string()).GetRW2("r");
	in.read(this->score);
	in.close();

	if(this->score < Episode->checkpointPenalty){
		return false;
	}
	this->score -= Episode->checkpointPenalty;
	PFile::RW out = PFile::Path(p3.string()).GetRW2("w");
	out.write(this->score);
	out.close();

	return true;
}


void GameClass::loadGameState(){

	PLog::Write(PLog::INFO, "PK2", "Loading checkpoint...");

	fs::path dataPath = PFilesystem::GetDataPath();
	fs::path p1 = dataPath / CHECKPOINT_DIR_NAME / "level.map";
	fs::path p2 = dataPath / CHECKPOINT_DIR_NAME / "game.dat";
	fs::path p3 = dataPath / CHECKPOINT_DIR_NAME / "score.dat";


	this->level.clearSectors();
	this->level.load(PFile::Path(p1.string()), false);
	this->lastCheckpoint = nullptr;
	

	PFile::RW file = PFile::Path(p2.string()).GetRW2("r");

	this->fromJson(file.readCBOR());
	file.close();

	this->info_timer = 0;
	PLog::Write(PLog::DEBUG, "PK2", "Checkpoint loaded!");
}


nlohmann::json GameClass::toJson() const
{
	nlohmann::json j;
	j["game_over"] = this->game_over;
	j["level_clear"] = this->level_clear;
	j["repeating"] = this->repeating;
	j["exit_timer"] = this->exit_timer;
	j["timeout"] = this->timeout;
	j["has_time"] = this->has_time;
	j["frame_count"] = this->frame_count;
	j["tiles_animation_counter"] = this->tiles_animation_counter;
	j["button_vibration"] = this->button_vibration;
	j["button1"] = this->button1;
	j["button2"] = this->button2;
	j["button3"] = this->button3;
	j["score"] = this->score;
	j["score_increment"] = this->score_increment;
	j["apples_count"] = this->apples_count;
	j["apples_got"] = this->apples_got;
	j["vibration"] = this->vibration;
	/*j["camera_x"] = this->camera_x;
	j["camera_y"] = this->camera_y;
	j["dcamera_x"] = this->dcamera_x;
	j["dcamera_y"] = this->dcamera_y;
	j["dcamera_a"] = this->dcamera_a;
	j["dcamera_b"] = this->dcamera_b;
	j["paused"] = this->paused;*/
	j["music_stopped"] = this->music_stopped;
	j["keys"] = this->keys;
	j["enemies"] = this->enemies;
	j["info_timer"] = this->info_timer;
	j["info_text"] = this->info_text;
	j["item_panel_x"] = this->item_panel_x;
	j["change_skulls"] = this->change_skulls;
	j["event1"] = this->event1;
	j["event2"] = this->event2;

	if(this->lastCheckpoint==nullptr){
		j["last_cp_id"] = nullptr;
	}
	else{
		j["last_cp_id"] = this->lastCheckpoint->id;
	}

	std::vector<nlohmann::json> sprites;
	for (const LevelSector *sector : this->level.sectors)
	{
		sprites.emplace_back(sector->sprites.toJson());
	}

	j["sprites"] = sprites;
	j["gifts"] = this->gifts.toJson();

	return j;
}

void GameClass::fromJson(const nlohmann::json &j)
{
	// Restore basic game state
	j.at("game_over").get_to(this->game_over);
	j.at("level_clear").get_to(this->level_clear);
	j.at("repeating").get_to(this->repeating);
	j.at("exit_timer").get_to(this->exit_timer);
	j.at("timeout").get_to(this->timeout);
	j.at("has_time").get_to(this->has_time);
	j.at("frame_count").get_to(this->frame_count);
	j.at("tiles_animation_counter").get_to(this->tiles_animation_counter);
	j.at("button_vibration").get_to(this->button_vibration);
	j.at("button1").get_to(this->button1);
	j.at("button2").get_to(this->button2);
	j.at("button3").get_to(this->button3);
	j.at("score").get_to(this->score);
	j.at("score_increment").get_to(this->score_increment);
	j.at("apples_count").get_to(this->apples_count);
	j.at("apples_got").get_to(this->apples_got);
	j.at("vibration").get_to(this->vibration);
	/*j.at("camera_x").get_to(this->camera_x);
	j.at("camera_y").get_to(this->camera_y);
	j.at("dcamera_x").get_to(this->dcamera_x);
	j.at("dcamera_y").get_to(this->dcamera_y);
	j.at("dcamera_a").get_to(this->dcamera_a);
	j.at("dcamera_b").get_to(this->dcamera_b);
	j.at("paused").get_to(this->paused);*/
	j.at("music_stopped").get_to(this->music_stopped);
	j.at("keys").get_to(this->keys);
	j.at("enemies").get_to(this->enemies);
	j.at("info_timer").get_to(this->info_timer);
	j.at("info_text").get_to(this->info_text);
	j.at("item_panel_x").get_to(this->item_panel_x);
	j.at("change_skulls").get_to(this->change_skulls);
	j.at("event1").get_to(this->event1);
	j.at("event2").get_to(this->event2);
	// Restore sprites for each sector
	const auto &sprites_json = j.at("sprites");

	std::optional<std::size_t> lastCheckpointId;
	if(!j.at("last_cp_id").is_null()){
		lastCheckpointId = j.at("last_cp_id").get<std::size_t>();
	}

	this->lastCheckpoint = nullptr;
	this->playerSprite = nullptr;

	for (size_t i = 0; i < sprites_json.size() && i < this->level.sectors.size(); ++i) {

		LevelSector* sector =  this->level.sectors.at(i);
		SpritesHandler& sprites = sector->sprites;
		sprites.fromJSON(sprites_json[i], this->spritePrototypes, sector);

		SpriteClass* player = sprites.findPlayer();
		if(player!=nullptr){
			this->playerSprite = player;
		}

		if(lastCheckpointId.has_value()){
			SpriteClass * s = sprites.getSpriteById(*lastCheckpointId);
			if(s!=nullptr){
				this->lastCheckpoint = s;
			}
		}
	}

	if(this->playerSprite==nullptr){
		throw std::runtime_error("The player sprite wasn't found while loading saved game!");
	}

	this->gifts.fromJson(j.at("gifts"), this->spritePrototypes);

	
	// Update camera and GFX texture
	this->setCamera();
	this->gfxTexture = this->playerSprite->level_sector->gfxTexture;
	this->exposePlayerToAIs();
}