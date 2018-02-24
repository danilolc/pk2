//SDL2 flashing random color example
//Should work on iOS/Android/Mac/Windows/Linux

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <android/log.h>
#define printf(x) __android_log_write(ANDROID_LOG_ERROR, "PK2", x)

#include <unistd.h>

#define AUDIO_FREQ 44100

int main (int argc, char** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	
	char cwd[100];
	getcwd(cwd,100);
	printf("Init SDL\n");
	printf(cwd);
	//int a[2];
	//int b = a[10000];

	SDL_Window* pWindow = NULL;
	pWindow = SDL_CreateWindow("PK2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 1920, SDL_WINDOW_SHOWN);
	SDL_Renderer* PD_Renderer = NULL;
	PD_Renderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	SDL_SetRenderDrawColor(PD_Renderer, 255, 0, 0, 255);
	SDL_RenderClear(PD_Renderer);
	SDL_RenderPresent(PD_Renderer);


	if( Mix_OpenAudio(AUDIO_FREQ, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
		printf("Can't open MIX");
		return -1;
	}
	Mix_Init(MIX_INIT_MOD || MIX_INIT_MP3 || MIX_INIT_OGG);
	Mix_Music* music = Mix_LoadMUS("/storage/sdcard0/Download/song13.xm");
	if (music == NULL)
		printf("Music not found");
	Mix_PlayMusic( music, -1);

    while(1);

    SDL_Quit();
    return 0;
}
