//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################
/**
 * @brief 
 * Missing texture placehoder generator by Saturnin
 */
#include "missing_texture.hpp"
#include "engine/PDraw.hpp"

int new_missing_texture_placeholder(u8 color1, u8 color2, u32 width,u32 height){

    int texture = PDraw::image_new(width, height);
    u8* pixels = nullptr;
    u32 pitch = 0;

    int cx = -1;
    int cy = -1;

    bool flipx = false;
    bool flipy = false;

    PDraw::drawimage_start(texture, pixels, pitch);
    

    for(u32 y=0;y<height;++y){

        ++cy;
        if(cy>=8){
            cy = 0;
            flipy = !flipy;
        }

        for(u32 x=0;x<pitch;++x){
            ++cx;
            if(cx>=8){
                cx = 0;
                flipx = !flipx;
            }
            
            pixels[y*pitch + x] =  flipx == flipy ? color1 : color2;
        }
    }
    PDraw::drawimage_end(texture);
    return texture;
}
