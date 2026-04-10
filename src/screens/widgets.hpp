//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
#pragma once

#include <string>
#include <vector>
#include "engine/PString.hpp"
#include "engine/PInputKey.hpp"
#include "gfx/text.hpp"

#include "keyboard_navigation.hpp"

namespace PDraw{
    class RECT;
}

namespace PK2gui{

bool Draw_BoolBox(int x, int y, bool value, bool active, bool mouseClicked);

class TextInput{
public:
    TextInput(std::size_t max_chars, const std::string& rejected="");

    void startInput();
    void endInput();

    void clear();
    void setText(const std::string& text);
    std::string getText()const;


    bool draw(const std::string& title, int tx_start, int ty_start, bool mouseClicked);
    int getMaxChars()const{
        return (int)this->buffer.size();
    }    
    bool isEditing(){
        return this->editing;
    }

    bool updateNavigation(const KeyNav::Nav& nav);
private:
    int getTextSize()const;
    bool acceptInputChar(PString::UTF8_Char c)const;

    std::vector<PString::UTF8_Char> rejectedChars;
    std::vector<PString::UTF8_Char> buffer;
    int selectedIndex = 0;
    bool editing = false;
};


class LinksMenu{
public:
    void draw(bool mouseClicked);
private:
    bool drawButton(int x, int y, const PDraw::RECT& rect, const std::string& label, bool mouseClicked);
    bool expanded = false;
};

}