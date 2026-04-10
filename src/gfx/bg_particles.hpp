//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#define BACKGROUND_PARTICLES_NUMBER 300

enum {

    WEATHER_NORMAL, //0
    WEATHER_RAIN, //1
    WEATHER_LEAVES, //2
    WEATHER_RAIN_LEAVES, //3
    WEATHER_SNOW, //4 
	WEATHER_DANDELIONS //5
};

namespace BG_Particles{

void Init(int weather, int rain_color);
void Draw(int camera_x, int camera_y);
void Update(int camera_x, int camera_y);

}

