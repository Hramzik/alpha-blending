

//--------------------------------------------------


#include "../../hpp/graphics/graphics.hpp"


//--------------------------------------------------


#include "../../lib/logs.hpp"


//--------------------------------------------------


Return_code mixer_update_conditions (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Event event = {};


    while (SDL_PollEvent (&event)) {

        if (event.type == SDL_QUIT) {

            mixer->conditions.exit = true;
            break;
        }


        if (event.type == SDL_KEYDOWN) mixer_update_conditions_keydown (mixer, event);


        if (event.type == SDL_KEYUP)   mixer_update_conditions_keyup   (mixer, event);
    }


    return SUCCESS;
}


Return_code mixer_update_conditions_keydown (Image_Mixer* mixer, SDL_Event event) {

    if (!mixer)                      { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (event.type != SDL_KEYDOWN) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    switch (event.key.keysym.sym) {

        case SDLK_UP: {

            if (mixer->conditions.pressed_up) break;

            mixer->data.vertical_speed  -= VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_up = true;
            break;
        }

        case SDLK_w: {

            if (mixer->conditions.pressed_w) break;

            mixer->data.vertical_speed -= VERTICAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_w = true;
            break;
        }

        case SDLK_LEFT: {

            if (mixer->conditions.pressed_left) break;

            mixer->data.horizontal_speed  -= HORIZONTAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_left = true;
            break;
        }

        case SDLK_a: {

            if (mixer->conditions.pressed_a) break;

            mixer->data.horizontal_speed -= HORIZONTAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_a   = true;
            break;
        }

        case SDLK_DOWN: {

            if (mixer->conditions.pressed_down) break;

            mixer->data.vertical_speed    += VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_down = true;
            break;
        }

        case SDLK_s: {

            if (mixer->conditions.pressed_s) break;

            mixer->data.vertical_speed += VERTICAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_s = true;
            break;
        }

        case SDLK_RIGHT: {

            if (mixer->conditions.pressed_right) break;

            mixer->data.horizontal_speed   += VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_right = true;
            break;
        }

        case SDLK_d: {

            if (mixer->conditions.pressed_d) break;

            mixer->data.horizontal_speed += HORIZONTAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_d   = true;
            break;
        }

        default: break;
    }


    return SUCCESS;
}


Return_code mixer_update_conditions_keyup (Image_Mixer* mixer, SDL_Event event) {

    if (!mixer)                    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (event.type != SDL_KEYUP) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    switch (event.key.keysym.sym) {

        case SDLK_UP: {

            mixer->data.vertical_speed  += VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_up = false;
            break;
        }

        case SDLK_w: {

            mixer->data.vertical_speed += VERTICAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_w = false;
            break;
        }

        case SDLK_LEFT: {

            mixer->data.horizontal_speed  += HORIZONTAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_left = false;
            break;
        }

        case SDLK_a: {

            mixer->data.horizontal_speed += HORIZONTAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_a   = false;
            break;
        }

        case SDLK_DOWN: {

            mixer->data.vertical_speed    -= VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_down = false;
            break;
        }

        case SDLK_s: {

            mixer->data.vertical_speed -= VERTICAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_s = false;
            break;
        }

        case SDLK_RIGHT: {

            mixer->data.horizontal_speed   -= VERTICAL_MOVE_SMALL_STEP;
            mixer->conditions.pressed_right = false;
            break;
        }

        case SDLK_d: {

            mixer->data.horizontal_speed -= HORIZONTAL_MOVE_BIG_STEP;
            mixer->conditions.pressed_d   = false;
            break;
        }

        case SDLK_h: mixer_mirror_horizontally (mixer); break;
        case SDLK_v: mixer_mirror_vertically   (mixer); break;

        case SDLK_t: mixer->data.currently_mirroring = PIC_ID_TOP;    break;
        case SDLK_b: mixer->data.currently_mirroring = PIC_ID_BOTTOM; break;


        case SDLK_f: mixer_save_result (mixer); break;


        default: break;
    }


    return SUCCESS;
}


Return_code mixer_handle_conditions (Image_Mixer* mixer) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    mixer->data.top_pic_offset.x += mixer->data.horizontal_speed;
    mixer->data.top_pic_offset.y += mixer->data.vertical_speed;


    return SUCCESS;
}


