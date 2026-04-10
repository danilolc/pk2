//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"

void Effect_Circle_Star(double x, double y, u8 color);
void Effect_Super(double x, double y, int w, int h);
void Effect_Points(double x, double y, int w, int h, u8 color);

void Effect_Feathers(double x, double y);
void Effect_Splash(double x, double y, u8 color);
void Effect_Explosion(double x, double y, u8 color);
void Effect_Smoke(double x, double y, u8 color);
void Effect_SmokeClouds(double x, double y);
void Effect_Stars(double x, double y, u8 color);


void Effect_By_ID(int effect_id, double x, double y);
