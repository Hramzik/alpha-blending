#ifndef GRAPHICS_STRUCTS_HPP_INCLUDED
#define GRAPHICS_STRUCTS_HPP_INCLUDED
//--------------------------------------------------

#include "graphics.hpp"

//--------------------------------------------------


typedef struct Mixer_Media {

    Picture32 top_pic;   // stored by lines
    Picture24 bottom_pic;

    Picture32 result_pic;

} Mixer_Media; const size_t MIXER_MEDIA_SIZE = sizeof (Mixer_Media);


typedef struct Mixer_Output {

    SDL_Window* window;

    SDL_Renderer* renderer;

} Mixer_Output; const size_t MIXER_OUTPUT_SIZE = sizeof (Mixer_Output);


typedef struct Mixer_Conditions {

    bool exit;

    bool pressed_w;
    bool pressed_a;
    bool pressed_s;
    bool pressed_d;

    bool pressed_left;
    bool pressed_right;
    bool pressed_up;
    bool pressed_down;

} Mixer_Conditions; const size_t MIXER_CONDITIONS_SIZE = sizeof (Mixer_Conditions);


typedef struct Mixer_Data {

    Point top_pic_offset;

    int window_width;
    int window_height;

    int horizontal_speed;
    int vertical_speed;

    Picture_Id currently_mirroring;

} Mixer_Data; const size_t MIXER_DATA_SIZE = sizeof (Mixer_Data);


//--------------------------------------------------
#endif