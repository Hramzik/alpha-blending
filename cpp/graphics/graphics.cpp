

//--------------------------------------------------


#include "../../hpp/graphics/graphics.hpp"


//--------------------------------------------------


#include "../../lib/logs.cpp"


//--------------------------------------------------


#include "c-dtors.cpp"
#include "fps_handler.cpp"


//--------------------------------------------------


Return_code mixer_load_media (Image_Mixer* mixer, const char* top_pic_name, const char* bottom_pic_name) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (top_pic_name)    mixer_load_top    (mixer,    top_pic_name);
    if (bottom_pic_name) mixer_load_bottom (mixer, bottom_pic_name);


    mixer_update_window_size_and_result (mixer);


    return SUCCESS;
}


Return_code mixer_load_top (Image_Mixer* mixer, const char* file_name) {

    if (!mixer)     { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!file_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    void** buffer_ptr = (void**) &mixer->media.top_pic.buffer;
    int*   width      =          &mixer->media.top_pic.width;
    int*   height     =          &mixer->media.top_pic.height;


    mixer_load_pic (buffer_ptr, width, height, file_name);


    return SUCCESS;
}


Return_code mixer_load_bottom (Image_Mixer* mixer, const char* file_name) {

    if (!mixer)     { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!file_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    void**  buffer_ptr = (void**) &mixer->media.bottom_pic.buffer;
    int*    width      =          &mixer->media.bottom_pic.width;
    int*    height     =          &mixer->media.bottom_pic.height;


    return mixer_load_pic (buffer_ptr, width, height, file_name);
}


// updates window size and result_pic buffer!!!

Return_code mixer_update_window_size_and_result (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int pic_width   = mixer_calculate_result_width  (mixer);
    int pic_height  = mixer_calculate_result_height (mixer);

    int result_width   = mixer->media.result_pic.width;
    int result_height  = mixer->media.result_pic.height;

    int window_width  = mixer->data.window_width;
    int window_height = mixer->data.window_height;

    //--------------------------------------------------

    if (result_width != pic_width || result_height != pic_height) {

        mixer->media.result_pic.width  = pic_width; printf ("debug: passed point %d %d\n", pic_width, pic_height);
        mixer->media.result_pic.height = pic_height;

        mixer_generate_result (mixer);
    }


    if (window_width != pic_width || window_height != pic_height) {

        mixer->data.window_width  = pic_width;
        mixer->data.window_height = pic_height;

        mixer_generate_output (mixer);
    }


    return SUCCESS;
}


int mixer_calculate_result_width (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int left_border  = my_min (0,                                                          mixer->data.top_pic_offset.x);
    int right_border = my_max (mixer->media.bottom_pic.width, mixer->media.top_pic.width + mixer->data.top_pic_offset.x);


    return right_border - left_border;
}


int mixer_calculate_result_height (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int lower_border = my_min (0,                                                            mixer->data.top_pic_offset.y);
    int upper_border = my_max (mixer->media.bottom_pic.height, mixer->media.top_pic.height + mixer->data.top_pic_offset.y);


    return upper_border - lower_border;
}


Return_code mixer_generate_result (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    size_t new_buffer_len = mixer->media.result_pic.width * mixer->media.result_pic.height;


    mixer->media.result_pic.buffer = (Pixel_color32*) realloc (mixer->media.result_pic.buffer, new_buffer_len * PIXEL_COLOR32_SIZE);


    return SUCCESS;
}


Return_code mixer_generate_output (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (mixer->output.renderer) SDL_DestroyRenderer (mixer->output.renderer);
    if (mixer->output.window)   SDL_DestroyWindow   (mixer->output.window);


    mixer->output.window = SDL_CreateWindow ("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        (int) mixer->data.window_width, (int) mixer->data.window_height, MIXER_OUTPUT_FLAGS); // empty header

    if(!mixer->output.window) {

        LOG_MESSAGE ("Window could not be created!\n");
        return LIB_ERR;
    }


    //--------------------------------------------------


    mixer->output.renderer = SDL_CreateRenderer(mixer->output.window, -1, SDL_RENDERER_ACCELERATED);

    if (!mixer->output.renderer) {

        LOG_MESSAGE ("Renderer could not be created!");
        return LIB_ERR;
    }


    return SUCCESS;
}


//--------------------------------------------------
#define BUFFER (*buffer_ptr)
//--------------------------------------------------

Return_code mixer_load_pic (void** buffer_ptr, int* width, int* height, const char* file_name) {

    if (!buffer_ptr) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!file_name)  { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    FILE* file = fopen (file_name, "rb");


    mixer_check_signature    (file);
    mixer_check_header_size  (file);
    mixer_load_picture_sizes (file, width, height);

    size_t file_size   = mixer_get_file_size   (file);
    size_t data_offset = mixer_get_data_offset (file);
    size_t buffer_size = file_size - data_offset;


    BUFFER = realloc (BUFFER, buffer_size);


    fseek (file, (long) data_offset, SEEK_SET);
    fread (BUFFER, CHAR_SIZE, buffer_size, file);


    return SUCCESS;
}

//--------------------------------------------------
#undef BUFFER
//--------------------------------------------------


size_t mixer_get_file_size (FILE* file) {

    if (!file) { LOG_ERROR (BAD_ARGS); return 0; }


    size_t result = 0;


    fseek (file, FILE_SIZE_OFFSET, SEEK_SET);
    fread (&result, CHAR_SIZE, FILE_SIZE_LEN, file); // read file len


    return result;
}


size_t mixer_get_data_offset (FILE* file) {

    if (!file) { LOG_ERROR (BAD_ARGS); return 0; }


    size_t result = 0;


    fseek (file, DATA_OFFSET_OFFSET, SEEK_SET);
    fread (&result, CHAR_SIZE, DATA_OFFET_LEN, file); // read data offset


    return result;
}


Return_code mixer_check_signature (FILE* file) {

    if (!file) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    size_t signature = 0;


    fseek (file, SIGNATURE_OFFSET, SEEK_SET);
    fread (&signature, CHAR_SIZE, SIGNATURE_LEN, file); // read signature


    if (signature != SIGNATURE_ETALON) { LOG_ERROR (FILE_ERR); return FILE_ERR; }


    return SUCCESS;
}


Return_code mixer_check_header_size (FILE* file) {

    if (!file) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    size_t header_size = 0;


    fseek (file, HEADER_SIZE_OFFSET, SEEK_SET);
    fread (&header_size, CHAR_SIZE, HEADER_SIZE_LEN, file); // read header_size


    if (header_size != HEADER_SIZE_ETALON1 && 
        header_size != HEADER_SIZE_ETALON2 &&
        header_size != HEADER_SIZE_ETALON3) { LOG_ERROR (FILE_ERR); return FILE_ERR; }


    return SUCCESS;
}


Return_code mixer_load_picture_sizes (FILE* file, int* width, int* height) {

    if (!file)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!width)  { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!height) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    *width = 0;
    fseek (file, WIDTH_OFFSET, SEEK_SET);
    fread (width, CHAR_SIZE, WIDTH_LEN, file); // read width

    if (*width <= 0) { LOG_ERROR (FILE_ERR); return FILE_ERR; }


    //--------------------------------------------------


    *height = 0;
    fseek (file, HEIGHT_OFFSET, SEEK_SET);
    fread (height, CHAR_SIZE, HEIGHT_LEN, file); // read height

    if (*height <= 0) { LOG_ERROR (FILE_ERR); return FILE_ERR; }


    return SUCCESS;
}


Return_code mixer_work (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Fps_handler* fps_counter = fps_handler_ctor ();


    while(true) {

    //--------------------------------------------------
        mixer_update_conditions (mixer);

        if (mixer->conditions.exit) break;
    //--------------------------------------------------


        for (size_t i = 0; i < PICTURE_GENERATIONS_COUNT; i++) {

            mixer_generate_picture (mixer);
        }


        SDL_RenderClear (mixer->output.renderer);
        mixer_render_picture   (mixer);
        SDL_RenderPresent (mixer->output.renderer);


        fps_handler_next_frame (fps_counter);
        fps_handler_print (fps_counter);
    }


    fps_handler_dtor (fps_counter);


    return SUCCESS;
}


Return_code mixer_generate_picture (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    merge_pictures (&mixer->media.result_pic, &mixer->media.top_pic, &mixer->media.bottom_pic, mixer->data.top_pic_offset);


    return SUCCESS;
}


Return_code mixer_render_picture (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int x = 0; x < mixer->data.window_width; x++) {

        for (int y = 0; y < mixer->data.window_height; y++) {

            mixer_render_pixel (mixer, x, y);
        }
    }


    return SUCCESS;
}


//--------------------------------------------------

#define MY_RENDERER mixer->output.renderer

//--------------------------------------------------

Return_code mixer_render_pixel (Image_Mixer* mixer, size_t x, size_t y) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Pixel_color32 color = mixer_get_pixel_color (mixer, x, y);


    SDL_SetRenderDrawColor (MY_RENDERER, color.red, color.green, color.blue, color.transparency);
    SDL_RenderDrawPoint    (MY_RENDERER, (int) x, (int) y);


    return SUCCESS;
}


Pixel_color32 mixer_get_pixel_color (Image_Mixer* mixer, size_t x, size_t y) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return { 0, 0, 0, 255 }; }


    size_t index = y * mixer->media.result_pic.width + x;


    return mixer->media.result_pic.buffer [index];
}


Return_code mixer_update_conditions (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Event event = {};


    while (SDL_PollEvent (&event)) {

        if (event.type == SDL_QUIT) {

            mixer->conditions.exit = true;
            break;
        }


        // if (event.type == SDL_KEYDOWN) mixer_update_conditions_keydown (mixer, event);


        // if (event.type == SDL_KEYUP)   mixer_update_conditions_keyup   (mixer, event);
    }


    return SUCCESS;
}

