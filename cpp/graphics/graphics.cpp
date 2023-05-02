

//--------------------------------------------------


#include "../../hpp/graphics/graphics.hpp"


//--------------------------------------------------


#include "../../lib/logs.cpp"


//--------------------------------------------------


#include "c-dtors.cpp"
#include "fps_handler.cpp"
#include "conditions.cpp"


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


    mixer_load_pic (buffer_ptr, width, height, &mixer->data.bottom_bits_per_pixel, file_name);


    return SUCCESS;
}


Return_code mixer_load_bottom (Image_Mixer* mixer, const char* file_name) {

    if (!mixer)     { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!file_name) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    void**  buffer_ptr = (void**) &mixer->media.bottom_pic.buffer;
    int*    width      =          &mixer->media.bottom_pic.width;
    int*    height     =          &mixer->media.bottom_pic.height;


    return mixer_load_pic (buffer_ptr, width, height, nullptr, file_name);
}


Return_code mixer_convert_bottom_from_24bit_to_32bit (Image_Mixer* mixer) {

    if (!mixer)     { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int pixel_count = BOTTOM_PIC.width * BOTTOM_PIC.height;

    Pixel_Color32* new_buffer = (Pixel_Color32*) calloc (pixel_count, PIXEL_COLOR32_SIZE);
    Pixel_Color24* old_buffer = BOTTOM_PIC.buffer;


    for (int i = 0; i < pixel_count; i++) {

        new_buffer [i].red = old_buffer [i].red;
        new_buffer [i].red = old_buffer [i].green;
        new_buffer [i].red = old_buffer [i].blue;
        new_buffer [i].red = 255;
    }


    free (old_buffer);


    BOTTOM_PIC.buffer = (Pixel_Color24*) new_buffer;


    mixer->data.bottom_bits_per_pixel = 32;


    return SUCCESS;
}


// updates window size and result_pic buffer!!!

Return_code mixer_update_window_size_and_result (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int pic_width   = mixer_calculate_result_width  (mixer);
    int pic_height  = mixer_calculate_result_height (mixer);

    int result_width   = RESULT_PIC.width;
    int result_height  = RESULT_PIC.height;

    int window_width  = mixer->data.window_width;
    int window_height = mixer->data.window_height;

    //--------------------------------------------------

    if (result_width != pic_width || result_height != pic_height) {

        mixer->media.result_pic.width  = pic_width;
        mixer->media.result_pic.height = pic_height;

        mixer_generate_result (mixer);
    }


    if (window_width != pic_width || window_height != pic_height) {

        mixer->data.window_width  = pic_width;
        mixer->data.window_height = pic_height;

        mixer_resize_output (mixer);
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


    size_t new_buffer_len = RESULT_PIC.width * RESULT_PIC.height;


    RESULT_PIC.buffer = (Pixel_Color32*) realloc (RESULT_PIC.buffer, new_buffer_len * PIXEL_COLOR32_SIZE);


    return SUCCESS;
}


Return_code mixer_resize_output (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_SetWindowSize (mixer->output.window, mixer->data.window_width, mixer->data.window_height);


    return SUCCESS;
}


//--------------------------------------------------
#define BUFFER (*buffer_ptr)
//--------------------------------------------------

Return_code mixer_load_pic (void** buffer_ptr, int* width, int* height, int* bits_per_pixel, const char* file_name) {

    if (!buffer_ptr) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!file_name)  { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    FILE* file = fopen (file_name, "rb");


    mixer_check_signature     (file);
    mixer_check_header_size   (file);
    mixer_load_picture_sizes  (file, width, height);

    if (bits_per_pixel) mixer_load_bits_per_pixel (file, bits_per_pixel);


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


Return_code mixer_load_bits_per_pixel (FILE* file, int* bits_per_pixel) {

    if (!file) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bits_per_pixel) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }



    fseek (file, BITS_PER_PIXEL_OFFSET, SEEK_SET);
    fread (bits_per_pixel, CHAR_SIZE, BITS_PER_PIXEL_LEN, file); // read bits per pixel


    return SUCCESS;
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


    Fps_Handler* fps_counter = fps_handler_ctor ();


    mixer->data.currently_mirroring = PIC_ID_BOTTOM;
    mixer_mirror_vertically (mixer);
    mixer->data.currently_mirroring = PIC_ID_TOP;
    mixer_mirror_vertically (mixer);

    int top_offset_x = mixer->data.window_width  * 1 / 3;
    int top_offset_y = mixer->data.window_height * 1 / 3;

    mixer->data.top_pic_offset = { .x = top_offset_x, .y = top_offset_y };


    while(true) {

    //--------------------------------------------------
        mixer_update_conditions (mixer);

        if (mixer->conditions.exit) break;

        mixer_handle_conditions (mixer);
    //--------------------------------------------------


        mixer_generate_picture (mixer);


        SDL_RenderClear (mixer->output.renderer);
        mixer_render_picture (mixer);
        SDL_RenderPresent (mixer->output.renderer);


        fps_handler_next_frame (fps_counter);
        fps_handler_print (fps_counter);
    }


    fps_handler_dtor (fps_counter);


    return SUCCESS;
}


Return_code mixer_generate_picture (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mixer_update_window_size_and_result (mixer);


    for (size_t i = 0; i < PICTURE_GENERATIONS_COUNT; i++) {

        merge_pictures (&RESULT_PIC, &TOP_PIC, &BOTTOM_PIC, mixer->data.top_pic_offset);
    }


    return SUCCESS;
}


Return_code mixer_render_picture_by_pixels (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int x = 0; x < mixer->data.window_width; x++) {

        for (int y = 0; y < mixer->data.window_height; y++) {

            mixer_render_pixel (mixer, x, y);
        }
    }


    return SUCCESS;
}


