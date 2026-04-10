//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"
#include "engine/PFile.hpp"
#include "engine/PInput.hpp"

#include "screen.hpp"
#include <vector>
#include <map>

class ScreensHandler{
public:
	ScreensHandler();
	~ScreensHandler();
	void Loop();

	void onKeyPressed(const PInput::Key& key);
	void onKeyReleased(const PInput::Key& key);

	bool clicked = false;
private:
	Screen* current_screen = nullptr;
	int current_screen_index = SCREEN_NOT_SET;

	std::map<int, Screen*> screens_map;
};
