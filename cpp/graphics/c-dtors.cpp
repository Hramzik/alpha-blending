

//--------------------------------------------------
#include "../../hpp/graphics/graphics.hpp"

#include "../../lib/logs.cpp"
//--------------------------------------------------


Image_Mixer* mixer_ctor (void) {

    Image_Mixer* mixer = (Image_Mixer*) calloc (1, MIXER_SIZE);


    mixer_output_ctor (&mixer->output);


    mixer_media_ctor (&mixer->media);


    mixer_conditions_ctor (&mixer->conditions);


    return mixer;
}


Return_code mixer_dtor (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mixer_output_dtor (&mixer->output);
    mixer_media_dtor  (&mixer->media);


    free (mixer);


    return SUCCESS;
}


Return_code initialize_sdl (void) {

    if(SDL_Init (SDL_INIT_VIDEO) < 0){

        LOG_MESSAGE ("SDL could not initialize!\n");
        return LIB_ERR;
    }


    if (!SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1")) {

        LOG_MESSAGE ("Error: Linear texture filtering not enabled!");
        return LIB_ERR;
    }


    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init (imgFlags) & imgFlags)) {

        LOG_MESSAGE ("SDL_image could not initialize!\n");
        return LIB_ERR;
    }


    return SUCCESS;
}


Return_code quit_sdl (void) {


    SDL_Quit ();
    IMG_Quit ();


    return SUCCESS;
}


Return_code mixer_output_ctor (Mixer_Output* output) {

    if (!output) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    output->window = SDL_CreateWindow ("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, MIXER_OUTPUT_FLAGS); // empty header

    if(!output->window) {

        LOG_MESSAGE ("Window could not be created!\n");
        return LIB_ERR;
    }


    //--------------------------------------------------


    output->renderer = SDL_CreateRenderer(output->window, -1, SDL_RENDERER_ACCELERATED);

    if (!output->renderer) {

        LOG_MESSAGE ("Renderer could not be created!");
        return LIB_ERR;
    }


    return SUCCESS;
}


Return_code mixer_output_dtor (Mixer_Output* output) {

    if (!output) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (output->renderer) SDL_DestroyRenderer (output->renderer);
    if (output->window)   SDL_DestroyWindow   (output->window);


    return SUCCESS;
}


Return_code mixer_conditions_ctor (Mixer_Conditions* conditions) {

    if (!conditions) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    conditions->exit = false;

    conditions->pressed_w = false;
    conditions->pressed_a = false;
    conditions->pressed_s = false;
    conditions->pressed_d = false;

    conditions->pressed_left  = false;
    conditions->pressed_right = false;
    conditions->pressed_up    = false;
    conditions->pressed_down  = false;


    return SUCCESS;
}


Return_code mixer_media_ctor (Mixer_Media* media) {

    if (!media) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    media->top_pic.buffer = nullptr;
    media->top_pic.width  = 0;
    media->top_pic.height = 0;

    media->bottom_pic.buffer = nullptr;
    media->bottom_pic.width  = 0;
    media->bottom_pic.height = 0;

    media->result_pic.buffer = nullptr;
    media->result_pic.width  = 0;
    media->result_pic.height = 0;


    return SUCCESS;
}


Return_code mixer_media_dtor (Mixer_Media* media) {

    if (!media) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    free (media->top_pic.buffer);
    free (media->bottom_pic.buffer);
    free (media->result_pic.buffer);


    return SUCCESS;
}


Return_code mixer_data_ctor (Mixer_Data* data) {

    if (!data) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    data->top_pic_offset = { 0, 0 };

    data->window_width  = 0;
    data->window_height = 0;

    data->horizontal_speed = 0;
    data->vertical_speed   = 0;

    data->currently_mirroring = PIC_ID_TOP;


    return SUCCESS;
}

