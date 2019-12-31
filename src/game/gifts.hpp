//#########################
//Pekka Kana 2
//by Janne Kivilahti from Piste Gamez (2003)
//#########################
#pragma once

#include "game/sprites.hpp"

#define MAX_GIFTS 4

bool Gifts_Add(int prototype_id);
int Gifts_Use();
void Gifts_Clean();
int Gifts_Count();

int Gifts_Get(int i);
void Gifts_Remove(int i);
PrototypeClass* Gifts_GetProtot(int i);
void Gifts_Draw(int i, int x, int y);
int Gifts_ChangeOrder();