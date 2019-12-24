#pragma once

enum UI_MODE {
	UI_TOUCH_TO_START,
	UI_CURSOR,
	UI_GAME_BUTTONS
};

void GUI_Activate(bool set);
void GUI_Change(int ui_mode);
void GUI_Load();