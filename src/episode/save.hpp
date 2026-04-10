//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * New saving system by Saturnin
 */

#pragma once

//#include "episodeclass.hpp"

#include "engine/types.hpp"
#include <string>
#include <vector>

class EpisodeClass;

namespace PK2save{

void LoadModern(EpisodeClass* episode);
void SaveModern(const EpisodeClass* episode);

}
