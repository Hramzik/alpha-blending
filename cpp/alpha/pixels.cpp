

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "../../lib/logs.hpp"


//--------------------------------------------------





Return_code load_pixel32_from_pixel24 (Pixel_Color32* dst, Pixel_Color24* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = src->red;
    dst->green = src->green;
    dst->blue  = src->blue;

    dst->transparency = BACKGROUND_TRANSPARENCY;


    return SUCCESS;
}


Return_code load_pixel32_from_pixel32 (Pixel_Color32* dst, Pixel_Color32* src) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = src->red;
    dst->green = src->green;
    dst->blue  = src->blue;

    dst->transparency = src->transparency;


    /*if (dst->transparency == 0) { // фон верхней картинки

        dst->red   = TOP_BACKGROUND_RED;
        dst->green = TOP_BACKGROUND_GREEN;
        dst->blue  = TOP_BACKGROUND_BLUE;

        dst->transparency = TOP_BACKGROUND_TRANSPARENCY;
    }*/


    return SUCCESS;
}


Return_code load_pixel32_default (Pixel_Color32* dst) {

    if (!dst) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    dst->red   = DEFAULT_RED;
    dst->green = DEFAULT_GREEN;
    dst->blue  = DEFAULT_BLUE;

    dst->transparency = DEFAULT_TRANSPARENCY;


    return SUCCESS;
}


Return_code merge_pixel (Pixel_Color32* result, Pixel_Color32* top, Pixel_Color24* bottom) {

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

