/*
c++ test.cpp -o test -lSDLmain -lSDL -lpthread -lSDL_image
*/
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#define PD_TAUSTABUFFER 0

const int SCREEN_WIDTH			= 800;
const int SCREEN_HEIGHT			= 600;
bool Running				= false;// jos tämä muuttuu todeksi niin ohjelma lopetetaan
SDL_Surface *PD_buffers[2];

void Game_Init() {
  PD_buffers[PD_TAUSTABUFFER] = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE|SDL_DOUBLEBUF);
  if(PD_buffers[PD_TAUSTABUFFER] == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
   PD_buffers[1] = IMG_Load("wormz2.bmp");
		Running = true;
}

int map_x = 0;
int Game_Main(void)
{
	map_x = 1 + map_x%640;
	SDL_FillRect(PD_buffers[PD_TAUSTABUFFER], NULL, rand()%60);
	SDL_Rect src = {map_x, 0, 0, 0};
	SDL_BlitSurface(PD_buffers[1], NULL, PD_buffers[PD_TAUSTABUFFER], &src);
	SDL_Flip(PD_buffers[PD_TAUSTABUFFER]);
}

int main(int argc, char *argv[])
{

  if ( SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0 )
  {
    printf("Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

	Game_Init();

	SDL_Event event;
	while(Running) {
		SDL_PollEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				Running = false;
				break;
		}
		Game_Main();
	}

}

