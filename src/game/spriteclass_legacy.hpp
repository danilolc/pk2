//#########################
//Pekka Kana 2
//Copyright (c) 2003 Janne Kivilahti
//#########################

/**
 * @brief 
 * Legacy .spr sprites support.
 */

#pragma once

#include "engine/types.hpp"

struct LegacySprAnimation {

    u8    sequence[10] = {0}; // sequence
    u8    frames = 0;                              // frames
    bool  loop = false;                           // loop

};

//.spr file structures
struct PrototypeClass10{

    u32     sprite_type;

    char    picture[13];

    char    sound_files[7][13];
    u32     sounds[7];

    u8      frames_number;
    LegacySprAnimation animaatiot[20];
    u8      animations_number;
    u8      frame_rate;
    u32     picture_frame_x;
    u32     picture_frame_y;
    u32     picture_frame_width;
    u32     picture_frame_height;
    u32     kuva_frame_vali;

    char    name[30];
    u32     width;
    u32     height;
    double  weight;
    bool    enemy;
    u32     energy;
    u32     damage;
    u32     score;
    u32     AI[5];
    u32     immunity_type;
    u8      max_jump;
    u8      max_speed;
    u32     charge_time;
    u8      color;
    bool    is_wall;
    u32     how_destroyed;

    char    transformation_str[13];
    char    bonus_str[13];
    char    ammo1_str[13];
    char    ammo2_str[13];
    bool    can_open_locks;
};
struct PrototypeClass11{

    u32     sprite_type;

    char    picture[13];

    char    sound_files[7][13];
    u32     sounds[7];

    u8      frames_number;
    LegacySprAnimation animaatiot[20];
    u8      animations_number;
    u8      frame_rate;
    u32     picture_frame_x;
    u32     picture_frame_y;
    u32     picture_frame_width;
    u32     picture_frame_height;
    u32     kuva_frame_vali;

    char    name[30];
    u32     width;
    u32     height;
    double  weight;
    bool    enemy;
    u32     energy;
    u32     damage;
    u8      damage_type;
    u8      immunity_type;
    u32     score;
    u32     AI[5];
    u8      max_jump;
    u8      max_speed;
    u32     charge_time;
    u8      color;
    bool    is_wall;
    u32     how_destroyed;
    bool    can_open_locks;
    bool    vibrates;
    u8      bonuses_number;
    u32     attack1_time;
    u32     attack2_time;
    u32     parallax_type;

    char    transformation_str[13];
    char    bonus_str[13];
    char    ammo1_str[13];
    char    ammo2_str[13];

};
struct PrototypeClass12{

    u32     sprite_type;

    char    picture[13];

    char    sound_files[7][13];
    u32     sounds[7];

    u8      frames_number;
    LegacySprAnimation animaatiot[20];
    u8      animations_number;
    u8      frame_rate;
    u32     picture_frame_x;
    u32     picture_frame_y;
    u32     picture_frame_width;
    u32     picture_frame_height;
    u32     kuva_frame_vali;

    char    name[30];
    u32     width;
    u32     height;
    double  weight;
    bool    enemy;
    u32     energy;
    u32     damage;
    u8      damage_type;
    u8      immunity_type;
    u32     score;
    u32     AI[5];
    u8      max_jump;
    u8      max_speed;
    u32     charge_time;
    u8      color;
    bool    is_wall;
    u32     how_destroyed;
    bool    can_open_locks;
    bool    vibrates;
    u8      bonuses_number;
    u32     attack1_time;
    u32     attack2_time;
    u32     parallax_type;

    char    transformation_str[13];
    char    bonus_str[13];
    char    ammo1_str[13];
    char    ammo2_str[13];

    bool    check_tiles;
    u32     sound_frequency;
    bool    random_sound_frequency;

    bool    is_wall_up;
    bool    is_wall_down;
    bool    is_wall_right;
    bool    is_wall_left;

};

struct PrototypeClass13{

    u32     sprite_type;                             // sprite type
    char    picture[100];                  // bmp path

    /**
     * @brief 
     * Sound files.
     * The index determines the sound type:
     * 0 -> damage
     * 1 -> destruction
     * 2 -> attack1
     * 3 -> attack2
     * 4 -> random
     * 5 -> special1
     * 6 -> special2
     */
    char    sound_files[7][100];
    /**
     * @brief 
     * This field is unused, seems to be -1 in all legacy sprites.
     * Although it's copied to SpritePrototype, but later is replaced while the sound loading.
     */
    u32     sounds[7];                           

    u8      frames_number;                            // number of frames
    LegacySprAnimation animaatiot[20];         // animation sequences
    u8      animations_number;                       // number of animations
    u8      frame_rate;                         // frame rate
    u32     picture_frame_x;                             // x position of first frame
    u32     picture_frame_y;                             // y position of first frame
    u32     picture_frame_width;                  // frame width
    u32     picture_frame_height;                 // frame height
    u32     picture_frame_space;                    // space between frames //unused


    char    name[30];                           // name
    u32     width;                             // width
    u32     height;                            // height
    double  weight;                             // weight (for jump and switches)
    bool    enemy;                         // if sprite is a enemy
    u32     energy;                            //?sprite energy
    u32     damage;                            //?damage if it got hit
    u8      damage_type;                     // damage type
    u8      immunity_type;                            // immunity type
    u32     score;                            // how much score
    u32     AI[10];                             // AI type (max 10)
    u8      max_jump;                          // max jump time
    double  max_speed;                         // max speed
    u32     charge_time;                        // wait post shoot
    u8      color;                               // color
    bool    is_wall;                               // is a wall
    u32     how_destroyed;                       // how sprite is destroyed
    bool    can_open_locks;                              // can sprite open locks
    bool    vibrates;                            // vibrate sprite randomly
    u8      bonuses_number;                       // number of bonuses
    u32     attack1_time;                       // attack 1 duration (frames)
    u32     attack2_time;                       // attack 2 duration (frames)
    u32     parallax_type;                    // parallax type (just to TYPE_BACKGROUND)


    char    transformation_str[100];                 // another sprite that this sprite may change
    char    bonus_str[100];                  // bonus that this sprite gives
    char    ammo1_str[100];                 // ammo 1 sprite
    char    ammo2_str[100];                 // ammo 2 sprite


    bool    check_tiles;                      //?make sounds?
    u32     sound_frequency;                           // sound frequency (def. 22050)
    bool    random_sound_frequency;                         // use random frequency?


    bool    is_wall_up;                          // if is wall at up
    bool    is_wall_down;                          // if is wall at down
    bool    is_wall_right;                      // if is wall at right
    bool    is_wall_left;                    // if is wall at left


    u8      effect;                             // sprite effect
    bool    is_transparent;                             // if it is transparent //unused
    u32     projectile_charge_time;                          //?charge_time inflicted by projectile sprites on the shooter
    bool    can_glide;                          // can drip quietly down?
    bool    boss;                               // if it is a boss //unused
    bool    bonus_always;                       // if not there is 1/4 chance of droping bonus
    bool    can_swim;                           // walk fast under water

};