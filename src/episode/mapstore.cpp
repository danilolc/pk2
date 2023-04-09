//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#include "episode/mapstore.hpp"

#include "system.hpp"

#include "engine/PUtils.hpp"
#include "engine/PFile.hpp"
#include "engine/PLog.hpp"

#include <cstring>
#include <algorithm>

std::vector<episode_entry> episodes;

void Search_Episodes() {

	std::vector<std::string> list;
	
	list = PFile::Path("episodes" PE_SEP).scandir("/");
	for (std::string ep : list) {

		episode_entry e;
		e.name = ep;
		e.is_zip = false;
        episodes.push_back(e);
        
	}

	#ifdef PK2_USE_ZIP

	std::string mapstore_path(data_path + "mapstore" PE_SEP);
	
	list = PFile::Path(mapstore_path).scandir(".zip");
	for (std::string zip : list) {
		
		PFile::Zip* zp = PFile::OpenZip(mapstore_path + zip);
		if (zp == nullptr) {

			PLog::Write(PLog::ERR, "PK2", "Can't open zip file %s", zip.c_str());
			continue;

		}

		std::vector<std::string> zip_list = PFile::Path(zp, "episodes" PE_SEP).scandir("/");

		for (std::string ep : zip_list) {

			episode_entry e;
			e.name = ep;
			e.is_zip = true;
			e.zipfile = zip;
			episodes.push_back(e);
			
		}

		PFile::CloseZip(zp);
		
	}

	#endif

	if (episodes.size() > 1)
		std::stable_sort(episodes.begin(), episodes.end(),
		[](const episode_entry& a, const episode_entry& b) {
			return PUtils::Alphabetical_Compare(a.name.c_str(), b.name.c_str()) == 1;
		});
	
	PLog::Write(PLog::DEBUG, "PK2", "Found %i episodes", (int)episodes.size());

}


