

//--------------------------------------------------


#include "../../hpp/alpha/alpha.hpp"


//--------------------------------------------------


#include "../../lib/logs.hpp"


//--------------------------------------------------



Return_code copy_top_upper (Picture32* result, int* line, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (0, top->height + offset.y);

    while (*line < last_line) {

        copy_top_line (result, top, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code fill_void_upper (Picture32* result, int* line) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < 0) {

        load_line_default (result);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_bottom_upper (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (offset.y, bottom->height);

    while (*line < last_line) {

        copy_bottom_line (result, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code merge_top_with_bottom (Picture32* result, int* line, Picture32* top, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    int last_line = my_min (offset.y + top->height, bottom->height);

    while (*line < last_line) {

        merge_line (result, top, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_bottom_lower (Picture32* result, int* line, Picture24* bottom, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!bottom) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < bottom->height) {

        copy_bottom_line (result, bottom, offset);
        *line += 1;
    }


    return SUCCESS;
}


Return_code fill_void_lower (Picture32* result, int* line, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < offset.y) {

        load_line_default (result);
        *line += 1;
    }


    return SUCCESS;
}


Return_code copy_top_lower (Picture32* result, int* line, Picture32* top, Point offset) {

    if (!result) { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!line)   { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }
    if (!top)    { LOG_ERROR (BAD_ARGS); return BAD_ARGS; }


    while (*line < offset.y + top->height) {

        copy_top_line (result, top, offset);
        *line += 1;
    }


    return SUCCESS;
}

