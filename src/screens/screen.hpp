//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include "engine/types.hpp"
#include "engine/PFile.hpp"
#include "engine/PInputKey.hpp"

#include <string>

enum SCREEN_TYPE {
	SCREEN_NOT_SET,
	SCREEN_FIRST_START,
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_MAP,
	SCREEN_GAME,
	SCREEN_SCORING,
	SCREEN_END,

	SCREEN_LEVEL_ERROR,
};

class Screen{
public:
	bool mouseKeysEnabled = false;
	static int next_screen;
	virtual ~Screen(){}
	virtual void Init()=0;
	virtual void Loop()=0;

	virtual void onKeyPressed(const PInput::Key& key);
	virtual void onKeyReleased(const PInput::Key& key);

	static bool closing_game;
	void clearMouseInput();


	static bool mouse_hidden;

protected:
	static int bg_screen;


	static void drawMouseCursor();
	bool enterPressed = false;
	bool mousePressed = false;

	

	bool drawMenuText(int id, int x, int y);
	virtual bool drawMenuTextS(const std::string& text, int x, int y);
	static void fadeQuit();	
};