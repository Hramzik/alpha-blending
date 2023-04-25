

//--------------------------------------------------


#include "../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "../lib/logs.hpp"


//--------------------------------------------------


Return_code merge_pictures (Picture32* result, Picture32* top, Picture24* bottom, Point offset) { // point = top-left

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    Buffer_saver3* saved_buffers = save_buffers (top, bottom, result);


    copy_pixels_until_equal_top (result, bottom, offset);
    merge_pixels_from_equal_top (result, top, bottom, offset);


    restore_buffers (top, bottom, result, saved_buffers);



    return SUCCESS;
}


// moves buffer pointers respectively

Return_code copy_pixels_until_equal_top (Picture32* result, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int i = 0; i < offset.y; i++) {

        copy_line (result, bottom);
    }


    return SUCCESS;
}


// fills result line from bottom line and default remainder
// moves buffer pointers respectively

Return_code copy_line (Picture32* result, Picture24* bottom) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (size_t i = 0; i < bottom->width; i++) {

        load_pixel32_from_pixel24 (result->buffer, bottom->buffer);


        result->buffer += 1;
        bottom->buffer += 1;
    }


    //--------------------------------------------------


    for (size_t i = bottom->width; i < result->width; i++) {

        load_pixel32_default (result->buffer);


        result->buffer += 1;
    }


    return SUCCESS;
}


Return_code merge_pixels_from_equal_top (Picture32* result, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int border_y = my_min ((int) bottom->height, (int) top->height + offset.y);

    for (int i = offset.y; i < border_y; i++) {

        merge_line (result, top, bottom, offset);
    }


    return SUCCESS;
}


Return_code merge_line (Picture32* result, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (int i = 0; i < offset.x; i++) {

        load_pixel32_from_pixel24 (result->buffer, bottom->buffer);


        result->buffer += 1;
        bottom->buffer += 1;
    }

    //--------------------------------------------------

    for (int i = offset.x; i < offset.x + (int) top->width; i++) {

        merge_pixels (result->buffer, top->buffer, bottom->buffer);


        result->buffer += 1;
        top->buffer    += 1;
        bottom->buffer += 1;
    }

    //--------------------------------------------------

    for (int i = offset.x + (int) top->width; i < (int) bottom->width; i++) {

        load_pixel32_from_pixel24 (result->buffer, bottom->buffer);


        result->buffer += 1;
        bottom->buffer += 1;
    }


    return SUCCESS;
}


Return_code load_pixel32_from_pixel24 (Pixel_color32* dst, Pixel_color24* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = src->red;
    dst->green = src->green;
    dst->blue  = src->blue;

    dst->transparency = BACKGROUND_TRANSPARENCY;


    return SUCCESS;
}


Return_code load_pixel32_default (Pixel_color32* dst) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = DEFAULT_RED;
    dst->green = DEFAULT_GREEN;
    dst->blue  = DEFAULT_BLUE;

    dst->transparency = DEFAULT_TRANSPARENCY;


    return SUCCESS;
}


Return_code merge_pixels (Pixel_color32* result, Pixel_color32* top, Pixel_color24* bottom) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    double coefficient = (double) top->transparency / 255;

    result->red   = (unsigned char) ((coefficient) * top->red   + (1 - coefficient) * bottom->red);
    result->green = (unsigned char) ((coefficient) * top->green + (1 - coefficient) * bottom->green);
    result->blue  = (unsigned char) ((coefficient) * top->blue  + (1 - coefficient) * bottom->blue);

    result->transparency = MERGED_TRANSPARENCY;


    return SUCCESS;
}


Buffer_saver3* save_buffers (Picture32* top, Picture24* bottom, Picture32* result) {

    if (!top)    { LOG_ERROR (BAD_ARGS); return nullptr; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return nullptr; }
    if (!result) { LOG_ERROR (BAD_ARGS); return nullptr; }


    Buffer_saver3* ans = (Buffer_saver3*) calloc (1, BUFFER_SAVER3_SIZE);


    ans->top    = top->buffer;
    ans->bottom = bottom->buffer;
    ans->result = result->buffer;


    return ans;
}


Return_code restore_buffers (Picture32* top, Picture24* bottom, Picture32* result, Buffer_saver3* saver) {

    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!saver)  { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    top->buffer    = saver->top;
    bottom->buffer = saver->bottom;
    result->buffer = saver->result;


    free (saver);


    return SUCCESS;
}

