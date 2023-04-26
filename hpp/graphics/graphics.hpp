#ifndef GRAPHICS_HPP_INCLUDED
#define GRAPHICS_HPP_INCLUDED
//--------------------------------------------------


#define  SDL_MAIN_HANDLED // no argc, argv
#include "../../../../../lib/SDL2/include/SDL2/SDL.h" // for VSC to highlight everything before compile time


//--------------------------------------------------


#include "../common/common.hpp"


//---------------- settings ------------------------


#define TOP_PICTURE_PATH    "media/cat.bmp"
#define BOTTOM_PICTURE_PATH "media/table.bmp"

const size_t MIXER_WINDOW_WIDTH  = 400;
const size_t MIXER_WINDOW_HEIGHT = 400;
const size_t MIXER_PIXEL_COUNT   = MIXER_WINDOW_WIDTH * MIXER_WINDOW_HEIGHT;

const size_t PICTURE_GENERATIONS_COUNT = 1;


//--------------------------------------------------


#include "constants.hpp"


//--------------------------------------------------


#include "structs.hpp"


//--------------------------------------------------


typedef struct Image_Mixer {

    Mixer_output output;

    Mixer_media media;

    Mixer_conditions conditions;

    Mixer_data data;

} Image_Mixer; const size_t MIXER_SIZE = sizeof (Image_Mixer);


//--------------------------------------------------


typedef struct Fps_handler {

    clock_t cur_frame_time;
    clock_t old_frame_time;

    size_t frame_number;

    double sum_delays_ms;
    double sum_fps;

} Fps_handler; const size_t FPS_HANDLER_SIZE = sizeof (Fps_handler);


//--------------------------------------------------

Return_code merge_pictures (Picture32* result, Picture32* top, Picture24* bottom, Point offset);

//--------------------------------------------------

Return_code initialize_sdl (void);
Return_code quit_sdl       (void);

Image_Mixer* mixer_ctor            (void);
Return_code  mixer_output_ctor     (Mixer_output* output);
Return_code  mixer_media_ctor      (Mixer_media* media);
Return_code  mixer_conditions_ctor (Mixer_conditions* conditions);
Return_code  mixer_data_ctor       (Mixer_data* data);

Return_code mixer_dtor        (Image_Mixer* mixer);
Return_code mixer_output_dtor (Mixer_output* graphics);
Return_code mixer_media_dtor  (Mixer_media* media);

//--------------------------------------------------

Return_code mixer_load_media  (Image_Mixer* mixer, const char* top_pic_name    = TOP_PICTURE_PATH,
                                                  const char* bottom_pic_name = BOTTOM_PICTURE_PATH);
Return_code mixer_load_top    (Image_Mixer* mixer, const char* file_name);
Return_code mixer_load_bottom (Image_Mixer* mixer, const char* file_name);
Return_code mixer_load_pic    (void** buffer_ptr, int* width, int* height, const char* file_name);

Return_code mixer_update_window_size_and_result (Image_Mixer* mixer);
Return_code mixer_generate_result               (Image_Mixer* mixer);
int         mixer_calculate_result_width        (Image_Mixer* mixer);
int         mixer_calculate_result_height       (Image_Mixer* mixer);
Return_code mixer_generate_output               (Image_Mixer* mixer);

size_t      mixer_get_file_size      (FILE* file);
size_t      mixer_get_data_offset    (FILE* file);
Return_code mixer_check_signature    (FILE* file);
Return_code mixer_check_header_size  (FILE* file);
Return_code mixer_load_picture_sizes (FILE* file, int* width, int* height);

//--------------------------------------------------

Return_code mixer_work (Image_Mixer* mixer);

Return_code   mixer_generate_picture (Image_Mixer* mixer);
Return_code   mixer_render_picture   (Image_Mixer* mixer);
Return_code   mixer_render_pixel     (Image_Mixer* mixer, size_t x, size_t y);
Pixel_color32 mixer_get_pixel_color  (Image_Mixer* mixer, size_t x, size_t y);

Return_code mixer_update_conditions (Image_Mixer* mixer);

//--------------------------------------------------

double my_min (double val1, double val2);

//--------------------------------------------------

Fps_handler* fps_handler_ctor          (void);
Return_code  fps_handler_dtor          (Fps_handler* handler);
Return_code  fps_handler_next_frame    (Fps_handler* handler);
double       fps_handler_get_delay_ms  (Fps_handler* handler);
double       fps_handler_get_fps       (Fps_handler* handler);
double       fps_handler_get_avg_delay (Fps_handler* handler);
double       fps_handler_get_avg_fps   (Fps_handler* handler);
Return_code  fps_handler_print         (Fps_handler* handler);

//--------------------------------------------------
#endif