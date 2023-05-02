

#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------



Return_code load_buffer32_from_buffer24_opt (Picture32* dst, Picture24* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int max_intrin_column = *column + ((max_column - *column) / 8) * 8;

    for (; *column < max_intrin_column; (*column) += 8) {

        load_8_pixels32_from_pixels24 (dst->buffer, src->buffer);


        dst->buffer += 8;
        src->buffer += 8;
    }


    load_buffer32_from_buffer24 (dst, src, column, max_column); // finish


    return SUCCESS;
}


Return_code load_buffer32_from_buffer32_opt (Picture32* dst, Picture32* src, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!src)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int max_intrin_column = *column + ((max_column - *column) / 8) * 8;

    for (; *column < max_intrin_column; (*column) += 8) {

        load_8_pixels32_from_pixels32 (dst->buffer, src->buffer);


        dst->buffer += 8;
        src->buffer += 8;
    }


    load_buffer32_from_buffer32 (dst, src, column, max_column); // finish



    return SUCCESS;
}


Return_code load_buffer32_default_opt (Picture32* dst, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int max_intrin_column = *column + ((max_column - *column) / 8) * 8;

    for (; *column < max_intrin_column; (*column) += 8) {

        load_8_pixels32_default (dst->buffer);


        dst->buffer += 8;
    }


    load_buffer32_default (dst, column, max_column); // finish


    return SUCCESS;
}


Return_code merge_buffer32_opt (Picture32* dst, Picture32* top, Picture24* bottom, int* column, int max_column) {

    if (!dst)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!column) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int max_intrin_column = *column + ((max_column - *column) / 8) * 8;

    for (; *column < max_intrin_column; (*column) += 8) {

        merge_8_pixels (dst->buffer, top->buffer, bottom->buffer, 32);


        dst->buffer    += 8;
        top->buffer    += 8;
        bottom->buffer += 8;
    }


    merge_buffer32 (dst, top, bottom, column, max_column); // finish


    return SUCCESS;
}


