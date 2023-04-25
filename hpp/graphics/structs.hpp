#ifndef GRAPHICS_STRUCTS_HPP_INCLUDED
#define GRAPHICS_STRUCTS_HPP_INCLUDED
//--------------------------------------------------

#include "graphics.hpp"

//--------------------------------------------------


typedef struct Mixer_media {

    Picture32 top_pic;   // stored by lines
    Picture24 bottom_pic;

    Picture32 result_pic;

} Mixer_media; const size_t MIXER_MEDIA_SIZE = sizeof (Mixer_media);


typedef struct Mixer_output {

    SDL_Window* window;

    SDL_Renderer* renderer;

} Mixer_output; const size_t MIXER_OUTPUT_SIZE = sizeof (Mixer_output);


typedef struct Mixer_conditions {

    bool exit;

} Mixer_conditions; const size_t MIXER_CONDITIONS_SIZE = sizeof (Mixer_conditions);


typedef struct Mixer_data {

    Point top_pic_offset;

    size_t window_width;
    size_t window_height;

} Mixer_data; const size_t MIXER_DATA_SIZE = sizeof (Mixer_data);


//--------------------------------------------------
#endif