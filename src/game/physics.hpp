//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "game/spriteclass.hpp"

//void PlayerRespawnOnCheckpoint(SpriteClass * player, SpriteClass * checkpoint);
void SpriteOffscreen(SpriteClass * sprite);
void SpriteOnRespawn(SpriteClass* sprite);
void SpriteOnDeath(SpriteClass* sprite);
void UpdateSprite(SpriteClass* sprite);
void UpdateBonusSprite(SpriteClass* sprite);
void UpdateBackgroundSprite(SpriteClass* sprite, double &yl);