Return_code mixer_mirror_horizontally (Image_Mixer* mixer) {

    if (!mixer)                                           { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (mixer->data.currently_mirroring == PIC_ID_RESULT) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Picture_Id pic_id = mixer->data.currently_mirroring;

    int left_column  = 0;
    int right_column = 0;
    int width        = 0;
    int height       = 0;
    void* pixels     = nullptr;

    if (pic_id == PIC_ID_TOP) {

        right_column = TOP_PIC.width - 1;
        width        = TOP_PIC.width;
        height       = TOP_PIC.height;
        pixels       = TOP_PIC.buffer;
    }

    else {

        right_column = BOTTOM_PIC.width - 1;
        width        = BOTTOM_PIC.width;
        height       = BOTTOM_PIC.height;
        pixels       = BOTTOM_PIC.buffer;
    }


    for (; left_column < right_column; left_column++, right_column--) {

        Point columns = { left_column, right_column };
        Point dimensions = { width, height };

        swap_columns (pixels, pic_id, columns, dimensions);
    }


    return SUCCESS;
}
Return_code mixer_mirror_vertically   (Image_Mixer* mixer) {

    if (!mixer)                                           { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (mixer->data.currently_mirroring == PIC_ID_RESULT) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Picture_Id pic_id = mixer->data.currently_mirroring;

    int top_row      = 0;
    int bottom_row   = 0;
    int width        = 0;
    int height       = 0;
    void* pixels     = nullptr;

    if (pic_id == PIC_ID_TOP) {

        bottom_row = TOP_PIC.height - 1;
        width      = TOP_PIC.width;
        height     = TOP_PIC.height;
        pixels     = TOP_PIC.buffer;
    }

    else {

        bottom_row = BOTTOM_PIC.height - 1;
        width      = BOTTOM_PIC.width;
        height     = BOTTOM_PIC.height;
        pixels     = BOTTOM_PIC.buffer;
    }


    for (; top_row < bottom_row; top_row++, bottom_row--) {

        Point rows       = { top_row, bottom_row };
        Point dimensions = { width, height };

        swap_rows (pixels, pic_id, rows, dimensions);
    }


    return SUCCESS;
}


Return_code swap_columns (void* buffer, Picture_Id pic_id, Point columns, Point dimensions) {

    if (!buffer)                 { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (pic_id == PIC_ID_RESULT) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int row = 0; row < dimensions.y; row++) {

        int offset_1  = row * dimensions.x + columns.x;
        int offset_2  = row * dimensions.x + columns.y;
        Point offsets = { offset_1, offset_2 };

        swap_pixels (buffer, pic_id, offsets);
    }


    return SUCCESS;
}
Return_code swap_rows    (void* buffer, Picture_Id pic_id, Point rows,    Point dimensions) {

    if (!buffer)                 { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (pic_id == PIC_ID_RESULT) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int column = 0; column < dimensions.x; column++) {

        int offset_1  = rows.x * dimensions.x + column;
        int offset_2  = rows.y * dimensions.x + column;
        Point offsets = { offset_1, offset_2 };

        swap_pixels (buffer, pic_id, offsets);
    }


    return SUCCESS;
}


Return_code swap_pixels (void* buffer, Picture_Id pic_id, Point offsets) {

    if (!buffer)                 { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (pic_id == PIC_ID_RESULT) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    if (pic_id == PIC_ID_TOP)    return swap_pixels32 ( (Pixel_Color32*) buffer, offsets);
    if (pic_id == PIC_ID_BOTTOM) return swap_pixels24 ( (Pixel_Color24*) buffer, offsets);


    return CRITICAL;
}


Return_code swap_pixels32 (Pixel_Color32* buffer, Point offsets) {

    if (!buffer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Pixel_Color32 old_buffer_at_offsets_x = buffer [offsets.x];
    buffer [offsets.x]                    = buffer [offsets.y];
    buffer [offsets.y]                    = old_buffer_at_offsets_x;


    return SUCCESS;
}


Return_code swap_pixels24 (Pixel_Color24* buffer, Point offsets) {

    if (!buffer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Pixel_Color24 old_buffer_at_offsets_x = buffer [offsets.x];
    buffer [offsets.x]                    = buffer [offsets.y];
    buffer [offsets.y]                    = old_buffer_at_offsets_x;


    return SUCCESS;
}


//--------------------------------------------------


Return_code mixer_save_result (Image_Mixer* mixer, const char* path) {

    if (!mixer) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    SDL_Surface* temp = mixer_generate_result_surface (mixer);
    if (!temp) { LOG_ERROR (CRITICAL); return CRITICAL; }


    IMG_SavePNG (temp, path);


    SDL_FreeSurface (temp);


    return SUCCESS;
}


//--------------------------------------------------