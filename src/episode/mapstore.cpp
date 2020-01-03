
#include "episode/mapstore.hpp"

#include "engine/PUtils.hpp"
#include "engine/PFile.hpp"

#include <cstring>
#include <algorithm>

std::vector<episode_entry> episodes;

static bool Alphabetical(episode_entry a, episode_entry b) {

    return PUtils::Alphabetical_Compare(a.name.c_str(), b.name.c_str()) == 1;

}

void Search_Episodes() {

	std::vector<std::string> list = PUtils::Scandir("/", "episodes", 60);
	int size = list.size();

	//TODO - store the vector
	for (int i = 0; i < size; i++) {

        episodes.push_back(episode_entry());
        episodes[i].name = list[i];
        episodes[i].zipfile = false;
        
	}

	
	
	list = PUtils::Scandir(".zip", "data" PE_SEP "mapstore", 60);

	for (std::string zip : list) {
		
		// TODO - get episodes from zip
		
	}

	

	std::sort(episodes.begin(), episodes.end(), Alphabetical);

}


