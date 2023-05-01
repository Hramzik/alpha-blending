

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


Return_code load_buffer32_from_buffer24 (Picture32* dst, Picture24* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_from_pixel24 (dst->buffer, src->buffer);


        dst->buffer += 1;
        src->buffer += 1;
    }


    return SUCCESS;
}


Return_code load_buffer32_from_buffer32 (Picture32* dst, Picture32* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_from_pixel32 (dst->buffer, src->buffer);


        dst->buffer += 1;
        src->buffer += 1;
    }


    return SUCCESS;
}


Return_code load_buffer32_default (Picture32* dst, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        load_pixel32_default (dst->buffer);


        dst->buffer += 1;
    }


    return SUCCESS;
}


Return_code merge_buffer32 (Picture32* dst, Picture32* top, Picture24* bottom, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    for (; *column < max_column; (*column)++) {

        merge_pixel (dst->buffer, top->buffer, bottom->buffer);


        dst->buffer    += 1;
        top->buffer    += 1;
        bottom->buffer += 1;
    }


    return SUCCESS;
}

