//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief
 * Conway's Game of Life implemented as an easter egg by SaturninTheAlien
 * Accessible only with the Lua 🌜 API.
 * 
 * Learn more about GoL:
 * https://conwaylife.com/wiki/Conway%27s_Game_of_Life
 */

#include "life.hpp"
#include "game/levelsector.hpp"
#include "exceptions.hpp"
#include "game/game.hpp"
#include "engine/PFilesystem.hpp"

#include "system.hpp"
#include <sstream>


namespace PK2lua{

class GameOfLife{
public:
    GameOfLife(LevelSector* sector, u8  cell_inactive, u8  cell_alive, u8  cell_dead);
    ~GameOfLife();


    void nextState();
    void start(int speed);
    bool started()const{
        return this->life_speed!=0;
    }

	void placeRLE(const std::string& name, int x, int y, int direction);
    void placeCell(bool alive, int x, int y);

	int getSpeed()const{
		return this->life_speed;
	}

private:
    void incrementBuffer(std::size_t y, std::size_t x);

    LevelSector * sector = nullptr;
    u8* life_buffer = nullptr;

    const u8  block_cell_inactive;
    const u8  block_cell_alive;
    const u8  block_cell_dead;

    int life_speed = 0;
};


GameOfLife::GameOfLife(LevelSector*sector, u8  cell_inactive, u8  cell_alive, u8  cell_dead):
sector(sector), block_cell_inactive(cell_inactive), block_cell_alive(cell_alive), block_cell_dead(cell_dead){
    this->life_buffer = new u8[sector->size()];
    memset(this->life_buffer, u8(0), sector->size());
}

GameOfLife::~GameOfLife(){
    delete[] this->life_buffer;
    this->life_buffer = nullptr;
}

void GameOfLife::start(int speed){
    this->life_speed = speed;

    for(std::size_t index=0; index<this->sector->size(); ++index){
        u8 back  = sector->background_tiles[index];

        if(back == this->block_cell_alive){
            sector->foreground_tiles[index] = BLOCK_BARRIER_DOWN;
        }
        else if(back == this->block_cell_inactive){
            sector->background_tiles[index] = this->block_cell_alive;
            sector->foreground_tiles[index] = BLOCK_BARRIER_DOWN;
        }
    }
}

void GameOfLife::nextState(){
    for(int y=0;y<(int)sector->getHeight();++y){
        for(int x=0;x<(int)sector->getWidth();++x){

            std::size_t index = y * sector->getWidth() + x;

            u8 back = sector->background_tiles[index];
            if(back==this->block_cell_alive){
                this->incrementBuffer(y-1, x-1);
				this->incrementBuffer(y-1, x);
				this->incrementBuffer(y-1, x+1);

				this->incrementBuffer(y, x-1);
				this->incrementBuffer(y, x+1);

				this->incrementBuffer(y+1, x-1);
				this->incrementBuffer(y+1, x);
				this->incrementBuffer(y+1, x+1);
            }
        }
    }

    for(std::size_t index=0; index< sector->size(); ++index){

        u8 back  = sector->background_tiles[index];
        u8 neighbours = this->life_buffer[index];
        this->life_buffer[index] = 0;



        if(back == this->block_cell_alive && neighbours !=2 && neighbours != 3){
            sector->background_tiles[index] = this->block_cell_dead;
            sector->foreground_tiles[index] = 255;
        }
        else if(back == this->block_cell_dead && neighbours == 3){
            sector->background_tiles[index] = this->block_cell_alive;
            sector->foreground_tiles[index] = BLOCK_BARRIER_DOWN;
        }
    }
}

void GameOfLife::incrementBuffer(std::size_t y, std::size_t x){
    y = (y + this->sector->getHeight()) % this->sector->getHeight();
	x = (x + this->sector->getWidth()) % this->sector->getWidth();

	u32 index = u32(x+y*this->sector->getWidth());
	++this->life_buffer[index];
}


void GameOfLife::placeCell(bool alive, int x, int y){

    if(x>=0 && x < (int)this->sector->getWidth() && y>=0 && y<(int)this->sector->getHeight()){

		u32 index = x+y*this->sector->getWidth();

		u8 back = this->sector->background_tiles[index];
		
		if(back==this->block_cell_dead && alive){

			if(this->started()){
				this->sector->background_tiles[index] = this->block_cell_alive;
				this->sector->foreground_tiles[index] = BLOCK_BARRIER_DOWN;
			}
			else{
				this->sector->background_tiles[index] = this->block_cell_inactive;
			}
		}
		else if(back==this->block_cell_alive && !alive){

			this->sector->background_tiles[index] = this->block_cell_dead;
			this->sector->foreground_tiles[index] = 255;
		}
	}
}

void GameOfLife::placeRLE(const std::string& name, int x, int y, int direction){

	std::optional<PFile::Path> path = PFilesystem::FindAsset(name, PFilesystem::LIFE_DIR);

	if(!path.has_value()){
		std::ostringstream os;
		os<<"RLE file: \""<<name<<"\" not found!";
		throw PExcept::PException(os.str());
	}


	int dx = 1;
	int dy = 1;

	switch (direction)
	{
	case 1:
		dx = 1;
		dy = 1;
		break;
	case 2:
		dx = -1;
		dy = 1;
		break;

	case 3:
		dx = -1;
		dy = -1;
		break;
	
	case 4:
		dx = 1;
		dy = -1;
	default:
		break;
	}


	PFile::RW rw = path->GetRW2("r");

	char c1 = 0;
	int state = 0;

	int n = 0;
	int orig_x = x;

	while (rw.read(&c1, 1)){
		
		if(state!=1 && c1=='!')break;

		/* code */
		switch (state)
		{
		/**
		 * @brief 
		 * default
		 */
		case 0:{
			/**
			 * @brief
			 * Numbers
			 */
			if(c1>='0' && c1<='9'){
				state = 2;
				n = c1 - '0';
			}

			/**
			 * @brief
			 * Ignore comments and metadata
			 */
			else if(c1=='#' || c1=='x' || c1=='y'){
				state = 1;
			}

			/**
			 * @brief 
			 * New line
			 */
			else if(c1=='$'){
				x = orig_x;
				y+=dy;
			}
			/**
			 * @brief 
			 * Dead cell
			 */
			else if(c1=='b'){
				this->placeCell(false, x, y);
				x+=dx;				
			}
			/**
			 * @brief 
			 * Alive cell
			 */
			else if(c1=='o'){
				this->placeCell(true, x, y);
				x+=dx;
			}

		}

		break;
		/**
		 * @brief 
		 * Reading a comment
		 */
		case 1:{
			if(c1=='\n'){
				state = 0;
			}
		}
		break;

		/**
		 * @brief 
		 * Reading a number
		 */
		case 2:{
			if(c1>='0' && c1<='9'){
				n *= 10;
				n += c1 - '0';
			}
			/**
			 * @brief 
			 * New line
			 */
			else if(c1=='$'){
				y += n*dy;
				x = orig_x;
				state = 0;
			}
			/**
			 * @brief 
			 * Dead cell
			 */
			else if(c1=='b'){
				for(int i=0;i<n;++i){
					this->placeCell(false, x, y);
					x+=dx;
				}
				state = 0;
			}
			/**
			 * @brief 
			 * Alive cell
			 */
			else if(c1=='o'){
				for(int i=0;i<n;++i){
					this->placeCell(true, x, y);
					x+=dx;
				}
				state = 0;
			}

		}

		default:
			break;
		}

	}
}


static GameOfLife* life = nullptr;
static int life_timer = 0;

GameOfLife* prepare_game_of_life(int sector_id, int block_cell_inactive, int block_cell_alive, int block_cell_dead){

	if(life!=nullptr){
		throw PExcept::PException("Game of life already prepared!");
	}


	LevelSector* sector = Game->level.sectors.at(sector_id);

	life = new GameOfLife(sector, block_cell_inactive, block_cell_alive, block_cell_dead);
	return life;
}

void ExposeGameOfLife(sol::table& PK2_API){
	PK2_API.new_usertype<GameOfLife>(
		"GameOfLife",
		sol::no_constructor,
		"start", &GameOfLife::start,
		"placeRle", &GameOfLife::placeRLE,
		"placeCell", &GameOfLife::placeCell,
		"nextState", &GameOfLife::nextState
	);

	PK2_API["prepareLife"] = prepare_game_of_life;
}

void UpdateGameOfLife(){
	if(life!=nullptr && life->started()){
		++life_timer;

		if(life_timer>=life->getSpeed()){
			life_timer = 0;
			life->nextState();
		}
	}
}

void ClearGameOfLife(){

	if(life!=nullptr){
		delete life;
		life = nullptr;
	}
}

}