Return_code mixer_render_picture (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Texture* result_texture = mixer_generate_result_texture (mixer);


    SDL_RenderCopy (mixer->output.renderer, result_texture, nullptr, nullptr);


    SDL_DestroyTexture (result_texture);


    return SUCCESS;
}


Return_code mixer_render_pixel (Image_Mixer* mixer, size_t x, size_t y) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Pixel_Color32 color = mixer_get_pixel_color (mixer, x, y);


    SDL_SetRenderDrawColor (MY_RENDERER, color.red, color.green, color.blue, color.transparency);
    SDL_RenderDrawPoint    (MY_RENDERER, (int) x, (int) y);


    return SUCCESS;
}


Pixel_Color32 mixer_get_pixel_color (Image_Mixer* mixer, size_t x, size_t y) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return { 0, 0, 0, 255 }; }


    size_t index = y * RESULT_PIC.width + x;


    return RESULT_PIC.buffer [index];
}


SDL_Surface* mixer_generate_result_surface (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return nullptr; }


    void* buffer = RESULT_PIC.buffer;

    int width  = RESULT_PIC.width;
    int height = RESULT_PIC.height;

    int depth = 32;
    int pitch = width * 4; // 4 = bytes per color

    Uint32 rmask = 0; // default mask
    Uint32 gmask = 0;
    Uint32 bmask = 0;
    Uint32 amask = 0;


    SDL_Surface* result_surface = SDL_CreateRGBSurfaceFrom (buffer, width, height, depth, pitch, 
                                                            rmask,  gmask, bmask,  amask);


    if (!result_surface) {

        LOG_MESSAGE ("Unable to create surface!");
        return nullptr;
    }


    return result_surface;
}


SDL_Texture* mixer_generate_result_texture (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return nullptr; }


    SDL_Surface* temp_surface = mixer_generate_result_surface (mixer);
    if (!temp_surface) { LOG_ERROR (CRITICAL); return nullptr; }


    SDL_Texture* result_texture = SDL_CreateTextureFromSurface (mixer->output.renderer, temp_surface);
    SDL_FreeSurface (temp_surface);


    if (!result_texture) {

        LOG_MESSAGE ("Unable to create texture!");
        return nullptr;
    }


    return result_texture;
}


